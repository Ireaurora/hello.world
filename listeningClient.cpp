#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
//#include <stdafx.h>
#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "1863"


int recvSock(SOCKET ListenSocket);

int __cdecl main(void) 
{
	WSADATA wsaData;

	struct addrinfo *result = NULL, client;
	
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}


	ZeroMemory(&client, sizeof(client));
	client.ai_family = AF_INET;
	client.ai_socktype = SOCK_STREAM;
	client.ai_protocol = IPPROTO_TCP;
	client.ai_flags = AI_PASSIVE;

	if ((iResult = getaddrinfo(NULL, DEFAULT_PORT, &client, &result)) != 0) {
		printf("[ERROR] getaddrinfo failed");
		WSACleanup();
		return -1;
	}

	SOCKET ListenSocket;

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
		printf("[ERROR] at socket");
		WSACleanup();
		return -1;
	}

	if (bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		printf("[ERROR] binding socket");
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}
	freeaddrinfo(result);
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("[ERROR] in listening socket");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	recvSock(ListenSocket);
}

int recvSock(SOCKET ListenSocket)
{
	SOCKET serverSocket;
	char recvbuf[8];
	__int32 recvbuflen, recvBytes;

	recvbuflen = 8;

	if ((serverSocket = accept(ListenSocket, NULL, NULL)) == INVALID_SOCKET) {
		printf("[ERROR] in serverSocket");
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}
	
	do {
		if ((recvBytes = recv(serverSocket, recvbuf, recvbuflen, 0)) > 0) {
			std::cout << recvbuf;
		}
		else {
			closesocket(serverSocket);
			WSACleanup();
			return 1;
		}
	} while (recvBytes > 0);
	if (shutdown(serverSocket, SD_SEND) == SOCKET_ERROR) {
		printf("[ERROR]shutdown failed");
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}