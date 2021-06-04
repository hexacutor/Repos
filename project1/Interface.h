
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <cmath>//211 �.�.����� ���������: ����� ��������� ��������� �� ��������� ������ ��� �����
#include <sstream>
#include <vector>
#include "Controller.h"
class Interface
{
    friend class Controller;
public:
    int id;
    bool write_log;
    Interface(int id, bool write_log, Controller* c);
    int run(); //211 �.�.����� �������� ������� ������ � ��������
    string com(string main);//211 �.�.����� ������ �������
    int command(stringstream* ss);//211 �.�.����� ������ �������
    void log(const string& s);//211 �.�.����� ������ � ��� ����
    void show(const string& s);//211 �.�.����� ����� ��������� �� �����
private:
    Controller* cc;//211 �.�.����� ����������
    ofstream logger;//211 �.�.����� ����� ��� �����
    string log_file_name;//211 �.�.����� ��� ��� �����
};
