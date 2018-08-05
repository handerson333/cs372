/*
######    R. Hayden Anderson    ######
######         6/3/2017         ######
######   File Transfer Server   ######
######     CS372 Networks       ######
######        Project 2         ######
*/

/*
EXTRA CREDIT
This program is able to send files of any type (tried with docx and pdf)

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


	//declare port
	int port;
	//make port the cla and convert the cla to an int
	port = atoi(argv[1]);

	//declare variables for sockets
	int server_fd, sock, received;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[100000] = {0};

	//create socket 
	//credit to http://www.geeksforgeeks.org/socket-programming-cc/
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//make the socket reussable after by setting a socket option
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
				&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	//make sure theres an address and set to AF Inet
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( port );
	//make sure port binds or else print error
	if (bind(server_fd, (struct sockaddr *)&address, 
				sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	//start of while loop to for server to get commands from client
	while(1){
		//listen for a client
		if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Listening for Client ...\n");
	//accept client or else print error
	if ((sock = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	//received = read( sock , buffer, 1024);
	
	printf("%s\n",buffer );
	//allocate for input and a buffer
	char *input = malloc(100000);
	char* temp = malloc(100000);

	//save input as a temp variable
	input = buffer;
		//printf("memset \n");
		//zero out input
		memset(input,'\0',strlen(input));
		
		//get input from server
		//flush stdin and out for protection
		fflush(stdin);
		fflush(stdout);
		//wait for input from server for a command
		received = read( sock , input, 100000);
		//tokenize the input
		char * tokenized_input = strtok(input, " ");
		//memset(tokenized_input,'\0',strlen(tokenized_input));

		char* temp1 = tokenized_input;
		//memset(temp1,'\0',strlen(temp1));
		//char* temp2 = strtok(input, " ");
		char* temp2 = tokenized_input +3;
		//memset(temp2,'\0',strlen(temp2));
		//printf("temp1 = %s , temp2 = %s, tokenized_input = %s, input = %s\n",temp1,temp2,tokenized_input,input);
		
		
		//if colient wants to see whats in the directory.
		if(!strcmp(temp1,"-l")){
			temp = "used -l option\n";
			printf("client %s",temp);
			//send(sock , temp , strlen(temp) - 1 , 0 );
			
			//Pulled from my small shell program from cs344
			//char buff[255];
			//if(getcwd(buff,255) != NULL){
			  //    printf("Current Working Directory is %s\n",buff);
			 //     fflush(stdout);
			//}
			
			//Credit to https://stackoverflow.com/questions/28507950/calling-ls-with-execv
			//declare variables
			int status;
			char *arg[2];
			//fork to parent and child
			pid_t pid = fork();
			//access the "ls" command
			arg[0] = "/bin/ls";        // first arg is the full path to the executable
			arg[1] = NULL;             // list of args must be NULL terminated
			//if child
			if (pid == 0 ){
					//redirect stdout to the current socket
					dup2(sock, STDOUT_FILENO);		
					//execute ls
					execvp( arg[0], arg); // child: call execv with the path and the args
			}	
			else{//parent process
				//wait for child process to finish
				wait( &status ); 
			}
			//scan stdout
			fscanf(stdout,"%s");
			//reset all variables possibly used
			memset(input,'\0',strlen(input));
			memset(tokenized_input,'\0',strlen(tokenized_input));
			memset(temp1,'\0',strlen(temp1));
			memset(temp2,'\0',strlen(temp2));
			//close the cokset using shutdown to wait for the send just in case
			shutdown(sock,SHUT_WR );
			
		}
		//if client wants a file
		else if(!strcmp(temp1,"-g")){
			
			temp = "used -g option\n";
			printf("client %s",temp);
			//send(sock , temp , strlen(temp) - 1 , 0 );
			char buf1[1024];
			//check if the file given exists
			if(access(temp2,F_OK) != -1){
				long total_length =0,i =0;
				//create filepointer
				FILE *fp;
				//open file and read
				fp=fopen(temp2, "r+");

				//fscanf(fp, "%100s", buf1);
				//seek the end of the file
				fseek(fp,0L,SEEK_END);
				//get the length of the file
				total_length = ftell(fp);
				//point back to the beginning of the file
				rewind(fp);
				printf("total_length of file = %i\n",total_length);
				//count hoiw many chunks we will need to send
				int times = total_length / 1024;
				//add a remainder if its not exactly divisible by 1024
				int remainder = total_length % 1024;
				//send(sock,(char*)total_length,sizeof(total_length),0);
				//send until you reach the amopunt of total chunks
				for( i=0; i < times ; i++){
					//read 1KB of the file
					fread(buf1, 1024, 1, fp);
					//send that KB of the file
					send(sock, buf1, 1024,0);
				}
				//send the remainder
				if(remainder > 0 ){
					//read the last bit
					fread(buf1,remainder,1,fp);
					//send the last bit
					send(sock,buf1,remainder,0);
					
				}
				
				//reset all possible variables used
				memset(input,'\0',strlen(input));
				memset(tokenized_input,'\0',strlen(tokenized_input));
				memset(temp1,'\0',strlen(temp1));
				memset(temp2,'\0',strlen(temp2));
				//close the file
				fclose(fp);
				//close the socket connection with shutdown so they can finish talking
				shutdown(sock,SHUT_WR );

				
			}else{
				//error
				//otherwise theres no file
				temp = "No such file";
				printf("No such file\n");
				//tell client there was not a file
				send(sock , temp , strlen(temp), 0 );
			}
			
			
		}
		else {
			//if they used something that wasn't an option at all
			temp = "that was not an option\n";
			printf("client tried something %s",temp);
			//tell them that's not an option
			send(sock , temp , strlen(temp) - 1 , 0 );

		}




	}

	return 0;	

}

//send the directory
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
//send a message that's not a file
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