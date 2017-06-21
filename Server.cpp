#include "Server.h"
#include <string>

WSockServer::WSockServer(WORD RequestVersion)
{
	int wsOK = WSAStartup(RequestVersion, &wsaData);// стартует WSA

	if (wsOK != 0)
	{
		throw "Startup failed.";
	}
}

WSockServer::~WSockServer()
{
	if (WSACleanup() != 0)
		throw "Cleanup failed.";
	if (listening != INVALID_SOCKET)
		closesocket(listening);
	if (ClientSocket != INVALID_SOCKET)
		closesocket(listening);
}


void WSockServer::SetServerSockAddr(sockaddr_in *hint, int PortNumber)
{
	hint->sin_family = AF_INET;
	hint->sin_port = htons(PortNumber);
	hint->sin_addr.S_un.S_addr = INADDR_ANY;			// Listen on all available ip's
}

bool WSockServer::RunServer(int PortNumber)
{

	if ((listening = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)		// Create socket
		throw "Could not create socket.";

	SetServerSockAddr(&hint, PortNumber);

	if (bind(listening, (sockaddr*)(&hint), sizeof(hint)) != 0)
		throw "Could not bind socket.";

	if (listen(listening, SOMAXCONN) != 0)
		throw "Could not put the socket in listening mode.";


	int clientsize = sizeof(client);

	cout << "Waiting for clients... ";
	ClientSocket = accept(listening, (sockaddr*)&client, &clientsize);

	char host[NI_MAXHOST]; //имя клиента(что то вроде)
	char service[NI_MAXSERV];

	cout << endl << "Server settings: " << endl;
	cout << "IP: " << inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST) << endl;
	cout << "PORT: " << ntohs(client.sin_port) << endl << endl;

	getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);
	cout << host << endl;

	return true;
}

void WSockServer::StartChat()
{
	while (true)
	{
			ZeroMemory(Buffer, 1024);
			int bytesReceived = recv(ClientSocket, Buffer, 1024, 0);// получаем данные от клиента


			if (bytesReceived == SOCKET_ERROR)
			{
				throw "Error in recv!";
		
			}
			if (bytesReceived == 0)
			{
				cout << "The client closed the connection. " << endl;
			}

			cout << string(Buffer, bytesReceived) << endl;

			send(ClientSocket, Buffer, bytesReceived + 1,0);// отправляем данные назад к клиенту (если нужно)
	}
}




