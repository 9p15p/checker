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

void trim(string &s){
    //去掉空格
    int index = 0;
    if (!s.empty()) {
        while ((index = s.find(' ', index)) != string::npos) {
            s.erase(index, 1);
        }
    }
}

class CGramCheck {
private:
    int num_line = 0;
    string str[1000];
    string str_oneline; //单行形式的代码

    //todo:错误的计数

    int error_flower_bracket =0;  //花括号错误数
    int error_semicolon=0;        //分号错误数
    int error_operator=0;         //运算符错误数
    int error_quotation=0;        //单双引号错误数
    int error_comment=0;          //注释错误数
    int error_circle_bracket=0;   //圆括号错误数

    //external
    int error_square_bracket=0;   //方括号错误数
    int error_sharp_bracket=0;    //尖括号错误数

public:
    bool readFile(string f_name);//读取文本

    int getNumberOfEdges(string f_name);//获取行数

    void checkComment();    //检测两种注释方式 // /**/
    void matchBrackets();   //检测四种括号 ([{<
    void checkquotation();  //检测两种引号 ' "
    void matchSemicolon();  //检测分号 ;
    void checkOperator();   //检测运算符

    void saveFile(string s_name);

};

bool CGramCheck::readFile(string f_name) {
    ifstream inFile;
    inFile.open(f_name, ios::in);
    num_line = getNumberOfEdges(f_name);

    if (inFile) {
        cout << "readFile success!" << endl;
        int i = 0;
        while (!inFile.eof()) {
            getline(inFile, str[i], '\n');
            i++;
        }
        inFile.close();
    } else
        cout << "readFile fail!No such file!" << endl;
}

int CGramCheck::getNumberOfEdges(string f_name) {
    char flag;
    int edgeNum, count;
    FILE *fp = fopen(f_name.c_str(), "rt+");//open the TXT file, can only read, cannot write
    while (!feof(fp)) {
        flag = fgetc(fp);
        if (flag == '\n') count++;
    }
    edgeNum = count + 1;//因为最后一行没有换行符\n，所以需要在count补加1
    fclose(fp);
    cout << "the edge number is " << edgeNum << endl;
    return edgeNum;
}

void CGramCheck::checkComment() {
    for(int i=0;i<num_line;i++){
        string a = str[i];
        trim(a); //去掉a的空格
        if(a.substr(0, 2)=="//")// 如果开头两个字符是//,则把该行赋为空
            str[i] = '\n';
    }

    //转成单行
    for (int i = 0; i < num_line; i++) {
        str_oneline.append(str[i]+'\n');
    }

    string test_oneline = str_oneline;
    int num_char = test_oneline.size();
    char last_char = '\0';
    bool clear_flag = false;
    for(int i=0; i<num_char; i++){
        char this_char = test_oneline[i];

        if(clear_flag)test_oneline[i] = '\0';

        //如果是 /*
        if(last_char=='/' and this_char=='*'){clear_flag=true;test_oneline[i]='\0';test_oneline[i-1]='\0';}
        if(last_char=='*' and this_char=='/')clear_flag=false;

        last_char = this_char;
    }
    if(clear_flag==true){cout << "error: /* is not end \n" << endl;error_comment++;}
}
void CGramCheck::checkquotation() {
    string test_oneline = str_oneline;
    int num_char = test_oneline.size();

    // for "
    int num_Dquo = 0;
    for(int i=0; i<num_char; i++){
        char this_char = test_oneline[i];
        if(this_char=='\"')num_Dquo++;

        // for ' ,且不在双引号内
        if(num_Dquo%2==0){
            if(this_char=='\'')
                if(not(test_oneline[i-2]=='\'' or test_oneline[i+2]=='\''))
                    {cout << "error: \' 在有效语句中,必须隔一个字符出现" << endl;error_quotation++;}
        }

    }
    if(num_Dquo%2!=0){cout << "error, \" must appear in pairs" << endl;error_quotation++;}
}

