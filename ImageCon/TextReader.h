#pragma once

#include<sstream> 
#include<iostream>
#include<fstream>
#include<stdio.h>
#include <direct.h>
using namespace std;
class reader
{
public:
    reader();
    ~reader();

    static string readFile(char *filename);
private:

};

reader::reader()
{
}

reader::~reader()
{
}

string reader::readFile(char*filename) {
    char buff[1000];
    _getcwd(buff, 1000);
    cout << "当前路径是：" << buff << endl;

    ifstream in(filename, ios::in);
    std::stringstream  sb;
    sb << in.rdbuf();
    
    return string(sb.str());
}
