#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <vector>
#include <string>
using namespace std;

pthread_mutex_t task_lock = PTHREAD_MUTEX_INITIALIZER;

template <typename T, typename U>
class WCM
{

private:
    struct TaskQueue;

    TaskQueue *tasks;

    int nthreads;

    static bool debug; // debug mode if 1, otherwise 0

    TaskQueue *setTaskQueue(std::vector<T> *data, std::vector<std::vector<int>> *chunkRanges, std::vector<U> *results, U (*mapper)(T), U (*reducer)(U, U));

    void printTaskQueue(TaskQueue *tasks);

    static void *work(void *args);

    static void chunksMapreduce(TaskQueue *q, int curChunkIdx);

    U processMultiThreads(TaskQueue *tasks, int n);

public:
    WCM(std::vector<T> *data, std::vector<std::vector<int>> *chunkRanges, int nthreads, std::vector<U> *results, U (*mapper)(T), U (*reducer)(U, U));

    U process();

    static void setDebugMode(bool debug);
};

// private
template <class T, class U>
struct WCM<T, U>::TaskQueue
{
    int id;                     // thread id
    int ntasks;                 // number of tasks
    int *nextTask;              // index of next task
    vector<T> *data;            // the entire input data
    vector<vector<int>> *queue; // data ranges for each task
    vector<U> *results;         // task results
    U (*mapper)
    (T); // mapper function
    U (*reducer)
    (U, U); // reducer function
};

template <class T, class U>
bool WCM<T, U>::debug = false;

template <class T, class U>
typename WCM<T, U>::TaskQueue *WCM<T, U>::setTaskQueue(vector<T> *data, vector<std::vector<int>> *chunkRanges, vector<U> *results, U (*mapper)(T), U (*reducer)(U, U))
{
    TaskQueue *tasks;
    tasks = (TaskQueue *)calloc(1, sizeof(TaskQueue));
    tasks->ntasks = (*chunkRanges).size();
    tasks->nextTask = (int *)calloc(1, sizeof(int));
    *(tasks->nextTask) = 0;
    tasks->data = data;
    tasks->queue = chunkRanges;
    tasks->results = results;
    tasks->mapper = mapper;
    tasks->reducer = reducer;
    return tasks;
};

template <class T, class U>
void WCM<T, U>::printTaskQueue(TaskQueue *tasks)
{
    printf("%d tasks : ", tasks->ntasks);
    for (auto chunk : *(tasks->queue))
        printf(" (%d, %d)", chunk[0], chunk[1]);
    printf("\n");
};

template <class T, class U>
void *WCM<T, U>::work(void *args)
{
    TaskQueue *q = (TaskQueue *)args;
    int curChunkIdx;
    do
    {
        curChunkIdx = -1;

        // assign current task and update next task
        if (debug)
            printf("thread %d requests lock ...\n", q->id);
        pthread_mutex_lock(&task_lock);
        if (debug)
            printf("thread %d gets lock ...\n", q->id);
        int *j = q->nextTask;
        if (*j < q->ntasks)
            curChunkIdx = (*j)++;
        if (debug)
            printf("thread %d the next job is %d ...\n", q->id, curChunkIdx);
        if (debug)
            printf("thread %d releases lock ...\n", q->id);
        pthread_mutex_unlock(&task_lock);

        if (curChunkIdx == -1) // check if all tasks are done
            break;

        // start to work on the current task
        chunksMapreduce(q, curChunkIdx);

        if (debug)
            printf("*** thread %d completed job %d ***\n", q->id, curChunkIdx);

    } while (curChunkIdx != -1);

    if (debug)
        printf("****** thread %d is finished ******\n", q->id);

    return NULL;
}

template <class T, class U>
void WCM<T, U>::chunksMapreduce(TaskQueue *q, int curChunkIdx)
{
    vector<int> chunkRange = (*(q->queue))[curChunkIdx];
    vector<T> data = *(q->data);
    U(*mapper)
    (T) = q->mapper;
    U(*reducer)
    (U, U) = q->reducer;
    int i;
    U prevResult;
    for (i = chunkRange[0]; i <= chunkRange[1]; i++)
    {
        T e = data[i];

        if (debug)
            printf("thread %d is working on data point at idx %d with value %s ...\n", q->id, i, toString(e).c_str());

        U curResult = mapper(e);
        prevResult = i == chunkRange[0] ? curResult : reducer(prevResult, curResult);
    }

    if (debug)
        printf("thread %d done with chunk at idx %d with result %s ...\n", q->id, curChunkIdx, toString(prevResult).c_str());

    pthread_mutex_lock(&task_lock);
    int tmp = (*(q->results)).size();
    (*(q->results)).at(curChunkIdx) = prevResult;
    pthread_mutex_unlock(&task_lock);
};

template <class T, class U>
U WCM<T, U>::processMultiThreads(TaskQueue *tasks, int n)
{
    pthread_t t[n];
    pthread_attr_t a;
    TaskQueue q[n];
    int i;

    // map process
    printf("creating %d threads ...\n", n);
    for (i = 0; i < n; i++)
    {
        q[i].ntasks = tasks->ntasks;
        q[i].id = i;
        q[i].nextTask = tasks->nextTask;
        q[i].data = tasks->data;
        q[i].queue = tasks->queue;
        q[i].results = tasks->results;
        q[i].mapper = tasks->mapper;
        q[i].reducer = tasks->reducer;
        pthread_attr_init(&a);
        pthread_create(&t[i], &a, work, (void *)&q[i]);
    }
    printf("waiting for threads to return ...\n");
    for (i = 0; i < n; i++)
        pthread_join(t[i], NULL);

    // reduce process
    U prevResult;
    vector<U> finalResults = *(tasks->results);
    U(*reducer)
    (U, U) = q->reducer;
    for (i = 0; i < finalResults.size(); i++)
    {
        U curResult = finalResults[i];
        prevResult = i == 0 ? curResult : reducer(prevResult, curResult);
    }
    printf("Done %d tasks\n", *(tasks->nextTask));
    return prevResult;
};

// public
template <class T, class U>
WCM<T, U>::WCM(vector<T> *data, vector<vector<int>> *chunkRanges, int nthreads, vector<U> *results, U (*mapper)(T), U (*reducer)(U, U))
{
    this->tasks = WCM::setTaskQueue(data, chunkRanges, results, mapper, reducer);
    if (debug) printTaskQueue(tasks);
    this->nthreads = nthreads;
};

template <class T, class U>
U WCM<T, U>::process()
{
    return processMultiThreads(tasks, nthreads);
};

template <class T, class U>
void WCM<T, U>::setDebugMode(bool d)
{
    debug = d;
};