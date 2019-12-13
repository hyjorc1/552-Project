#include "mr.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
using namespace std;

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
    int dataSize;
    scanf("%d", &dataSize);

    printf("How many chunks? ");
    int nchunks;
    scanf("%d", &nchunks);

    printf("How many threads ? ");
    int nthreads;
    scanf("%d", &nthreads);

    int (*mapper)(int) = MyMapper;
    int (*reducer)(int, int) = MyReducer;

    vector<int> data;
    for( int i = 1; i <= dataSize; i++ )
        data.push_back(i);


    MR<int, int> mr(&data, nchunks, nthreads, mapper, reducer);
    mr.RunMapReduce();
    

    return 0;
}


