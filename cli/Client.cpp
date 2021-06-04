// 211 М.В.Новак 
/**
*  В.Д.Валединский
*  Простейший пример TCP клиента и сервера.
*
*  Файлы
*  cli_tcp.cpp  - пример TCP клиента
*  ser_tcp_select.cpp - пример TCP сервера c использованием select()
*
*  Клиент в цикле получает с клавиатуры текстовую строку и отсылает
*  ее на сервер, читает и выводит на экран ответ сервера.
*  Сервер ждет соединений от клиентов. При установленом
*  соединении получает строку от клиента, переводит ее в верхний
*  регистр и отсылает обратно клиенту. Если клиент посылает строку
*  содержащую слово stop, то сервер закрывает соединение с этим клиентом,
*  а клиент заканчивает свою работу.
*  Клиент и сервер печатают на экран протокол своей работы т.е.
*  разные данные, которые они получают или отсылают.
*
*  Для ясности примеры составлены максимально просто и не анализируют
*  некорректные ответы и запросы клиента и сервера, возможность переполнения
*  буферов ввода вывода, неожиданное 'падение' сервера и т.п.
*
*/


#pragma warning(suppress : 4996)
#pragma comment(lib, "Ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include "windows.h"
#include <time.h>
using namespace std;
// Определимся с портом, адресом сервера и другими константами.
// В данном случае берем произвольный порт и адрес обратной связи
// (тестируем на одной машине).
#define  SERVER_PORT     5555
#define  SERVER_NAME    "127.0.0.1"
#define  BUFLEN          10

// Две вспомогательные функции для чтения/записи (см. ниже)
int  writeToServer(int fd, double t );// 211 М.В.Новак отправка серверу
int  readFromServer(int fd);// 211 М.В.Новак чтение с сервера
void log(const string& s);// 211 М.В.Новак строка в лог файл
void show(const string& s);// 211 М.В.Новак вывод на экран сообщения


