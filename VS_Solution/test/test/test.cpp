#include <iostream>
#include <string>
using namespace std;

int main() {
    const int x = 10;
    int* y = const_cast<int*>(&x);
    // 此处只能修改*y的值, 无法修改x的值, 因为在C++中修改一个const变量，如x，通过解引用const_cast得到的指针是未定义行为
    *y = 20;
    printf("%x  %d\n", &x, x);
    printf("%x  %d\n", y, *y);  
    return 0;
}