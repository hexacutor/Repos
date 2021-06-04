
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <cmath>//211 М.В.Новак интерфейс: класс обработки сообщений из командной строки или файла
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
    int run(); //211 М.В.Новак основная функция чтения и ожидания
    string com(string main);//211 М.В.Новак мёртвые функции
    int command(stringstream* ss);//211 М.В.Новак мёртвые функции
    void log(const string& s);//211 М.В.Новак запись в лог файл
    void show(const string& s);//211 М.В.Новак вывод сообщения на экран
private:
    Controller* cc;//211 М.В.Новак контроллер
    ofstream logger;//211 М.В.Новак поток лог файла
    string log_file_name;//211 М.В.Новак имя лог файла
};
