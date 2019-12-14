#include "mr.h"
#include "demo.h"
#include <chrono>
using namespace std::chrono;

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

    auto start = high_resolution_clock::now();

    if (testNum == 1)
    {
        // test 1 int
        vector<int> data = generateData(dataSize, 1);
        MR<int, int> mr(&data, nchunks, nthreads, timesTwo, sum);
        mr.runMapReduce(true);
    }
    else if (testNum == 2)
    {
        // test 2 double
        vector<double> data = generateData(dataSize, 1.0);
        MR<double, double> mr(&data, nchunks, nthreads, timesThree, sum);
        mr.runMapReduce(true);
    }
    else
    {
        // test 3 string
        vector<string> data = generateStringData(dataSize);
        MR<string, int> mr(&data, nchunks, nthreads, len, max);
        mr.runMapReduce(true);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Execution Time: "
         << duration.count() / 1e6
         << " sec" << endl;

    return 0;
}
