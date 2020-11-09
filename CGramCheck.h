//
// Created by ldz on 2020/11/8.
//

#ifndef CHECKER_CGRAMCHECK_H
#define CHECKER_CGRAMCHECK_H

#include <string>
#include <iostream>

using namespace std;

#include <fstream>
#include <stdio.h>

#include <vector>
#include <stack>

class CGramCheck {
private:
    ifstream inFile;
    int num_line = 0;
    string str[1000];
public:
    bool readFile(string f_name);
    int getNumberOfEdges(string str_list);
    void matchBrackets();
    void matchSemicolon();

};

bool CGramCheck::readFile(string f_name) {
    inFile.open(f_name, ios::in);
    num_line = getNumberOfEdges(f_name);
//    string str[num_line];
    if (inFile) {
        cout << "readFile success!" << endl;
        int i = 0;
        while (!inFile.eof()) {
            getline(inFile, str[i], '\n');
            i++;
        }
        for (int ii = 0; ii < i; ii++) {
            cout << str[ii] << endl;
        }
        inFile.close();
    } else
        cout << "readFile fail!No such file!" << endl;
}

int CGramCheck::getNumberOfEdges(string str_list) {
    char flag;
    int edgeNum, count;
    FILE *fp = fopen(str_list.c_str(), "rt+");//open the TXT file, can only read, cannot write
    while (!feof(fp)) {

        flag = fgetc(fp);
        if (flag == '\n') count++;
    }
    edgeNum = count + 1;//因为最后一行没有换行符\n，所以需要在count补加1
    fclose(fp);
    cout << " the edge number is " << edgeNum << endl;
    return edgeNum;
}

void CGramCheck::matchBrackets() {
}
void CGramCheck::matchSemicolon() {

}

#endif //CHECKER_CGRAMCHECK_H
