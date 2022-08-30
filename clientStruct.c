//Lab 4: TCP Socket File Transfer
//Matt Beres 0640800 April 6 2022
//Sending a file name from client to server and copying all the data from the file from the client onto the server
//Called from command line, 1 argument
//Argument needed, it is the file you desire to copy
//No functions, everything is handled inside of main

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	char *ip = "127.0.0.1";
	int port = 50800;

	int sock;
	struct sockaddr_in addr;
	socklen_t addr_size;
	char buffer[1024];
	char null[1];
	int n;

	//Creating TCP socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("[-] Socket error");
		exit(1);
	}
	printf("[+] TCP server socket created.\n");

	//Connecting to port and IP address
	memset(&addr, '\0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr(ip);

	//Connecting to server socket
	connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	printf("Connected to server\n");

	//Sending the filename to the server, which is the command line argument
	bzero(buffer, 1024);
	strcpy(buffer, argv[1]);
	//printf("Client printing argument: %s\n", argv[1]);
	send(sock, buffer, strlen(buffer), 0);

	//Receiving message from server to send file data
	bzero(buffer, 1024);
	recv(sock, buffer, sizeof(buffer),0);
	printf("Server: %s\n", buffer);

	//Opening file for read
	int file;
	file = open(argv[1], O_APPEND);
	if (file == -1)
	{
		perror("[-] Error opening 2");
	}

	//Creating struct for file data
	struct fileData
	{
		int fileSize;
		char inputData[1024];
	};

	struct fileData theFile;

	//sending all buffers
	ssize_t byteCount;
	while((byteCount = read(file, buffer, 1024)) > 0)
	{
		theFile.fileSize = byteCount;
		strcpy(theFile.inputData, buffer);
		printf("Sending buffer...\n");
		send(sock, &theFile, sizeof(buffer), 0);
		bzero(buffer, 1024);		
	}
	printf("File transferred. Goodbye.\n");
}