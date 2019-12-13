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
    int numberOfChunks;
    scanf("%d", &numberOfChunks);

    printf("How many threads ? ");
    int nthreads;
    scanf("%d", &nthreads);

    int (*mapper)(int) = MyMapper;
    int (*reducer)(int, int) = MyReducer;

    vector<int> data;
    for( int i = 1; i <= dataSize; i++ )
        data.push_back(i);
    MR mr(&data, numberOfChunks, nthreads, mapper, reducer);
    vector<int> result = mr.RunMapReduce();
    printf("done %d tasks with result %d\n", result[0], result[1]);

    return 0;
}


