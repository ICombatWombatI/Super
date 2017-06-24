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
	if (listening != INVALID_SOCKET)
		closesocket(listening);
	if (ClientSocket != INVALID_SOCKET)
		closesocket(listening);
}


void WSockServer::SetServerSockAddr(sockaddr_in *hint, int PortNumber)
{
	hint->sin_family = AF_INET;
	hint->sin_port = htons(PortNumber);
	hint->sin_addr.S_un.S_addr = INADDR_ANY;			
}

bool WSockServer::RunServer(int PortNumber)
{

	if ((listening = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)	
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
			else if (bytesReceived == 0)
			{
				cout << "The client closed the connection. " << endl;
			}
			else
			{
				EnterCriticalSection(&MyStream.mguard);
				__try
				{
					Buffer[bytesReceived] = 0;
					strcpy_s(MyStream.msg, Buffer);
					MyStream.f_ready = 1;
				}
				__finally
				{
					LeaveCriticalSection(&MyStream.mguard);
				}
			}

			//cout << string(Buffer, bytesReceived) << endl;

			//send(ClientSocket, Buffer, bytesReceived + 1,0);// отправляем данные назад к клиенту (если нужно)
	}
}



 unsigned int __stdcall StremServer::server_f(void *args)// _beginthreadex принемает только функции некоторого типа(__stdcall), поэтому тут и есть этот тип
{
	try
	{
		WSockServer MyServer(REQ_WINSOCK_VER);
		if (MyServer.RunServer(54000))
		{
			cout << "Client connected. " << endl;
			MyServer.StartChat();
		}
	}
	catch (char *ErrMsg)
	{
		cout << "\nError: " << ErrMsg;
		getchar();//Фунция просто ждет смвол (по типу system("pause");). Она есть есть и в Линукс
		//f_stop = 1;
		throw "Stop";
		return 1;//выход из функции и завершение потока
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
			 { //структурна обработка исключений//  __try,__finally - механизм встроеный в виндовс который позволяет отлавливать ошибки (стандарт Win32app) 
				 cout << "Message: " << msg << endl;
				 f_ready = 0;
			 }
			 __finally//  __catch тоже существует но механизм  __finally хорош тем что выполноется только тогда когда выполнется код после __try (можно и без них но лучше с ними)
			 {
				 LeaveCriticalSection(&mguard);
			 }
		 }
	 
 }