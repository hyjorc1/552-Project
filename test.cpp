#include <iostream>
#include<string>

using namespace std;

int main()
{
    cout << "What is the sound of a cat? " << endl;
    int a = 1;
    int b = 2;
    int *p = &a;
    cout << *p << endl;

    auto func = [&]() { cout << "func" + to_string(a) << endl; };
    func();
    return 0;
}