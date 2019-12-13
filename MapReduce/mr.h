#ifndef _MR_H
#define _MR_H
#include <vector>

class MR
{

private:
    std::vector<int> *_data;
    int _numberOfChunks;
    int _nthreads;
    int (*_mapper)(int);
    int (*_reducer)(int, int);

public:
    MR(void *data, int numberOfChunks, int nthreads, int (*mapper)(int), int (*reducer)(int, int));

    std::vector<std::vector<int>> getChuckRanges(std::vector<int> data, int numberOfChunks);

    std::vector<int> RunMapReduce();
};

#endif
