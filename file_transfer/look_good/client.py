#!/usr/bin/python

######    R. Hayden Anderson    ######
######         6/3/2017         ######
######   File Transfer Client   ######
######     CS372 Networks       ######
######        Project 2         ######


import sys
import string
import os
import csv
import numpy as np
import random
import socket
from termios import tcflush, TCIFLUSH
	
def client_request():
	#flush stdin
	tcflush(sys.stdin, TCIFLUSH)
	#get the input to send
	if len(sys.argv) > 4:
		filename = sys.argv[4]
		input = sys.argv[3] + ' ' + sys.argv[4]
	else:
		input = sys.argv[3]
		the_socket.send(input)
		received = the_socket.recv(100000)
		print received +"\n"
		return
	if '-g' in input:
	
		if len(sys.argv) < 5:
			print "no file name"
		else:
			the_socket.send(input)
			fname = filename.split('.')
			i=1
			while os.path.isfile(filename): 
				
				filename=fname[0]+str(i)+'.'+fname[1]
				i+=1
			
			
			buffer = the_socket.recv(1000000)
			if "No such file" == buffer:
				print 'No such file'
			else:
				with 	open(filename, "a") as myfile:
					myfile.write(buffer)
					myfile.close()
		
		
	

###actual start of program###

#create a socket object
the_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  

#get the local host name       
host = sys.argv[1]


#if there's not 2 cla arguments (program name and port number)
if len(sys.argv) < 3 :
	#then show the syntax
	print "syntax is: ./server <port number> <host name>"
	#exit with error
	sys.exit(2)

#make the port the cla	
port = int(sys.argv[2])


#make sure port is not a well known/used port
if port < 1024:
	print "That port is a commonly used system port, please select one above 1024"
	sys.exit(2)

the_socket.connect((host,port))

#continuously receive and then send messages

	
client_request()

