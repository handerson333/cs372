#!/usr/bin/python
import sys
import string
import os
import csv
import numpy as np
import random
from socket import *

def chat_receive():
	#get input from already made socket
	received = c.recv(100000)
	#if received input has '\quit' in it, then exit
	if (received.find("\\quit") > 0):
		print "Client has ended the session"
		#close the socket
		c.close()
		sys.exit(0)
	#print the received text to the screen
	print received +"\n"
	
def chat_send():
	#get the input to send
	input = raw_input(handle + "> ")
	#if the input has '\quit' then exit
	if input == '\\quit':
		print handle + " has ended the session\n"
		#send the closing signal to the client
		c.send(input)
		#close the socket
		c.close()
		sys.exit(0)
	#otherwise send the message to the client
	c.send(handle + "> " + input)
	
###actual start of program###

#create a socket object
the_socket = socket()  

#get the local host name       
host = '' 

#if there's not 2 cla arguments (program name and port number)
if len(sys.argv) != 2 :
	#then show the syntax
	print "syntax is: ./server <port number>"
	#exit with error
	sys.exit(2)
	
#make the port the cla	
port = int(sys.argv[1])

#make sure port is not a well known/used port
if port < 1024:
	print "That port is a commonly used system port, please select one above 1024"
	sys.exit(2)
	
#get users handle for chat session
handle = raw_input("What's your chat handle?\n")

#bind socket to the port
the_socket.bind((host, port))      

#wait for client connection
the_socket.listen(5)                 
print "\nwaiting for client\n"

#create connection with client and get the address
c, addr = the_socket.accept()   
print 'Got connection from', addr

#continuously receive and then send messages
while True:
	chat_receive()
	chat_send()
	
	
	
	



