
#include "Server.h"
#include <process.h>

#include <iostream>

using namespace std;

StremServer MyStream;

interThread interThr;

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
