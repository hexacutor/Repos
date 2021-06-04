#pragma once
#include "Interface.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>
#define _CRT_SECURE_NO_WARNINGS//211 М.В.Новак main программы без сервера-клиента
int main()
{
    auto* cc = new Controller(true);
    auto* ii = new Interface(0, true, cc);//211 М.В.Новак инициализация интерфейса и его контроллера
    ii->run();//211 М.В.Новак функция чтения-ожидания
    return 0;
}
