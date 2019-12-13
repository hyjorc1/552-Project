#include "wcm.h"
#include <stdio.h>
#include <vector>
using namespace std;

template <class T, class U>
class MR
{

private:
    std::vector<int> *_data;
    int _numberOfChunks;
    int _nthreads;
    U (*_mapper)(T);
    U (*_reducer)(U, U);

public:
    MR(std::vector<T> *data, int numberOfChunks, int nthreads, U (*mapper)(T), U (*reducer)(U, U));

    std::vector<std::vector<int>> getChuckRanges(int dataSize, int numberOfChunks);

    U RunMapReduce();
};

template <class T, class U>
MR<T, U>::MR(vector<T> *data, int numberOfChunks, int nthreads, U (*mapper)(T), U (*reducer)(U, U))
{
    this->_data = data;
    this->_numberOfChunks = numberOfChunks;
    this->_nthreads = nthreads;
    this->_mapper = mapper;
    this->_reducer = reducer;
};

template <class T, class U>
U MR<T, U>::RunMapReduce() {
    vector<vector<int>> chunkRanges = getChuckRanges((*_data).size(), _numberOfChunks);
    vector<U> results(chunkRanges.size());
    vector<U> *resPtr = &results;
    WCM<T, U> wcm(_data, &chunkRanges, _nthreads, resPtr, _mapper, _reducer);
    U result = wcm.process();
    printf("Final result %d\n", result);
    return result;
};

template <class T, class U>
vector<vector<int>> MR<T, U>::getChuckRanges(int dataSize, int numberOfChunks) {
    vector<vector<int>> chunks;
    int chunkSize = dataSize / numberOfChunks;
    int residue = dataSize - numberOfChunks * chunkSize;
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
    vector<int> last{(numberOfChunks - 1) * chunkSize + head, (int)(dataSize - 1)};
    chunks.push_back(last);
    return chunks;
};