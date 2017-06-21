
#include "Server.h"

#include <iostream>

using namespace std;

void main()
{
	//
	try
	{
		WORD ver = MAKEWORD(2, 2);
		WSockServer MyServer(ver);
		if (MyServer.RunServer(54000))
		{
			MyServer.StartChat();
		}
	}
	catch (char *ErrMsg)
	{
		cout << "\nError: " << ErrMsg;
	}
	return ;
}