#pragma once
#include "Interface.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>
#define _CRT_SECURE_NO_WARNINGS//211 �.�.����� main ��������� ��� �������-�������
int main()
{
    auto* cc = new Controller(true);
    auto* ii = new Interface(0, true, cc);//211 �.�.����� ������������� ���������� � ��� �����������
    ii->run();//211 �.�.����� ������� ������-��������
    return 0;
}
