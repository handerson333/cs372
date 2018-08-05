#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void chat_send(char* input, int sock, char* buffer);
void chat_receive(char *buffer,int received,int sock);
void send_directory( char* input, int sock, char* buffer);

int main(int argc, char *argv[])
{
	//make sure all arguments are there
	if (argc != 2) { fprintf(stderr,"USAGE:%s port\n",argv[0]);  exit(0); } // Check usage & args



	int port;

	port = atoi(argv[1]);


	int server_fd, sock, received;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[100000] = {0};
	char *hello = "\nYou have connected to the C Server.\nTo search my directory use: -l.\nTo request a file transfer use: -g <filename>";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
				&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( port );

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, 
				sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Listening for Client ...\n");
	if ((sock = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	memset(buffer,0,strlen(buffer));
	received = read( sock , buffer, 1024);
	printf("%s\n",buffer );
	send(sock , hello , strlen(hello) , 0 );
	char *input = malloc(100000);
	char* temp = malloc(100000);


	input = buffer;
	while(1){
		
		char * tokenized_input = strtok(input, " ");
		char* temp1 = tokenized_input;
		char* temp2 = tokenized_input+3;
		memset(input,0,strlen(input));
		strcpy(input, "");
		input[0] = 0;
		/*
		memset(tokenized_input,0,strlen(tokenized_input));
		strcpy(tokenized_input, "");
		tokenized_input[0] = 0;
		
		memset(temp1,0,strlen(temp1));
		strcpy(temp1, "");
		temp1[0] = 0;
		
		memset(temp2,0,strlen(temp2));
		strcpy(temp2, "");
		temp2[0] = 0;
		*/
		
		
		//get input from server
		fflush(stdin);
		fflush(stdout);
		received = read( sock , input, 100000);
		
		
		printf("arg1 = %s, arg2 = %s\n",tokenized_input,tokenized_input+3);
		//temp = strtok(input, " ");
		//while(temp != NULL) {
		//	printf("%s\n", temp);
		//	temp = strtok(NULL, " ");
		//}
		//printf("first tokem = %s",temp[0]);
		//memset(temp1,0,strlen(input));
		//memset(temp2,0,strlen(temp2));
		
		if(!strcmp(temp1,"-l")){
			temp = "used -l option\n";
			printf("client %s",temp);
			send(sock , temp , strlen(temp) - 1 , 0 );
			
			//Pulled from my small shell program from cs344
			char buff[255];
			if(getcwd(buff,255) != NULL){
			      printf("Current Working Directory is %s\n",buff);
			      fflush(stdout);
			}
			//Credit to https://stackoverflow.com/questions/28507950/calling-ls-with-execv
			int status;
			char *arg[2];
			pid_t pid = fork();
			arg[0] = "/bin/ls";        // first arg is the full path to the executable
			arg[1] = NULL;             // list of args must be NULL terminated
			if (pid == 0 ){
				
					dup2(sock, STDOUT_FILENO);		
					execvp( arg[0], arg); // child: call execv with the path and the args
			}	
			else
				wait( &status ); 
			//fscanf(stdout,"%s");
			//send_directory(input,sock,buffer);
		}
		else if(!strcmp(temp1,"-g")){
			temp = "used -g option\n";
			printf("client %s",temp);
			send(sock , temp , strlen(temp) - 1 , 0 );
			
			if(access(temp2,F_OK) != -1){
				printf("file exists!\n");
			}else{
				//error
				printf("No such file\n");
			}
			
			
		}
		else {
			temp = "that was not an option\n";
			printf("client tried something %s",temp);
			send(sock , temp , strlen(temp) - 1 , 0 );
			//printf("not an option\n");
			//send_directory(input,sock,buffer);
		}


		//chat_receive(buffer,received,sock);
		//chat_send(input,sock,buffer);

	}

	return 0;




	/*
	//declare variables
	int port,received,connection;
	long sock =0;
	int opt = 1;
	char *input = malloc(100000);
	char *handle = malloc(100000);
	//port equals second argument


	//printf("port = %d", port);

	//get hostname
	struct hostent* host;
	host = gethostbyname(argv[1]);

	//tokenize the handle
	strtok(handle, "\n");


	//make socket struct
	struct sockaddr_in server_address;
	int addrlen = sizeof(server_address);


	//zero out buffer
	char buffer[100000] = {0};

	//create socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
	printf("\n Socket creation error \n");
	return -1;
	}
	//set socket
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
	printf("\n Socket creation error \n");
	return -1;
	}


	memset(&server_address, '0', sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));


	//listen
	printf("listening ...\n");
	listen(sock, 1);

	//accept
	connection = accept(sock, (struct sockaddr *)&server_address, (socklen_t*)&addrlen);

	 */


	//send and receive until end of time

}
void send_directory( char* input, int sock, char* buffer){
	while(1){


		//flush out stdout for safety
		fflush(stdout);

		//continously show the handle> while null
		if(fgets(input,100000,stdin) == NULL){
			continue;
		}

		//zero out buffer
		memset(buffer,0,strlen(buffer));


		//add '>' to buffer with handle
		//strcat(buffer,"> ");

		//add the actual input to the buffer after that
		strcat(buffer, input);

		//send the whole handle> input minus the \n
		send(sock , buffer , strlen(buffer) - 1 , 0 );

		//temporary check for input if it's \quit
		char quit_it[] = "\\quit\n";
		if(!strcmp(input, quit_it)){

			//if it's \quit then exit with 0 
			printf("session has ended\n");
			exit(0);
		}

		break;
	}
}

void chat_send( char* input, int sock, char* buffer){
	while(1){


		//flush out stdout for safety
		fflush(stdout);

		//continously show the handle> while null
		if(fgets(input,100000,stdin) == NULL){
			continue;
		}

		//zero out buffer
		memset(buffer,0,strlen(buffer));


		//add '>' to buffer with handle
		//strcat(buffer,"> ");

		//add the actual input to the buffer after that
		strcat(buffer, input);

		//send the whole handle> input minus the \n
		send(sock , buffer , strlen(buffer) - 1 , 0 );

		//temporary check for input if it's \quit
		char quit_it[] = "\\quit\n";
		if(!strcmp(input, quit_it)){

			//if it's \quit then exit with 0 
			printf("session has ended\n");
			exit(0);
		}

		break;
	}
}



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
