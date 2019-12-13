#include <iostream>
#include <vector>
#include <string>
using namespace std;

template <typename T>
string toString(T x) { return to_string(x); }
template <>
string toString(string s) { return s; }