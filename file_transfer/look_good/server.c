/*
######    R. Hayden Anderson    ######
######         6/3/2017         ######
######   File Transfer Server   ######
######     CS372 Networks       ######
######        Project 2         ######
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void chat_send(char* input, int sock, char* buffer);
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
	
	while(1){
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
	//received = read( sock , buffer, 1024);
	printf("%s\n",buffer );
	//send(sock , hello , strlen(hello) , 0 );
	char *input = malloc(100000);
	char* temp = malloc(100000);


	input = buffer;
		printf("memset \n");
		memset(input,'\0',strlen(input));

		//get input from server

		fflush(stdin);
		fflush(stdout);

		received = read( sock , input, 100000);

		char * tokenized_input = strtok(input, " ");


		char* temp1 = tokenized_input;
		char* temp2 = strtok(input, " ");
		if(!strcmp(temp1,"-l")){
			temp = "used -l option\n";
			printf("client %s",temp);
			//send(sock , temp , strlen(temp) - 1 , 0 );
			
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
			fscanf(stdout,"%s");
			
			
			
			shutdown(sock,SHUT_WR );
			
		}
		else if(!strcmp(temp1,"-g")){
			
			
			
			temp = "used -g option\n";
			printf("client %s",temp);
			//send(sock , temp , strlen(temp) - 1 , 0 );
			char buf1[1024];
			if(access(temp2,F_OK) != -1){
				long total_length =0,i =0;
				FILE *fp;
				
				fp=fopen(temp2, "r+");

				//fscanf(fp, "%100s", buf1);
				fseek(fp,0L,SEEK_END);
				total_length = ftell(fp);
				rewind(fp);
				printf("total_length = %i\n",total_length);
				int times = total_length / 1024;
				int remainder = total_length % 1024;
				//send(sock,(char*)total_length,sizeof(total_length),0);
				for( i=0; i < times ; i++){

					fread(buf1, 1024, 1, fp);
					send(sock, buf1, 1024,0);
				}
				
				if(remainder > 0 ){
					
					fread(buf1,remainder,1,fp);
					send(sock,buf1,remainder,0);
					
				}

				fclose(fp);
				shutdown(sock,SHUT_WR );

				
			}else{
				//error
				temp = "No such file";
				printf("No such file\n");
				send(sock , temp , strlen(temp), 0 );
			}
			
			
		}
		else {
			temp = "that was not an option\n";
			printf("client tried something %s",temp);
			send(sock , temp , strlen(temp) - 1 , 0 );

		}




	}

	return 0;




	

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