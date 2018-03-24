/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mycommands.h
 * Author: Admin
 *
 * Created on March 3, 2017, 10:51 AM
 */



#include <sys/types.h> //for dir
#include <dirent.h> //for dir
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h> 
#include <ctype.h>

#ifndef MYCOMMANDS_H
#define MYCOMMANDS_H

//constraints for our buffer, up here for easy modification
#define maxbuffa 60


//important values for our program,
#define MAX_SIZE  1000


///stuck with static variables, I use them across my entire program                                               
extern char input;
//our reader info
extern char buffer[MAX_SIZE];                                       
extern  int bChars;

//extern char *commandArg[100];
extern int cmdcount;
extern char *path;
extern int status;
extern int statis;

extern pid_t parent;

extern pid_t child;


void leave();
void list(int size,char *cmda);
void clear();

void envro(int size);
void slept(char**arg,int size);
void stats(char**arg,int size);
void differ(char**arg,int size);
void changedirect(char**arg,int size);

void making(char**arg,int size);
void deleting(char**arg,int size);
void killer(char**arg,int size);

void printing();

//void timeoutcommand(char**arg,int size);

void timeout(char**arg,int size);
void waiting(char**arg,int size);

void timeoutcommand(char**arg,int size);//assist 









    
   

#endif /* MYCOMMANDS_H */

