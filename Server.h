#pragma once
#include <iostream>
using namespace std;

#include <WS2tcpip.h>

#pragma comment (lib,"ws2_32.lib")
class WSockServer
{
public:
	WSockServer(WORD RequestVersion);
	~WSockServer();

	bool RunServer(int PortNumber);	
	void StartChat();

private:
	WSADATA wsaData;
	SOCKET listening;
	SOCKET ClientSocket;
	sockaddr_in hint;
	sockaddr_in client;

	char Buffer[1024];

	void SetServerSockAddr(sockaddr_in *sockAddr, int PortNumber);
};

class Parsing
{
	//	string chat;
	//	string name;

	int Poss[4];

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
		char conwert[1024];
		for (int i = 0, k = 0, j = 0; i<1024 || buf[i] == NULL; i++)
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
			}
		}
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