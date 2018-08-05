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
	
	
	#sends argument and prints whats needed to the screen
def send_argument():
	#flush stdin
	tcflush(sys.stdin, TCIFLUSH)
	#check and make sure there are at least 5 arguments
	if len(sys.argv) > 4:
		#filename should be the last argument
		filename = sys.argv[4]
		#what they want to do should be the 4th argument
		input = sys.argv[3] + ' ' + sys.argv[4]
	else:
		#if there arent four arguments, its an ls command
		input = sys.argv[3]
		#send the -l command to server
		the_socket.send(input)
		#get server response
		received = the_socket.recv(100000)
		#print server response (the directory contents)
		print received +"\n"
		return
		#for -g inputs to get a file
	if '-g' in input:
		#double check the arguments list is long enough that they have a filename
		if len(sys.argv) < 5:
			print "no file name"
		else:
			#send the -g and filename to server
			the_socket.send(input)
			#split the filename at the . and continuously add one to the filename uintil it is a valid, non-used filename
			fname = filename.split('.')
			i=1
			while os.path.isfile(filename): 
				#add one to the variable file name to try and make it unique
				filename=fname[0]+str(i)+'.'+fname[1]
				i+=1
			
			#get server response
			buffer = the_socket.recv(1000000)
			#if server sent no such file, tell the client
			if "No such file" == buffer:
				print 'No such file'
			else:
				#otherwise, open or create the file and append text to it.
				with 	open(filename, "a") as myfile:
					#add the received content
					myfile.write(buffer)
					#close the file
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
#connect to the server
the_socket.connect((host,port))

#continuously receive and then send messages

send_argument()

