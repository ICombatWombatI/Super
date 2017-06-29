#pragma once
#include <iostream>
using namespace std;
#include <vector>
#include <WS2tcpip.h>
#include <process.h>
#include <windows.h>
#include <string>
#include <time.h>

#define REQ_WINSOCK_VER 2
#pragma comment (lib,"ws2_32.lib")

//флаг готовности сообщения

struct interThread {
	int ready;
	char msg[1024];
} ;

class WSockServer
{
public:
	WSockServer(WORD RequestVersion);
	~WSockServer();

	bool RunServer(int PortNumber);	
	void StartChat();

private:
	WSADATA wsaData;
	SOCKET sServerListen;
	SOCKET ClientSockets;
	sockaddr_in hint;
	sockaddr_in client;
	FD_SET ReadSet, WriteSet;

	int readySock = 0;
	int bytesReceived = 0;
	int TotalSocket =0 ;

	char SendText[1024];	
	char RecvText[1024];

	void SetServerSockAddr(sockaddr_in *sockAddr, int PortNumber);
};
//////////////////////////////////////////////////////////////////
class Parsing
{
	string chat;
	//	string name;

	///int Poss[4];
	vector <int> Poss;
	//posStartH, posStartV;
	//	int posEndH, posEndV; 

public:

	Parsing()
	{
		for (int i = 0;i < 4;i++)
		{
			Poss[i] = 0;
		}
	};
	~Parsing()
	{

	};
	void parsing(char *buf)
	{
		char conwert[4];
		int Sid=0;
		for (int i = 1, k = 0, j = 0; i < 1024 || buf[i] == NULL; i++)
		{
			if (buf[0])
			{
				if (buf[i] != '\n')
				{
					conwert[j] = buf[i];
					j++;
				}
				else
				{
					j = 0;
					k++;
					Poss[k] = atoi(conwert);
					for (int i = 0;i < 4;i++)
						conwert[i] = NULL;
				}
			}
			else if(!buf[0])
			{
				for (int i = 0;i < 1024 || buf[i] == NULL;i++)
					chat[i] = buf[i];
			}
		}
	
	
		/*for (int i = 0, k = 0, j = 0; i<1024 || buf[i] == NULL; i++)
		{
			if (buf[i] != '\n')
			{
				conwert[j] = buf[i];
				j++;
			}
			else
			{
				j = 0;
				k++;
				Poss[k] = atoi(conwert);
				for (int i = 0;i < 1024;i++)
					conwert[i] = NULL;
			}*/
		//}
	}

	int getPosStartH()
	{
		return Poss[0];
	}
	int getPosStartV()
	{
		return Poss[1];
	}
	int getPosEndH()
	{
		return Poss[2];
	}
	int getPosEndV()
	{
		return Poss[3];
	}

};
class  StremServer
{
	unsigned int ThId;

	static unsigned  int  __stdcall  server_f(void *args);


	public:

	static unsigned int __stdcall  fCin(void *);

	int  f_ready;
		// флаг остановки
	int  f_stop;
		// сообщение
	char msg[1024];
		//критическая секция для недопущения одновременного доступа к msg из двух потоков
	CRITICAL_SECTION mguard;

	HANDLE server_h;

	HANDLE hThread;

	void initializeCS();

	void mainCS();

};