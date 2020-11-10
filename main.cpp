#include <iostream>
using namespace std;
#include "CGramCheck.h"

int main() {
    CGramCheck G;
    G.readFile("tobecheck.cpp"); //todo:绝对路径改成相对路径

    G.checkComment();   //检查注释
    G.matchBrackets();  //检查括号
    G.checkquotation(); //检查引号
    G.matchSemicolon(); //检查分号
    G.checkOperator();  //检查运算符

    G.saveFile("checkres.txt");//输出文件
    return 0;
}
