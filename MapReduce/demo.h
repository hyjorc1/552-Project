#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// *** ------ mapper ------ ***
// test 1
int timesTwo(int x)
{
    usleep(1e6);
    return (x * 2);
};
// test 2
double timesThree(double x)
{
    usleep(1e6);
    return (x * 3);
};
// test 3
int len(string s)
{
    usleep(1e6);
    return s.length();
};

// *** ------ reducer ------ ***
template <typename T> 
T sum(T p, T c)
{
    return p + c;
};

template <typename T> 
T max(T p, T c)
{
    if (p > c)
        return p;
    return c;
};

// *** ------ data ------ ***
template <typename T>
vector<T> generateData(int dataSize, T x)
{
    vector<T> data;
    for (int i = 1; i <= dataSize; i++)
        data.push_back((T)i);
    return data;
};

vector<string> generateStringData(int dataSize)
{
    vector<string> data;
    for (int i = 1; i <= dataSize; i++)
        data.push_back(to_string(i).c_str());
    return data;
};

