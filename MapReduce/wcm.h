#ifndef _WCM_H
#define _WCM_H
#include <vector>

class WCM
{

private:
    struct taskqueue;

    taskqueue *tasks;

    int nthreads;

    taskqueue *setTaskQueue(std::vector<int> *data, std::vector<std::vector<int>> *chunkRanges, std::vector<int> *results, int (*mapper)(int), int (*reducer)(int, int));

    void write_taskqueue(taskqueue *tasks);

    static void *work(void *args);

    std::vector<int> process_taskqueue(taskqueue *tasks, int n);

public:
    WCM(std::vector<int> *data, std::vector<std::vector<int>> *chunkRanges, int nthreads, std::vector<int> *results, int (*mapper)(int), int (*reducer)(int, int));

    std::vector<int> process();
};

#endif