int  main(int argc, char** argv)// 211 М.В.Новак главный процесс, ожидание команд и cin
{
	const char* serverName;
	serverName = (argc < 2) ? "127.0.0.1" : argv[1];
	int err;
	int sock;
	struct sockaddr_in server_addr;
	//struct hostent    *hostinfo;

	// инициализация windows sockets
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed\n");
		return -1;
	}

	// Получаем информацию о сервере по его DNS имени
	// или точечной нотации IP адреса.
	//hostinfo = gethostbyname(SERVER_NAME);
	//if (hostinfo == NULL) {
	//	fprintf(stderr, "Unknown host %s.\n", SERVER_NAME);
	//	exit(EXIT_FAILURE);
	//}
	// можно было бы использовать GetAddrInfo()

	// Заполняем адресную структуру для последующего
	// использования при установлении соединения
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	//server_addr.sin_addr = *(struct in_addr*) hostinfo->h_addr;
	unsigned int iaddr;
	inet_pton(AF_INET, serverName, &iaddr);
	server_addr.sin_addr.s_addr = iaddr;

	// Создаем TCP сокет.
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Client: socket was not created");
		exit(EXIT_FAILURE);
	}

	// Устанавливаем соединение с сервером
	err = connect(sock, (struct sockaddr*) & server_addr, sizeof(server_addr));
	if (err < 0) {
		perror("Client:  connect failure");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "Connection is ready\n");

	// Обмениваемся данными
    cout << "HELP-spisok komand" << endl;
    string main;
    cin >> main;
    for (auto& c : main) c = toupper(c);
    while (main != "EXIT")
    {
        if (main == "HELP")
        {
            string manual ="help.txt";
                ifstream help(manual);
                if (!help.is_open()) {
                    log("Help file not found: " + manual);
                }
                else {
                    string cur_line;
                    log("Help file:");
                    while (getline(help, cur_line)) {
                        show(cur_line);
                    }
                    help.close();
                }           
        }
        else if (main == "GC")
        {
                double  disp_x, disp_y, c_x, c_y;
                int dots;
                cin >> c_x >> c_y >> disp_x >> disp_y >> dots;
                if (cin.fail())
                {
                    show("Bad values");
                    show("Default cloud created");
                    writeToServer(sock, 1); Sleep(1);
                    writeToServer(sock,0); Sleep(1);
                    writeToServer(sock, 0); Sleep(1);
                    writeToServer(sock, 1); Sleep(1);
                    writeToServer(sock, 1); Sleep(1);
                    writeToServer(sock, 100); Sleep(1);
                    writeToServer(sock, 1000); Sleep(1);
                    if (readFromServer(sock)==2)
                    {
                        log("->oblaco sozdano s par-mi: " + to_string(1) + " " + to_string(1) + " " + to_string(0) + " " + to_string(0) + " " + to_string(100));
                        show("HELP-spisok komand");
                    }
                    else
                    {
                        log(" error: no answer server ");
                        break;
                    }
                    cin.clear();
                }
                else
                {
                    writeToServer(sock, 1);
                    Sleep(1);
                    writeToServer(sock, c_x);
                    Sleep(1);
                    writeToServer(sock, c_y); Sleep(1);
                    writeToServer(sock, disp_x); Sleep(1);
                    writeToServer(sock, disp_y); Sleep(1);
                    writeToServer(sock, dots); Sleep(1);
                    writeToServer(sock, 1000); Sleep(1);
                    if (readFromServer(sock) == 2)
                    {
                        show("oblaco sozdano");
                        log("->oblaco sozdano s par-mi" + to_string(disp_x) + " " + to_string(disp_y) + " " + to_string(c_x) + " " + to_string(c_y) + " " + to_string(dots));
                    }
                    else
                    {
                        log(" error: no answer server ");
                        break;
                    }
                }
        }
        else if (main == "GEN")
        {
            int q;
            cin >> q;
            if (cin.fail())
            {
                show("Bad values");
                show("Default random buffer created");
                writeToServer(sock, 2); Sleep(1);
                writeToServer(sock, 500); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                if (readFromServer(sock) == 2)
                {
                    log("->buffer sgenerirovan,tochek: " + to_string(500));
                    show("->buffer sgenerirovan,tochek: " + to_string(500));
                }
                else
                {
                    log(" error: no answer server ");
                    break;
                }
                cin.clear();
            }
            else
            {
                writeToServer(sock, 2); Sleep(1);
                writeToServer(sock, q); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                if (readFromServer(sock) == 2)
                {
                show("buffer sgenerirovan");
                log("buffer sgenerirovan,tochek: " + to_string(q));
                }
                else
                {
                    log(" error: no answer server ");
                    break;
                }
            }
        }
        else if (main == "UNLOAD")
        {
        writeToServer(sock, 3); Sleep(1);
        writeToServer(sock, 1000); Sleep(1);
        if (readFromServer(sock) == 2)
        {
            show("buffer vigruzhen");
        }
        else
        {
            log(" error: no answer server ");
            break;
        }
        }
        else if (main == "MATRIX")
        {
            writeToServer(sock, 4); Sleep(1);
            writeToServer(sock, 1000); Sleep(1);
            if (readFromServer(sock) == 2)
            {
            show("matrica sozdana");
            show("pole perevedeno v sosotoyanie 2(analis)");
                  
                }
       else
        {
        log(" error: no answer server ");
        break;
        }
        }
        else if (main == "KMC")
        {
            int k, p;
            cin >> k >> p;
            if (cin.fail())
            {
                show("Bad values");
                show("KMeans with default values (3,3)");
                writeToServer(sock, 5); Sleep(1);
                writeToServer(sock, 3); Sleep(1);
                writeToServer(sock, 3); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                if (readFromServer(sock) == 2)
                {
                    log("KMeans with values (3,3) printed");
                }
                else
                {
                    log(" error: no answer server ");
                    break;
                }
                cin.clear();
            }
            else
            {
                writeToServer(sock, 5); Sleep(1);
                writeToServer(sock, k); Sleep(1);
                writeToServer(sock, p); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                if (readFromServer(sock) == 2)
                {
                    show("proscanirovano(kmcore)");
                    show("pole raspechatano");
                    log("KMeans with values ( " + to_string(k) + "," + to_string(p) + ") printed");
                }
                else
                {
                    log(" error: no answer server ");
                    break;
                }
            }
        }
        else if (main == "FOREL")
        {
            double r;
            int dep;
            cin >> r >> dep;
            if (dep < 1)
            {
                show("Bad values");
                show("HELP-spisok komand");
            }
            else
            {
                if (cin.fail())
                {
                    show("Bad values");
                    show("Forel with default value (0.1) and depth (2) printed");
                    writeToServer(sock, 6); Sleep(1);
                    writeToServer(sock, 1); Sleep(1);
                    writeToServer(sock, 2); Sleep(1);
                    writeToServer(sock, 1000); Sleep(1);
                    if (readFromServer(sock) == 2)
                    {
                        log("Forel with value (0.1) and depth (2) printed");
                        show("Forel with value (0.1) and depth (2) printed");
                    }
                else
                {
                    log(" error: no answer server ");
                    break;
                }
                    cin.clear();
                }
                writeToServer(sock, 6); Sleep(1);
                writeToServer(sock, r); Sleep(1);
                writeToServer(sock, dep); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                if (readFromServer(sock) == 2)
                {
                    show("Forel with value (" + to_string(r) + ") and depth (" + to_string(dep) + ") printed");
                    log("Forel with value (" + to_string(r) + ") and depth (" + to_string(dep) + ") printed");
                }
                else
                {
                    log(" error: no answer server ");
                    break;
                }
            }
        }
        else if (main == "KM")
        {
            int k;
            cin >> k;
            if (k < 1)
            {
                show("Bad values");
                show("HELP-spisok komand");
            }
            else
            {
                if (cin.fail())
                {
                    show("Bad values");
                    show("Kmeans with default value (5) printed");
                    writeToServer(sock, 7); Sleep(1);
                    writeToServer(sock, 5); Sleep(1);
                    writeToServer(sock, 1000); Sleep(1);
                    if (readFromServer(sock) == 2)
                    {
                        log("Kmeans with value (5) printed");
                    }
                    else
                    {
                        log(" error: no answer server ");
                        break;
                    }
                    cin.clear();
                }
                writeToServer(sock, 7); Sleep(1);
                writeToServer(sock, k); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                if (readFromServer(sock) == 2)
                {
                    show("Kmeans with value (" + to_string(k) + ") printed");
                    log("Kmeans with value (" + to_string(k) + ") printed");
                }
                else
                {
                    log(" error: no answer server ");
                    break;
                }
            }
        }
        else if (main == "EM")
        {
            int k;
            cin >> k;
            if (k < 1)
            {
                show("Bad values");
                show("HELP-spisok komand");
            }
            else
            {
                if (cin.fail())
                {
                    show("Bad values");
                    show("ExpMax with default value (3) printed (gif)");
                    writeToServer(sock, 8); Sleep(1);
                    writeToServer(sock, 3); Sleep(1);
                    writeToServer(sock, 1000); Sleep(1);
                    if (readFromServer(sock) == 2)
                    {
                        log("ExpMax with value (5) printed");
                        cin.clear();
                    }
                    else
                    {
                        log(" error: no answer server ");
                        break;
                    }
                }
                writeToServer(sock, 8); Sleep(1);
                writeToServer(sock, k); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                if (readFromServer(sock) == 2)
                {
                    show("ExpMax with value (" + to_string(k) + ") printed (gif)");
                    log("ExpMax with value (" + to_string(k) + ") printed");
                }
                else
                {
                    log(" error: no answer server ");
                    break;
                }
            }
        }
        else if (main == "DBS")
        {

                double del;
                int k;
                cin >> del >> k;
                writeToServer(sock, 9); Sleep(1);
                writeToServer(sock, del); Sleep(1);
                writeToServer(sock, k); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                if (readFromServer(sock) == 2)
                {
                    show("proscanirovano");
                    log("->proscanirovano(dbs)" + to_string(del) + to_string(k));
                }
                else
                {
                    log(" error: no answer server ");
                    break;
                }
        }
        else if (main == "IER")
        {
        writeToServer(sock, 10); Sleep(1);
        writeToServer(sock, 1000); Sleep(1);
        if (readFromServer(sock) == 2)
        {
            show("Hierarchy algorithm printed");
            log("Hierarchy algorithm printed");
        }
        else
        {
            log(" error: no answer server ");
            break;
        }
        }
        else if (main == "VORONOI")
        {
        writeToServer(sock, 13); Sleep(1);
        writeToServer(sock, 1000); Sleep(1);
        if (readFromServer(sock) == 2)
        {
            show("Voronoi diagram printed");
            log("Voronoi diagram printed");
        }
        else
        {
            log(" error: no answer server ");
            break;
        }
        }
        else if (main == "TREE")
        {
            int err;
            writeToServer(sock, 11); Sleep(1);
            writeToServer(sock, 1000); Sleep(1);
            show("derevo sozdano");
            log("->derevo sozdano s hist " + to_string(30) + " default");
            if (readFromServer(sock) == 2)
            {
                show("derevo raspechatano");
                log("->derevo  raspechatano : default");
            }
        else
        {
        log(" error: no answer server ");
        break;
        }
        }
        else {
            show("neizvestnaya comanda");
            show("HELP-spisok komand");
        }
        cin >> main;
        for (auto& c : main) c = toupper(c);
    }
	//while (1) {
	//	if (writeToServer(sock) < 0) break;
	//	if (readFromServer(sock) < 0) break;
	//}
	//fprintf(stdout, "The end\n");

	// Закрываем socket
	closesocket(sock);
	WSACleanup();
	exit(EXIT_SUCCESS);
}



