#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char argv[])
{
	if (argc < 4) {
		printf("[FUNCTION] %s <UID> <FID>\n", argv[0]);
		return -1;
	}

	int sockfd, int UID, int FID, sent_bytes;

	UID = int(argv[1]);

	struct sockaddr_in client_addr, server_addr;

	socklen_t sin_size;
	int recv_length = 1, yes = 1;
	char buffer[1024];

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0) == -1)) {
		printf("[FATAL] error in setting sockfd\n");
		return -1;
	}
	
	server_addr.sin_port = 80;
	server_addr.sin_addr = htonl(argv[3]);

	buffer = strcat("POST ", char(UID));

	if (! connect(sockfd, server_addr, socklen_t)) {
		printf("[FATAL] error connecting to server on ", inet_ntoa(server_addr.sin_addr));
		return -1;
	}

	send_bytes = strlen(buffer);

	if (sent_bytes = send(sockfd, buffer, send_bytes, 0)) {
		printf("[SUCCESS] %d bytes sent");
		return 0;
	}
}
