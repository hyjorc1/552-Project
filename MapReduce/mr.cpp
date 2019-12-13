#include "mr.h"
#include "wcm.h"
#include <stdio.h>
#include <vector>
using namespace std;

MR::MR(void *data, int numberOfChunks, int nthreads, int (*mapper)(int), int (*reducer)(int, int))
{
    this->_data = (vector<int> *)data;
    this->_numberOfChunks = numberOfChunks;
    this->_nthreads = nthreads;
    this->_mapper = mapper;
    this->_reducer = reducer;
};

vector<int> MR::RunMapReduce() {
    vector<vector<int>> chunkRanges = getChuckRanges(*_data, _numberOfChunks);
    vector<int> results(chunkRanges.size());
    vector<int> *resPtr = &results;
    WCM wcm(_data, &chunkRanges, _nthreads, resPtr, _mapper, _reducer);
    return wcm.process();;
};

vector<vector<int>> MR::getChuckRanges(vector<int> data, int numberOfChunks) {
    vector<vector<int>> chunks;
    int chunkSize = data.size() / numberOfChunks;
    int residue = data.size() - numberOfChunks * chunkSize;
    int start;
    int end;
    int head = 0;
    for (int i = 0; i < numberOfChunks - 1; i++)
    {
        start = chunkSize * i + head;
        if (residue > 0)
        {
            end = start + chunkSize;
            residue--;
            head++;
        }
        else
        {
            end = start + chunkSize - 1;
        }
        vector<int> vect{start, end};
        chunks.push_back(vect);
    }
    vector<int> last{(numberOfChunks - 1) * chunkSize + head, (int)(data.size() - 1)};
    chunks.push_back(last);
    return chunks;
};