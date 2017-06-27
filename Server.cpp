#include "Server.h"

extern StremServer MyStream;

WSockServer::WSockServer(WORD RequestVersion)
{
	int wsOK = WSAStartup(RequestVersion, &wsaData);

	if (wsOK != 0)
		throw "Startup failed.";
}

WSockServer::~WSockServer()
{
	if (WSACleanup() != 0)
		throw "Cleanup failed.";
	if (sServerListen != INVALID_SOCKET)
		closesocket(sServerListen);
	if (ClientSockets != INVALID_SOCKET)
		closesocket(sServerListen);
}


void WSockServer::SetServerSockAddr(sockaddr_in *hint, int PortNumber)
{
	hint->sin_family = AF_INET;
	hint->sin_port = htons(PortNumber);
	hint->sin_addr.S_un.S_addr = INADDR_ANY;			
}

bool WSockServer::RunServer(int PortNumber)
{

	sServerListen = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sServerListen == SOCKET_ERROR)
	{
		throw "sServerListen == SOCKET_ERROR";
	}

	ULONG ulBlock;
	ulBlock = 1;
	if (ioctlsocket(sServerListen, FIONBIO, &ulBlock) == SOCKET_ERROR)
	{
		throw "ulBlocking fail";
	}

	SetServerSockAddr(&hint, PortNumber);

	if (bind(sServerListen, (sockaddr*)(&hint), sizeof(hint)) != 0)
		throw "Could not bind socket.";

	if (listen(sServerListen, 4) != 0)
		throw "Could not put the socket in listening mode.";

	int clientsize = sizeof(client);

	cout << "Waiting for clients... ";
	//char host[NI_MAXHOST]; //им€ клиента(что то вроде)
	//char service[NI_MAXSERV];

	//cout << endl << "Server settings: " << endl;
	//cout << "IP: " << inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST) << endl;
	//cout << "PORT: " << ntohs(client.sin_port) << endl << endl;

	//getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);
	//cout << host << endl;
	return true;
}

void WSockServer::StartChat()
{

	while (true)
	{
		ZeroMemory(SendText, 1024);
		ZeroMemory(RecvText, 1024);

	//	cin.getline(SendText, 1024, '\n');

		//////////////////////////////////////////////////////////////////////////////////
		FD_ZERO(&ReadSet);
		FD_ZERO(&WriteSet);

		FD_SET(sServerListen, &ReadSet);
		//////////////////////////////////////////////////////////////////////////////////
		if (TotalSocket)
		{
			if (ClientSockets != INVALID_SOCKET)
				FD_SET(ClientSockets, &ReadSet);
		}
		if (TotalSocket)
		{
			if (strlen(SendText) != 0)
				if (ClientSockets != INVALID_SOCKET)
					FD_SET(ClientSockets, &WriteSet);
		}

		//////////////////////////////////////////////////////////////////////////////////
		TIMEVAL tval;
		tval.tv_sec = 1;
		tval.tv_usec = 0;

		TIMEVAL *ptval;

		ptval = &tval;

		//////////////////////////////////////////////////////////////////////////////////
		if ((readySock = select(0, &ReadSet, &WriteSet, NULL, ptval)) == SOCKET_ERROR)
		{
			throw" Select error ";
		}

		//≈сть новые подключени€
		//////////////////////////////////////////////////////////////////////////////////
		if (FD_ISSET(sServerListen, &ReadSet))
		{

			int iSize = sizeof(client);
			ClientSockets = accept(sServerListen, (struct sockaddr *)&client, &iSize);

			if (ClientSockets == INVALID_SOCKET)
			{
				throw" Creating client socket error ";
			}
			TotalSocket++;
			cout << "Client connected"<<endl;
		}
		//////////////////////////////////////////////////////////////////////////////////
		if (FD_ISSET(ClientSockets, &ReadSet))
		{
			int bytesReceived = recv(ClientSockets, RecvText, 1024, 0);

			if (bytesReceived == 0)
			{
				closesocket(ClientSockets);
				ClientSockets = INVALID_SOCKET;
				throw" was bin recived 0 ";
			}
			else if (bytesReceived == SOCKET_ERROR)
			{
				throw" Recive data filed ";
			}
			//cout << string(RecvText, bytesReceived) << endl;
			EnterCriticalSection(&MyStream.mguard);
			__try
			{
			//	Buffer[bytesReceived] = 0;
				strcpy_s(MyStream.msg, RecvText);
				MyStream.f_ready = 1;
			}
			__finally
			{
				LeaveCriticalSection(&MyStream.mguard);
			}

		}
		//////////////////////////////////////////////////////////////////////////////////
		if (FD_ISSET(ClientSockets, &WriteSet))
		{
			FD_CLR(ClientSockets, &WriteSet);
			//if (SendText != 0)
				send(ClientSockets, "LoL", strlen("LoL"), 0);
			//memset(&out, 0, sizeof(out));

		}	
	}
}


 unsigned  int  __stdcall StremServer:: server_f(void *args)
{
	try
	{
		WSockServer MyServer(REQ_WINSOCK_VER);
		if (MyServer.RunServer(54000))
		{
		//	cout << "Client connected. " << endl;
			MyServer.StartChat();
		}
	}
	catch (char *ErrMsg)
	{
		cout << "\nError: " << ErrMsg;
		getchar();//‘унци€ просто ждет смвол (по типу system("pause");). ќна есть есть и в Ћинукс
				  //f_stop = 1;
		throw "Stop";
	}
}

 void StremServer:: initializeCS()
{

	InitializeCriticalSection(&mguard);
	server_h = (HANDLE)_beginthreadex(NULL, 0, server_f, NULL, 0, &ThId);
};

 void StremServer::mainCS() {

		 if (f_stop == 1) {
			 throw "Conection lost";
		 }
		 if (f_ready == 1)
		 {
			 EnterCriticalSection(&mguard);
			 __try
			 { //структурна обработка исключений//  __try,__finally - механизм встроеный в виндовс который позвол€ет отлавливать ошибки (стандарт Win32app) 
				 cout << "Message: " << msg << endl;
				 f_ready = 0;
			 }
			 __finally//  __catch тоже существует но механизм  __finally хорош тем что выполноетс€ только тогда когда выполнетс€ код после __try (можно и без них но лучше с ними)
			 {
				 LeaveCriticalSection(&mguard);
			 }
		 }
	 
 }