#Scott Spitler 05/20/2017
#why did I not use this file before

#this makefile is broken
#to compile server run gcc server.c linkedlist.c handler.c socks.c -o server
#to compile client run gcc client.c linkedlist.c socks.c -o client
#needs to be fixed in next update

OBJS=server.o client.o
CC=cc
CFLAGS=-I.r-Wall 
//DEPS= func_a.h

all:
	gcc server.c linkedList.c handler.c socks.c -o ./bin/server
	gcc client.c linkedList.c socks.c -o ./bin/client

cleanall:
	rm ./bin/*