int  writeToServer(int fd, double t)
{
	int   nbytes;
	int n;
    double x = t * 1000;
    //cout << x << "\n";
    int k =ceil( x);
   // cout << k << "\n";
	char buf[BUFLEN];
	fprintf(stdout, "> ");
		sprintf(buf, "%d", k);
		if (k > 0)
		{
			n = (ceil(log10(k)) + 1);
            nbytes = send(fd, buf, n, 0);
		}
        else if (k == 0)
        {
            send(fd, "0", 1, 0);
            nbytes = 1;
        }
		else
		{
			n = (ceil(log10(abs(k))) + 1);
            nbytes = send(fd, buf, n, 0);
		}
	if (nbytes < 0) { perror("write"); return -1; }
	return 0;
}


int  readFromServer(int fd)
{
    char  buf[BUFLEN];
	int   nbytes;
	nbytes = recv(fd, buf, BUFLEN, 0);
	if (nbytes < 0) {
		// ошибка чтения
		perror("read");
		return -1;
	}
	else if (nbytes == 0) {
		// нет данных для чтения
		fprintf(stderr, "Client: no message\n");
	}
	else {
		// ответ успешно прочитан
		fprintf(stdout, "Server's replay: OK\n");
	}
    if (nbytes>0) return 2;
	return 0;
}


void log(const string& s)
{   
    string log_file_name = "logint.txt";
    ofstream logger = ofstream(log_file_name, ios::app);
        struct tm* a;
        const time_t timer = time(NULL);
        a = localtime(&timer);
        logger << a->tm_mday << "." << (a->tm_mon + 1) << " " << (a->tm_hour) << ":" << (a->tm_min) << ":" << (a->tm_sec) << " -- " << s << endl;
        logger.close();
}
void show(const string& s)
{
    string log_file_name = "logint.txt";
    ofstream logger = ofstream(log_file_name, ios::app);
    cout << s << endl;
    log("<< " + s);
    logger.close();
}

