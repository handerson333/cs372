#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void chat_send(char *handle, char* input, int sock, char* buffer);
void chat_receive(char *buffer,int received,int sock);



int main(int argc, char *argv[]){
    //check argument count
	if (argc != 2) { fprintf(stderr,"USAGE: port\n", argv[0]); exit(0); }
    //declare variables
	int port,received;
	long sock =0;
	char *input = malloc(100000);
	char *handle = malloc(100000);
	//port equals second argument
	port = atoi(argv[2]);
  
    //get hostname
  
    //make socket struct
  
    //make socket
    
    //make port
  
    //check port is above 1024 and not 30020 or 30021
  
    //bind socket to port
  
    //listen over the socket for client
  
    //continuously receive messages
  
    
  
  
  
  
}
//receive messages
    
    //get input
    
    //check if input == ls or
    
    //elseif input == request file
	//check for file in current directory
	    //send file(filename)
    
    //else send message of bad command



//send to client
    //get type of send (-1 for error, 0 for ls, 1 for file)
    
    //if -1 send error message
    
    //if 0 getdir and send
    
    //if 1 getfile and send



int main(int argc, char *argv[])
{
	//make sure all arguments are there
	if (argc != 3) { fprintf(stderr,"USAGE: hostname port\n", argv[0]); exit(0); } // Check usage & args
	
	//declare variables
	int port,received;
	long sock =0;
	char *input = malloc(100000);
	char *handle = malloc(100000);
	//port equals second argument
	port = atoi(argv[2]);
	
	//get clients handle for session
	printf("\nWhat is your handle?\n");
	fgets(handle, 100000, stdin);
	printf("\n");
	
	//get hostname
	struct hostent* host;
	host = gethostbyname(argv[1]);
	
	//tokenize the handle
	strtok(handle, "\n");
	
	
	//make socket struct
    struct sockaddr_in server_address;
	
	//zero out buffer
    char buffer[100000] = {0};
	
	//create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&server_address, '0', sizeof(server_address));
  
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
	
    //credit to http://www.geeksforgeeks.org/socket-programming-cc/ for a lot of this connection
    
	// Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
	//connect with socket and address
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
		//if error fail with 2
        printf("\nConnection Failed \n");
        exit(2);
    }
	
	//send and receive until end of time
	while(1){
		
		chat_send(handle,input,sock,buffer);
		chat_receive(buffer,received,sock);		
	}

	return 0;
}


void chat_send(char *handle, char* input, int sock, char* buffer){
	while(1){
		
		//show handle
		printf("%s> ",handle);
		
		//flush out stdout for safety
		fflush(stdout);
		
		//continously show the handle> while null
		if(fgets(input,100000,stdin) == NULL){
			continue;
		}
		
		//zero out buffer
		memset(buffer,0,strlen(buffer));
		
		//put handle into buffer
		strcat(buffer,handle);
		
		//add '>' to buffer with handle
		strcat(buffer,"> ");
		
		//add the actual input to the buffer after that
		strcat(buffer, input);
		
		//send the whole handle> input minus the \n
		send(sock , buffer , strlen(buffer) - 1 , 0 );
		
		//temporary check for input if it's \quit
		char quit_it[] = "\\quit\n";
		if(!strcmp(input, quit_it)){
			
			//if it's \quit then exit with 0 
			printf("%s has ended the session\n", handle);
			exit(0);
		}
		
		break;
	}
}

### LS ###



### SEND FILE ###



### BAD COMMAND ###
void chat_receive(char *buffer,int received,int sock){
	
	//zero out buffer
	memset(buffer,0,strlen(buffer));
	
	//get input from server
	received = read( sock , buffer, 100000);
	
	//check if input from server is \quit
	if(!strcmp(buffer,"\\quit")){
		//if it is, then tell user and exit with 0
		printf("Server has ended the session\n");
		exit(0);
	}
	
	//otherwise, print the message
	printf("%s\n\n",buffer );
}
