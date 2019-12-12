#include "mapreduceconfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
using namespace std;

#define debug 1 // debug mode if 1, otherwise 0

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* task queue as each thread arg */
struct taskqueue
{
    int id;                     // thread id
    int numberOfTasks;          // number of tasks
    int *nextTask;              // index of next task
    vector<int> *data;          // the entire input data
    vector<vector<int>> *queue; // data ranges for each task
    vector<int> *results;       // task results
    int (*mapper)(int);
    int (*reducer)(int, int);
};


/* initialize the task queue */
taskqueue *setTaskQueue(vector<int> *data, vector<vector<int>> *chunkRanges, vector<int> *results, int (*mapper)(int), int (*reducer)(int, int))
{
    taskqueue *tasks;
    tasks = (taskqueue *)calloc(1, sizeof(taskqueue));
    tasks->numberOfTasks = (*chunkRanges).size();
    tasks->nextTask = (int *)calloc(1, sizeof(int));
    *(tasks->nextTask) = 0;
    tasks->data = data;
    tasks->queue = chunkRanges;
    tasks->results = results;
    tasks->mapper = mapper;
    tasks->reducer = reducer;
    return tasks;
}

/* display data ranges for each task */
void write_taskqueue(taskqueue *tasks)
{
    printf("%d tasks : ", tasks->numberOfTasks);
    for (auto vect : *(tasks->queue))
        printf(" (%d, %d)", vect[0], vect[1]);
    printf("\n");
}

/* work on each task */
void *work(void *args)
{
    taskqueue *q = (taskqueue *)args;
    int curChunkIdx;
    do
    {
        curChunkIdx = -1;

        // assign current task and update next task
        if (debug > 0)
            printf("thread %d requests lock ...\n", q->id);
        pthread_mutex_lock(&lock);
        if (debug > 0)
            printf("thread %d gets lock ...\n", q->id);
        int *j = q->nextTask;
        if (*j < q->numberOfTasks)
            curChunkIdx = (*j)++;
        if (debug > 0)
            printf("thread %d the next job is %d ...\n", q->id, curChunkIdx);
        if (debug > 0)
            printf("thread %d releases lock\n", q->id);
        pthread_mutex_unlock(&lock);

        if (curChunkIdx == -1) // check if all tasks are done
            break;

        // start to work on the current task
        vector<int> chunkRange = (*(q->queue))[curChunkIdx];
        vector<int> data = *(q->data);
        int (*mapper)(int) = q->mapper;
        int (*reducer)(int, int) = q->reducer;
        int i;
        int prevResult = -1;
        for (i = chunkRange[0]; i <= chunkRange[1]; i++)
        {
            int e = data[i];
            int curResult = mapper(e);
            prevResult = i == chunkRange[0] ? curResult : reducer(prevResult, curResult);
            printf("thread %d is working on data at idx %d with value %d\n", q->id, i, e);
            // usleep(1e6);
        }

        printf("thread %d done with chunk at idx %d with result %d\n", q->id, curChunkIdx, prevResult);

        pthread_mutex_lock(&lock);
        int tmp = (*(q->results)).size();
        printf("thread %d results size: %d\n", q->id, tmp);
        (*(q->results)).at(curChunkIdx) = prevResult;
        pthread_mutex_unlock(&lock);

        if (debug > 0)
            printf("*** thread %d does job %d ***\n", q->id, curChunkIdx);

        // int w = 1;
        // if (v > 0)
        //     printf("thread %d sleeps %f seconds\n", q->id, w/1e6);
        // usleep(w);

    } while (curChunkIdx != -1);

    if (debug > 0)
        printf("thread %d is finished\n", q->id);

    return NULL;
}

/*
 * Processes the queue of tasks with n threads,
 * returns the number of tasks done. */
vector<int> process_taskqueue(taskqueue *tasks, int n)
{
    pthread_t t[n];
    pthread_attr_t a;
    taskqueue q[n];
    int i;

    printf("creating %d threads ...\n", n);
    for (i = 0; i < n; i++)
    {
        q[i].numberOfTasks = tasks->numberOfTasks;
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

    int prevResult = -1;
    vector<int> finalResults = *(tasks->results);
    int (*reducer)(int, int) = q->reducer;
    
    for (i = 0; i < finalResults.size(); i++)
    {
        int curResult = finalResults[i];
        prevResult = i == 0 ? curResult : reducer(prevResult, curResult);
    }
    vector<int> res{*(tasks->nextTask), prevResult};
    return res;
}

int MyMapper(int x)
{
    usleep(1e3);
    return (x * 2);
};

int MyReducer(int p, int c)
{
    return p + c;
};

int main(int argc, char *argv[])
{
    printf("Data size? ");
    int dataLength;
    scanf("%d", &dataLength);

    printf("How many chunks? ");
    int chunksLength;
    scanf("%d", &chunksLength);

    MapReduceConfig config;
    vector<int> data;
    for( int i = 1; i <= dataLength; i++ )
        data.push_back(i);
    vector<vector<int>> chunkRanges = config.getChuckRanges(data, chunksLength);
    int (*mapper)(int) = MyMapper;
    int (*reducer)(int, int) = MyReducer;
    vector<int> results(chunkRanges.size(), -1);
    vector<int> *resPtr = &results;
    taskqueue *tasks = setTaskQueue(&data, &chunkRanges, resPtr, mapper, reducer);

    if (debug > 0)
        write_taskqueue(tasks);

    printf("How many threads ? ");
    int nthreads;
    scanf("%d", &nthreads);
    vector<int> done = process_taskqueue(tasks, nthreads);
    printf("done %d tasks with result %d\n", done[0], done[1]);

    return 0;
}