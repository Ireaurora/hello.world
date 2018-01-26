#define WIN32_LEAN_AND_MEAN

//#include <stdafx.h>
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_PORT "1863"

int __cdecl main(int argc, char argv[])
{
	if (argc < 4) {
		printf("[FUNCTION] <UID> <FUID> <DATA>");
		return -1;
	}

	WSADATA wsaData;

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	char sendbuf[2048];

	strcpy(sendbuf, (char *)argv[2]);

	strcat(sendbuf, " ");
	strcat(sendbuf, (char *)argv[3]);
	strcat(sendbuf, " ");
	strcat(sendbuf, (char *)argv[4]);

	getaddrinfo(&argv[1], DEFAULT_PORT, &hints, &result);

	SOCKET ConnectSocket;

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		break;
	}
	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("[ERROR] unable to connect");
		return -1;
	}
	if (!send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0)) {
		printf("[ERROR] in send()");
		closesocket(ConnectSocket);
		WSACleanup();
		return -1;
	}

	if (!shutdown(ConnectSocket, SD_SEND)) {
		printf("[ERROR] shutting down connection");
		closesocket(ConnectSocket);
		WSACleanup();
		return -1;
	}
	closesocket(ConnectSocket);
	WSACleanup();
}