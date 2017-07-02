#pragma once
#include <iostream>
#include <vector>
#include <WS2tcpip.h>
#include <process.h>
#include <windows.h>
#include <string>
#include <time.h>

#define REQ_WINSOCK_VER 2
#pragma comment (lib,"ws2_32.lib")
using namespace std;

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