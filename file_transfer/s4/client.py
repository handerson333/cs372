#!/usr/bin/python
import sys
import string
import os
import csv
import numpy as np
import random
import socket
from termios import tcflush, TCIFLUSH



def chat_receive():
	#get input from already made socket
	received = the_socket.recv(100000)
	#if received input has '\quit' in it, then exit
	if (received.find("\\quit") > 0):
		print "Client has ended the session"
		#close the socket
		the_socket.close()
		sys.exit(0)
	#print the received text to the screen
	print received +"\n"
	

	
def chat_send():
	#flush stdin
	tcflush(sys.stdin, TCIFLUSH)
	#get the input to send
	input = raw_input("> ")
	#if the input has '\quit' then exit
	if input == '\\quit':
		print  " has ended the session\n"
		#send the closing signal to the client
		the_socket.send(input)
		#close the socket
		the_socket.close()
		sys.exit(0)
	#otherwise send the message to the client
	the_socket.send(input)
	if input == '-l':
		received = the_socket.recv(100000)
		print received +"\n"
	
	

###actual start of program###

#create a socket object
the_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  

#get the local host name       
host = '127.0.0.1' 


#if there's not 2 cla arguments (program name and port number)
if len(sys.argv) != 2 :
	#then show the syntax
	print "syntax is: ./server <port number> <host name>"
	#exit with error
	sys.exit(2)

#make the port the cla	
port = int(sys.argv[1])


#make sure port is not a well known/used port
if port < 1024:
	print "That port is a commonly used system port, please select one above 1024"
	sys.exit(2)

the_socket.connect((host,port))
the_socket.send("Python client connected!")
chat_receive()
#continuously receive and then send messages
while True:
	
	chat_send()
	chat_receive()
	
	
	



