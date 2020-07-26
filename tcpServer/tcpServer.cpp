// tcpServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	char bufSend[128] = {0};
	char bufRcv[128] = {0};
	//初始化WSA
	WSADATA wsaData;
	int nRe = WSAStartup(WINSOCK_VERSION, &wsaData);
	if(nRe != 0)
	{
		printf("WSAStartup Fail\r\n");
	}
	//创建socket
	SOCKET sockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockListen == INVALID_SOCKET)
	{
		printf("create socket fail\r\n");
	}
	//绑定端口
	sockaddr_in addrListen, addrClient;
	memset(&addrListen, 0, sizeof(addrListen));
	addrListen.sin_family = AF_INET;
	addrListen.sin_port = htons(1234);
	addrListen.sin_addr.S_un.S_addr = inet_addr("192.168.1.104");
	
	if(bind(sockListen, (sockaddr*)&addrListen, sizeof(addrListen)) != 0)
	{
		printf("socket bind fail\r\n");
	}
	//监听
	if(listen(sockListen, SOMAXCONN) != 0)
	{
		printf("socket listen fail\r\n");
	}

	printf("server started\r\n");
	for(;;)
	{
		//接受客户的连接请求
		int nAddrLen = sizeof(addrClient);
		memset(&addrClient, 0, nAddrLen);
		SOCKET sockConn = accept(sockListen, (sockaddr*)&addrClient, &nAddrLen );
		if(sockConn == INVALID_SOCKET)
		{
			Sleep(1);
			continue;
		}
		printf("accept\r\n");

		while(true)
		{
			if(recv(sockConn, bufRcv, sizeof(bufRcv), 0) <= 0)
			{
				printf("recv fail\r\n");
				closesocket(sockConn);
				break;
			}

			printf("%s\r\n", bufRcv);
			if(strcmp(bufRcv, "q") == 0)
				strcpy(bufSend, "q");
			else
				strcpy(bufSend, "hello client\r\n");

			if(send(sockConn, bufSend, sizeof(bufSend), 0) <= 0)
			{
				printf("send fail \r\n");
				closesocket(sockConn);
				break;
			}

			if(strcmp(bufRcv, "q") == 0)
			{
				closesocket(sockConn);
				break;
			}
		}
	}

	closesocket(sockListen);

	WSACleanup();

	getchar();
	return 0;
}

