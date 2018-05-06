#pragma once

#include<sstream> 
#include<iostream>
#include<fstream>
#include<stdio.h>
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
    

    ifstream in(filename, ios::in);
    std::stringstream  sb;
    sb << in.rdbuf();
    
    return string(sb.str());
}
