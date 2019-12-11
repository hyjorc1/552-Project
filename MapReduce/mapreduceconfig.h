#ifndef _MAPREDUCECONFIG_H
#define _MAPREDUCECONFIG_H
#include <vector>

class MapReduceConfig
{

private:
    std::vector<int> _data;
    int _numberOfChunks;
    int _poolSize;
    int (*_mapper)(void);
    int (*_reducer)(void);

    

public:
    MapReduceConfig(void *data, int numberOfChunks, int poolSize, int (*mapper)(void), int (*reducer)(void));
    MapReduceConfig();
    std::vector< std::vector<int> > getChuckRanges(std::vector<int> data, int numberOfChunks);
    void RunMapReduce();
};

#endif