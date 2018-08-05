Robert Hayden Anderson
CS372 project 1
chat server and chat client


to run:

Start off with the python server using the command:
server <port #>
make sure the port number is above 1024 to stay away from well known ports
enter the handle you want as the server

in a separate terminal, compile the c client with the command:
gcc -o client client.c

then run it with the command:
./client <hostname> <port>

my server is set at the local host so use "localhost" to connect to it and make sure to use the same port
as what you chose with the python server

enter your user handle for the client as whatever you want. 

Then start off the session by writing a message in the client window and press enter.
To respond, go to the server's window and type whatever you want.

to exit at either time use "\quit" in the terminal awaiting the chat input.
