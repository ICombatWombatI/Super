
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
//static unsigned int __stdcall server_f(void *args)// _beginthreadex ��������� ������ ������� ���������� ����(__stdcall), ������� ��� � ���� ���� ���
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
//		getchar();//������ ������ ���� ����� (�� ���� system("pause");). ��� ���� ���� � � ������
//		f_stop = 1;
//		return 1;//����� �� ������� � ���������� ������
//	}
//	return 0;
//}