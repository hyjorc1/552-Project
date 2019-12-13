#include "mr.h"
#include "demo.h"

int main(int argc, char *argv[])
{
    printf("Which test? ");
    int testNum;
    scanf("%d", &testNum);

    printf("Data size? ");
    int dataSize;
    scanf("%d", &dataSize);

    printf("How many chunks? ");
    int nchunks;
    scanf("%d", &nchunks);

    printf("How many threads? ");
    int nthreads;
    scanf("%d", &nthreads);

    if (testNum == 1)
    {
         // test 1 int
        vector<int> data = generateData(dataSize, 1);
        MR<int, int> mr(&data, nchunks, nthreads, timesTwo, sum);
        mr.RunMapReduce();
    }
    else if (testNum == 2)
    {
         // test 2 double
        vector<double> data = generateData(dataSize, 1.0);
        MR<double, double> mr(&data, nchunks, nthreads, timesThree, sum);
        mr.RunMapReduce();
    }
    else
    {
         // test 3 string
        vector<string> data = generateStringData(dataSize);
        MR<string, int> mr(&data, nchunks, nthreads, len, max);
        mr.RunMapReduce();
    }

    return 0;
}
