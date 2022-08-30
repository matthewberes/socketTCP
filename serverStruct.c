//Lab 4: TCP Socket File Transfer
//Matt Beres 0640800 April 6 2022
//Sending a file name from client to server and copying all the data from the file from the client onto the server
//Called from command line, no arguments
//No functions, everything is handled inside of main

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main()
{
	char *ip = "127.0.0.1";
	int port = 50800;

	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_size;
	char buffer[1024];
	int n;

	//Creating TCP socket
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0)
	{
		perror("[-] Socket error");
		exit(1);
	}
	printf("[+] TCP server socket created.\n");

	//Connecting to port and IP address
	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

	//Binding
	n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (n < 0)
	{
		perror("[-] Bind error");
		exit(1);
	}
	printf("[+] Binded to the port number: %d\n", port);

	listen(server_sock, 5);
	printf("Listening...\n");

	while(1)
	{
		//Creating socket for client to connect to
		addr_size = sizeof(client_addr);
		client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
		printf("[+] Client connected.\n");

		//Receiving file name from client
		bzero(buffer, 1024);
		recv(client_sock, buffer, sizeof(buffer), 0);
		printf("Argument from client: %s\n", buffer);

		//Taking file name and creating a name to copy to: "copy.FILENAME.EXT"
		char copy[6] = "copy.";
		char fileName[100];
		strcat(copy, buffer);
		strcpy(fileName, copy);

		//Creating simple name "copy.txt", other way did not work when using files from other directories
		char tempName[10] = "copy.txt";

		printf("File name: %s\n", tempName);

		//Sending message back to client to prepare it to send the file data
		bzero(buffer, 1024);
		strcpy(buffer, "Ready for file data\n");
		send(client_sock, buffer, strlen(buffer), 0);

		//Creating file to copy the data to
		int file;
		file = open(tempName, O_CREAT | O_RDWR, 0777);

		//Creating struct for file data
		struct fileData
		{
		int fileSize;
		char inputData[1024];
		};

		struct fileData fBuffer;

		//Recieving the buffers of file data
		int writeStop;
		while(writeStop < 1)
		{
			//bzero(buffer, 1024);
			recv(client_sock, &fBuffer, sizeof(buffer), 0);
			write(file, &fBuffer.inputData, fBuffer.fileSize);
			bzero(buffer, 1024);
			
			if(fBuffer.fileSize < 1024)
			{
				writeStop++;
			}
		}
		printf("File transferred. Goodbye.\n");
	}
}