void CGramCheck::matchBrackets() {
    int num_char = str_oneline.size();
    stack<char> Bracket_stack;
    string Bracket_list = "{}[]()"; //todo:暂时没有"<>"
    char last_br;
    for (int i = 0; i < num_char; i++) {
        int oneline_idx = Bracket_list.find(str_oneline[i]);
        if (oneline_idx != -1) { //如果查找到了是个符号
            switch (str_oneline[i]) {
                case '{'://{
                    Bracket_stack.push(str_oneline[i]);
                    break;
                case '}'://}
                    if(Bracket_stack.empty()){cout << "error,the stack is empty" << endl;error_flower_bracket++;break;} //堆栈为空时,不能出现后括号.
                    last_br = Bracket_stack.top();
                    if(last_br == '{')
                        Bracket_stack.pop(); //是{,则弹出
                    else
                        {cout << "error!last_br is  "<< last_br << endl;error_flower_bracket++;}
                    break;

                case '[': //[
                    Bracket_stack.push(str_oneline[i]);
                    break;
                case ']': //]
                    if(Bracket_stack.empty()){cout << "error,the stack is empty" << endl;error_square_bracket++;break;}
                    last_br = Bracket_stack.top();
                    if(last_br == '[')
                        Bracket_stack.pop(); //是(,则弹出
                    else
                        {cout << "error!last_br is  "<< last_br << endl;error_square_bracket++;}
                    break;

                case '(': //(
                    Bracket_stack.push(str_oneline[i]);
                    break;
                case ')': //)
                    if(Bracket_stack.empty()){cout << "error,the stack is empty" << endl;error_circle_bracket++;break;}
                    last_br = Bracket_stack.top();
                    if(last_br == '(')
                        Bracket_stack.pop(); //是(,则弹出
                    else
                        {cout << "error!last_br is  "<< last_br << endl;error_circle_bracket++;}
                    break;
            }
        }
    }
    //cout <<"Bracket_stack is empty: "<<Bracket_stack.empty()<< endl;
    if (not Bracket_stack.empty()){
        char left_char = Bracket_stack.top();
        cout << "Bracket_stack is not empty: "<< left_char << endl;
        switch (left_char) {
            case '(':;
            case ')':error_circle_bracket++;break;
            case '[':;
            case ']':error_square_bracket++;break;
            case '{':;
            case '}':error_flower_bracket++;break;
            case '<':;
            case '>': error_sharp_bracket++;break;
        }
    }
}

void CGramCheck::matchSemicolon() {
    //出现#则不要;
    //出现(,[,<,{则不需要;
    for(int i=0;i<num_line;i++){
        string a = str[i];
        trim(a); //去掉a的空格
        if(a[a.length()-1]==';')
            if(a[0]=='#' )
                {cout << "error,if use # don't use ; this line." << endl;error_semicolon++;}
            if(a[a.length()-2]=='{')
                {cout << "error,if use { in the end. don't use ; this line." << endl;error_semicolon++;}
            if(a[a.length()-2]=='[')
                {cout << "error,if use [ in the end. don't use ; this line." << endl;error_semicolon++;}
            if(a[a.length()-2]=='(')
                {cout << "error,if use ( in the end. don't use ; this line." << endl;error_semicolon++;}
            if(a[a.length()-2]=='<')
                {cout << "error,if use < in the end. don't use ; this line." << endl;error_semicolon++;}
    }
}
void CGramCheck::checkOperator() {
    //todo:词法分析
    error_operator++;
}
void CGramCheck::saveFile(string s_name) {
    ofstream outFile;
    outFile.open(s_name, ios::out);
    if(outFile){
        outFile << "代码总行数: " << num_line << '\n';
        outFile << "花括号错误数: " << error_flower_bracket << '\n';
        outFile << "分号错误数: " << error_semicolon << '\n';
        outFile << "运算符错误数: " << error_operator << '\n';
        outFile << "单双引号错误数: " << error_quotation << '\n';
        outFile << "注释错误数: " << error_comment << '\n';
        outFile << "圆括号错误数: " << error_circle_bracket << '\n';
        outFile << "方括号错误数: " << error_square_bracket << '\n';
        outFile << "尖括号错误数: " << error_sharp_bracket << '\n';
        outFile.close();
    }
}

#endif //CHECKER_CGRAMCHECK_H
