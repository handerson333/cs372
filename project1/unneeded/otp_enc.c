#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues
char* readfile(char*);


int main(long argc, char *argv[])
{

	long socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[100000];
	char handle[255];
	char text[255];
	//check it has all CLA's
	if (argc < 2) { fprintf(stderr,"USAGE: port\n", argv[0]); exit(0); } // Check usage & args

	
	long int port = strtol(argv[1], &buffer, 10);
	printf("port: %d\n What is your handle?\n", port);
	scanf("%s", &handle);
	printf("The port is %d your handle is %s.", port, handle);
	printf("What do you want to say?\n");
	
	
	
	scanf("%s", &text);


	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address


	//check for host
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address
	printf("connected\n");


	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) error("CLIENT: ERROR opening socket");

	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		error("CLIENT: ERROR connecting");




	// Send plaintext to daemon
	charsWritten = send(socketFD, text, strlen(text), 0); // Write to the server
	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
	if (charsWritten < strlen(text)) printf("CLIENT: WARNING: Not all data written to socket!\n");


	// Get return message confirmation from daemon
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
	charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
	if (charsRead < 0) error("CLIENT: ERROR reading from socket");



	//print the encrypted text to stdout
	fprintf(stdout, "%s\n", buffer);



	close(socketFD); // Close the socket
	return 0;
}



char *readfile(char* textfile){

	FILE *fp;
	char *text = malloc(200000);
	fp=fopen(textfile,"r");
	//read file until you get to newline
	fscanf(fp, "%[^\n]", text);
	fclose(fp);
	return text;

}





