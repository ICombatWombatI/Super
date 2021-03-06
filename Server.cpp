#include "Server.h"

extern StremServer MyStream;
extern interThread interThr;

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
	
	return true;
}

void WSockServer::StartChat()
{
	MyStream.hThread = (HANDLE)_beginthreadex(NULL, 0, MyStream.fCin, NULL, 0, NULL);

	interThr.ready = 0;

	while (true)
	{
		ZeroMemory(SendText, sizeof(SendText));
		ZeroMemory(RecvText, sizeof(RecvText));

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
			if (interThr.ready == 1)
			{
				strcpy_s(SendText, interThr.msg);
				interThr.ready = 0;
			}

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
		//���� ����� �����������
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
		//���� ��� ��������
		//////////////////////////////////////////////////////////////////////////////////
		if (FD_ISSET(ClientSockets, &ReadSet))
		{
			int bytesReceived = recv(ClientSockets, RecvText, sizeof(RecvText), 0);

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
			//����������� ������ 
			EnterCriticalSection(&MyStream.mguard);
			__try
			{
				strcpy_s(MyStream.msg, RecvText);
				MyStream.f_ready = 1;
			}
			__finally
			{
				LeaveCriticalSection(&MyStream.mguard);
			}
		}
		//���� ��� ���������
		//////////////////////////////////////////////////////////////////////////////////
		if (FD_ISSET(ClientSockets, &WriteSet))
		{
			FD_CLR(ClientSockets, &WriteSet);
			
			if (SendText != 0)
				send(ClientSockets, SendText, strlen(SendText), 0);
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
			MyServer.StartChat();
		}
	}
	catch (char *ErrMsg)
	{
		cout << "\nError: " << ErrMsg;
		getchar();//������ ������ ���� ����� (�� ���� system("pause");). ��� ���� � � �������
		throw "Stop";
	}
}

  unsigned int __stdcall StremServer::fCin(void *)
 {
	 AllocConsole();

	 HANDLE myConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);

	 char command[1024];
	 int charsRead = 0;

	 while (1)
	 {
		 ZeroMemory(command, sizeof(command));
		 cout << endl;
		 cin >> command;

		 ZeroMemory(interThr.msg, sizeof(interThr.msg));

		 strcpy_s(interThr.msg, command);

		 interThr.ready = 1;
		 if (strcmp(command, "exit") == 0) 
		 {
			 interThr.ready = -1;
			 break;
		 }
	 }
	 return 0;
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
			 { //���������� ��������� ����������//  __try,__finally - �������� ��������� � ������� ������� ��������� ����������� ������ (�������� Win32app) 
				 cout << endl;
				 cout << "Message: " << msg << endl;
				 f_ready = 0;
			 }
			 __finally//  __catch ���� ���������� �� ��������  __finally ����� ��� ��� ����������� ������ ����� ����� ���������� ��� ����� __try (����� � ��� ��� �� ����� � ����)
			 {
				 LeaveCriticalSection(&mguard);
			 }
		 }
	 
 }