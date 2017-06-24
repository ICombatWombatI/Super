
#include "Server.h"
#include <process.h>

#include <iostream>

using namespace std;

StremServer MyStream;

void main()
{
	MyStream.initializeCS();
	
	while (true)
	{
		try
		{
			MyStream.mainCS();
		}
		catch (char *ErrMsg)
		{
			cout << "\nError: " << ErrMsg;
		}
	}
	
}
//static unsigned int __stdcall server_f(void *args)// _beginthreadex принемает только функции некоторого типа(__stdcall), поэтому тут и есть этот тип
//{
//	try
//	{
//		WSockServer MyServer(REQ_WINSOCK_VER);
//		if (MyServer.RunServer(54000))
//		{
//			cout << "Client connected. " << endl;
//			MyServer.StartChat();
//		}
//	}
//	catch (char *ErrMsg)
//	{
//		cout << "\nError: " << ErrMsg;
//		getchar();//Фунция просто ждет смвол (по типу system("pause");). Она есть есть и в Линукс
//		f_stop = 1;
//		return 1;//выход из функции и завершение потока
//	}
//	return 0;
//}