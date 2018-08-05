README.txt

Robert Hayden Anderson
6/4/2017
CS372
Project2


This is a simple file transfer server.  The python client sends arguments to the C server, and the server responds correctly.
There are a lot of possible bugs, but checking those was not a portion of the assignment. The server is on the local host by default.
The program will not override a file if it already exists, it will create a new file with incrementing numbers after the filename but
before the file type.

EXTRA CREDIT:  The program can send more than just .txt files (tried with .c,.cpp,.java,.pdf,.docx,.doc)

To run:

Start with the server.

Compile the server using:
			gcc server -o server.c

Run the server with a well known port: 
			server 1234

Then run the python client: 	
			python client.py 127.0.0.1 1234 -l
or			python client.py 127.0.0.1 1234 -g <filename>
or			client.py 127.0.0.1 -l
or			client.py 127.0.0.1 -g <filename>