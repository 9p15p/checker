#include <iostream>
using namespace std;
#include "CGramCheck.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    CGramCheck G;
    G.readFile("/home/ldz/CLionProjects/checker/tobecheck.cpp"); //todo:绝对路径改成相对路径
    return 0;
}
