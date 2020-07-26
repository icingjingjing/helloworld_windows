// testSocket.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	char bufSend[128] = {0};
	char bufRcv[128] = {0};

	WSADATA wsaDATA;
	if(WSAStartup(WINSOCK_VERSION, &wsaDATA) != 0)
	{
		printf("WSAStartup fail\r\n");
	}

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockClient == INVALID_SOCKET)
	{
		printf("create socket fail\r\n");
	}

	sockaddr_in addrServer;
	memset(&addrServer, 0, sizeof(addrServer));
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(1234);
	addrServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.104");
	while(true)
	{
		if(connect(sockClient, (sockaddr*)&addrServer, sizeof(sockaddr)) != 0)
		{
			printf("connect fail\r\n");
			Sleep(1);
			continue;
		}
	
		printf("client connected\r\n");
		while(true)
		{
			scanf("%s", bufSend);
			if(send(sockClient, bufSend, sizeof(bufSend), 0) <= 0)
			{
				printf("send fail\r\n");
			}

			if(recv(sockClient, bufRcv, sizeof(bufRcv), 0) <= 0)
			{
				printf("recv fail\r\n");
			}

			
			if(strcmp(bufRcv, "q")==0)
			{
				printf("shutting down\r\n");
				if(shutdown(sockClient, 1) != 0)
				{
					printf("shutdown fail\r\n");
				}
				sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				break;
			}
			else
				printf(bufRcv);
		}
	}

	

	WSACleanup();
	getchar();
	return 0;
}

