#include <iostream>
using namespace std;
#include "CGramCheck.h"

int main__() {
    std::cout << "Hello, World!" << std::endl;
    CGramCheck G;
    G.readFile("/home/ldz/CLionProjects/checker/tobecheck.cpp");
    return 0;
}