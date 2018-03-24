/* My Shell
 * Professor Kim
 * From Toni Oluyide
 
 
 */

#include <sys/types.h> //for dir
#include <dirent.h> //for dir
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <ctype.h>
#include <sys/wait.h>
#include "mycommands.h"
using namespace std;


//constraints for our buffer, up here for easy modification
#define maxbuffa 60


//important values for our program,
#define MAX_SIZE  1000

///stuck with static variables, I use them across my entire program                                               
 char input = '\0';
//our reader info
 char buffer[MAX_SIZE];                                       
 int bChars = 0;
///parser info
char *commandArg[100];
int cmdcount = 0;

///path
char *path;
int status;
int statis;
pid_t parent;
pid_t child;
char *cmmds;




void welcomeScreen(){
    printf("\n================SHELL PROGRAM=====================\n");
    printf("\n=                                                =\n");
    printf("\n=           Designed With Simple C               =\n");
    printf("\n=                                                =\n"); 
    printf("\n==================================================\n");
    printf("\n==----------------------------------------------==\n");
}
void prompt(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Admin$>%s:  ",cwd);
}
//using this to test my buffer worked, leaving it in
void clearinput(){
while (cmdcount != 0) {
                commandArg[cmdcount] = NULL;
                cmdcount--;                                                                      
      }
      bChars = 0;    
}
//reads and parses our input, 


void parse()
{
    char *p, *start;
    int c;
    enum states { DEFAULT, WITHINWORD, WITHINBLOCK } state = DEFAULT;
    
        clearinput();                                                                      
        while ((bChars <MAX_SIZE) && (input != '\n') ) {
                buffer[bChars++] = input;
                input = getchar();
        }
        buffer[bChars] = 0x00;      //pointing to numerical nullpoint                                                      
       
    

    for (p = buffer; cmdcount< 100 && *p != '\0'; p++) {
        c = (unsigned char) *p;
        switch (state) {
        case DEFAULT:
            if (isspace(c)) {
                continue;
            }

            if (c == '"') {
                state = WITHINBLOCK;
                start = p + 1; 
                continue;
            }
            state = WITHINWORD;
            start = p;
            continue;

        case WITHINBLOCK:
            if (c == '"') {
                *p = 0;
                commandArg[cmdcount++] = start;
                state = DEFAULT;
            }
            continue;

        case WITHINWORD:
            if (isspace(c)) {
                *p = 0;
                commandArg[cmdcount++] = start;
                state = DEFAULT;
            }
            continue;
        }
    }

    if (state != DEFAULT && cmdcount < 100)
        commandArg[cmdcount++] = start;

}
///process
void processmaker(){


  pid_t pid;
  int stat;

  child = fork();
  if (child == 0) {
    // Dummy Process
      system("sleep 10 &");
    exit(EXIT_FAILURE);
  } else if (child < 0) {
 
    perror("error");
  } else {
    // Parent process
    do {
      waitpid(child, &stat, WUNTRACED);
    } while (!WIFEXITED(stat) && !WIFSIGNALED(stat));
  }
}




void commandrouting(){
   //just as check for the commands
    int i;
    while(commandArg[i]!=NULL){
        i++;
    }
    cmdcount = i;
   
    
    if (strcmp("exit",commandArg[0])==0){ 
        leave();
        return;
    }
    if (strcmp("ls",commandArg[0])==0){
        list(cmdcount,*commandArg);
        prompt();
        return;
    }
    if (strcmp("clear",commandArg[0])==0){
        clear();
        prompt();
        return;
    }
     if (strcmp("env",commandArg[0])==0){
        envro(cmdcount);
        prompt();
        return;
    }
     if (strcmp("sleep",commandArg[0])==0){
        slept(commandArg,cmdcount);
        prompt();
        return;
    }
     if (strcmp("stat",commandArg[0])==0){
        stats(commandArg,cmdcount);
        prompt();
        return;
    }
    
     if (strcmp("cd",commandArg[0])==0){
        changedirect(commandArg,cmdcount);
        prompt();
        return;
    }
     if (strcmp("mkdir",commandArg[0])==0){
         making(commandArg,cmdcount);
        prompt();
        return;
    }
      if (strcmp("rmdir",commandArg[0])==0){
        deleting(commandArg,cmdcount);
        prompt();
        return;
    }
    if (strcmp("diff",commandArg[0])==0){
        differ(commandArg,cmdcount);
        prompt();
        return;
    }
    if (strcmp("kill",commandArg[0])==0){
        processmaker();
        killer(commandArg,cmdcount);
        prompt();
        return;
    }
       if (strcmp("wait",commandArg[0])==0){
        processmaker();
        waiting(commandArg,cmdcount);
        prompt();
        return;
    }   if (strcmp("timeout",commandArg[0])==0){
        timeout(commandArg,cmdcount);
        prompt();
        return;
    }
    
    
    

    
    
    printf(" -SH command %s doesn't exist",commandArg[0]);
    printf("\n");
    prompt();
    return;
    
   
}






int main(){
        welcomeScreen();
       
        prompt(); 
     
        while (1) {
                input = getchar();
                switch (input) {
                case '\n':                  //if the user just presses enter, next line
                        prompt();                                               
                        break;
                default:
                        //parsing(); 
                       
                        parse();
                
                      
                        commandrouting();
                    
                        break;
                }
        }
        printf("\n");
      //  return 0;
}