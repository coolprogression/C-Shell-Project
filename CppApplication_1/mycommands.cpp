/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mycommands.cpp
 * Author: Admin
 *
 * Created on March 14, 2017, 7:41 PM
 */

#include <cstdlib>

using namespace std;

#include <sys/types.h> //for dir
#include <pwd.h>
#include <grp.h>
#include <dirent.h> //for dir
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h> 
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include<sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<time.h>
#include <sys/stat.h>


//constraints for our buffer, up here for easy modification
#define maxbuffa 60


//important values for our program,
#define MAX_SIZE 1000


///stuck with static variables, I use them across my entire program                                               
extern char input;
//our reader info
extern char buffer[MAX_SIZE];                                       
extern  int bChars;

extern char *commandArg[100];
extern int cmdcount;
extern char **environ;
extern char *path;
extern int status;
extern int statis;
extern pid_t parent;

extern pid_t child;





void leave(){
    exit(0);
}
void list(int size,char *cmda){
/// 
DIR *dir;
struct dirent *director;

char pathd[1000];


char *r = getcwd(pathd,sizeof(pathd));
path = r;

dir=opendir(r);

if(size != 1){
    printf("Invalid Argument \n");
    return;
}    

while((director=readdir(dir))!=NULL){
    if(director ->d_name !="."){ //we don't need dots
    //do nothing
    printf("%s\n",director->d_name);    
    }
    
}

closedir(dir);
return;

}

void clear(){
//escape command
    printf("\033[H\033[J");
}


//prints out enviroment
void envro(int size){
    if(size!=1){
        printf("%s : no such directory or file \n",commandArg[1]);
        return;
    }
//environ is a pointer for the enviroment variables in c, this just prints out the enviroment
//variables line by line
int i = 0;
while(environ[i]) {
  printf("%s \n", environ[i++]); 
  
}
printf("\n ");
 return;   
}

void slept(char**arg,int size){
  
    if(size ==1){
        printf("Missing arguments after sleep \n");
        return;
    }
    if((atoi(arg[1])==0) || atoi(arg[1])<0){
        printf("%s not valid time interval \n",commandArg[1]);
        return;
    }else{
        sleep(atoi(arg[1]));
        return;
    }
   
  
    

}

void stats(char**arg,int size){
    struct stat buf;
    int i = 1;
    if(size==1){
        printf("Missing Operand \n");
        return;
    }
    if(stat(arg[1],&buf)!=0){
        printf("%s : file or directory doesn't exist \n",arg[1]);
        return;
    }
    
    
    
    if(stat(arg[1],&buf)==0){
        printf("File: '%s'    \n",arg[1]);
        printf("Size: %jd     Blocks:%ld        IOBlock:%ld "  , (int)buf.st_size,buf.st_blocks,(long)buf.st_blksize);
   
    //we need to determine which type of file it is
    switch (buf.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
    }
    
    int m = buf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO) ;
    printf("Device:%xh / %dd       Inode:%ld     Links:%ld       \n",(int)buf.st_dev,(int)buf.st_dev,(long)buf.st_ino,(long) buf.st_nlink);
   
    
    printf("Access: (%lo/",m);
    
    
    
    printf( (S_ISDIR(buf.st_mode)) ? "d" : "-");
    printf( (buf.st_mode & S_IRUSR) ? "r" : "-");
    printf( (buf.st_mode & S_IWUSR) ? "w" : "-");
    printf( (buf.st_mode & S_IXUSR) ? "x" : "-");
    printf( (buf.st_mode & S_IRGRP) ? "r" : "-");
    printf( (buf.st_mode & S_IWGRP) ? "w" : "-");
    printf( (buf.st_mode & S_IXGRP) ? "x" : "-");
    printf( (buf.st_mode & S_IROTH) ? "r" : "-");
    printf( (buf.st_mode & S_IWOTH) ? "w" : "-");
    printf( (buf.st_mode & S_IXOTH) ? "x" : "-");
    
    //we need to get the user ids/group id
    struct passwd *pw = getpwuid(geteuid());
    struct group *grp;
    char *output;
    
    if ((grp = getgrgid(buf.st_gid)) != NULL){
          output = grp->gr_name;
         printf(")     Uid(%u/  %s)     Gid(%u/ %s)",buf.st_uid,pw->pw_name,buf.st_gid,output);
    }else{
        printf(")     Uid(%u/  %s)     Gid(%u/ none)",buf.st_uid,pw->pw_name,buf.st_gid);   
    }
    
   /*Time formatting */
   struct tm *om;
   char b1[100];
   

om = localtime(&buf.st_atime);
strftime(b1, sizeof(b1), "%Y-%m-%d %H:%M:%S", om);
printf("\n Access: %s\n", b1);

om = localtime(&buf.st_mtime);
strftime(b1, sizeof(b1), "%Y-%m-%d %H:%M:%S", om);
printf(" Modify: %s\n", b1);

om = localtime(&buf.st_ctime);
strftime(b1, sizeof(b1), "%Y-%m-%d %H:%M:%S", om);
printf(" Change: %s\n", b1);

 
    }else{
        printf("File or Directory doesn't exist  \n");
    }
    
}
   

/* */
void differ(char**arg,int counts){
    if(counts ==1){
        printf("Missing operand after diff,you need 2 \n");
        return;
    }
    if(counts == 2){
        printf("Missing operand after diff, you need 2 \n");
        return;
    }
    FILE *f1;
    FILE *f2;
    
    
    
    
    if((f1 = fopen(arg[1],"r"))==NULL){
      printf("Can't open argument #1 %s  \n",arg[1]); 
      fclose(f1);
      return;
    }
    
  
     if((f2 = fopen(arg[2],"r"))==NULL){
      printf("Can't open argument #1 %s  \n",arg[2]); 
      fclose(f2);
      return;
    }
    int row;
    size_t position =0;
    char *line = NULL;
    
    int filelinecounter = 0;
    
    char* file1 [100];  
    char* file2 [100];
    for(int i = 0; i<100; i++){
        file1[i] = (char*) malloc(sizeof(char)*100);
        file2[i] = (char*) malloc(sizeof(char)*100);
               
    }
    
  
    
    
    while((row = getline(&line,&position,f1))!=-1){
        
      strcpy(file1[filelinecounter],line);
    //  printf("F1: %s \n",file1[filelinecounter]);
        filelinecounter++;
        
              
    }
    filelinecounter = 0;
    position = 0;
    line = NULL;
    
    while((row = getline(&line,&position,f2))!=-1){
        
       strcpy(file2[filelinecounter],line);
    //   printf(" F2: %s \n",file2[filelinecounter]);
       filelinecounter++;   
    }
    
    //initating our array of equalivancies
    bool filecmp[100];
    int i; 
    bool dotheymatch = true;
    for(i=0; i<100; i++){
      filecmp[i] = dotheymatch;  
    }
    
    ///making array of file equivalancies
    int start=0;
    int end=0;
    bool startindexlocated = false;
    
    for (i = 0; i<filelinecounter+1; i++){
        if( strcmp(file1[i],file2[i])!=0){
            filecmp[i]= false;
            if(startindexlocated == false){
                start = i;
                startindexlocated = true;
            }
           
        }else{
            char label[20]="";
            char lb[3];
            
            int j = start;
           
            startindexlocated = false;
            for(j; j<i; j++){
                sprintf(lb,"%d",j+1);
                strcat(label,lb);
                if((i-(j+1))!=0){
                    strcat(label,",");
                }else{
                    strcat(label,"c");  
                   
                    if(i-start==1){
                    strcat(label,lb); 
                    }else{ ////multiple commas
                        
                        int k = start;
                        for(k; k<i; k++){
                            sprintf(lb,"%d",k+1);
                            strcat(label,lb);
                            if((i-(k+1))!=0){
                                strcat(label,",");
                        }
                        
                        }
                    }
                }
            } 
            //1,2c1,2
            //label 1,2c currently
            
            
            printf("%s \n",label);
            j=start;
            for(j; j<i; j++){
                printf("\n< %s  ",file1[j]);
            }
            j=start;
            printf("--- \n");
            for(j; j<i; j++){
                printf("\n> %s  ",file2[j]);
            }
        }
    }
    return;
}  

 


void changedirect(char**arg,int size){
   if(size>3){
       printf("Too many arguments, \n");
       return;
   }
    
    
    

        if ((size == 1) || ((strcmp("~",arg[1])==0) && size==2)){
	    chdir(getenv("HOME"));
          
            char pathd[1000];
            getcwd(pathd,sizeof(pathd));
            path = pathd;
           // printf("%i   \n",size);
            return;
} else{
            
            
//    char out[200];
//
//    
//    
//    int i= 1;
//    while(i<cmdcount){
//    strcat(out,commandArg[i]);
//    if(i!=cmdcount-1){
//    strcat(out," ");
//    }
//    i++;
//    }
    
    if(chdir(arg[1])!=0){
    printf("Invalid Argument, No Such File/Directory %s \n",arg[1]);
    }
    
    char pathd[1000];
    getcwd(pathd,sizeof(pathd));
    path = pathd;       
              
            
}
  
}
void making(char**arg,int size){
    struct stat s1;
    int i=1;
     if(size ==1){
        printf("Missing Arguments \n");
        return;
   }
   
     
      while(i<cmdcount){
         if((mkdir(arg[i], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0)){
           printf("%s : directory already exist \n",arg[i]);         
       }
        i++;
     }
      
      
}


void deleting(char**arg,int size){
    int i=1;
    if(size ==1){
        printf("Missing Arguments \n");
        return;
    }
    while(i<cmdcount){
        if(rmdir(arg[i])!=0){
            printf("%s : directory doesn't exist \n",arg[i]);         
        }
        i++;
    }
    
    return;
}

void killer(char**arg,int size){
    if(size == 1){
        printf("kill: usage: need process  \n");
        return;
    }
    
    if ( (size ==2)  && (atoi(arg[1])!=0) ){
        
        if (kill(atoi(arg[1]),SIGKILL)==-1){
            printf(" %s: Invalid Process:  \n",arg[1]);
            return;
        }else{
            printf("%s, Terminated  \n",arg[1]); 
            return; 
            }
        
        
    }
    // (0) Kill
    // (1) SIGNAL
    // (2) Process
    if((size ==3)  && (atoi(arg[2])!=0) ){
        
        
        if(strcmp(arg[1],"-SIGKILL")==0) {
           
            if (kill(atoi(arg[2]),SIGKILL)!=0){
            printf("%s, Invalid Process ID: \n",arg[2]);
            return;
            }else{
            printf("%s, Terminated \n",arg[2]);  
            return;
            }
           
         }
         
         if (strcmp(arg[1],"-SIGINT")==0) {
           
            if (kill(atoi(arg[2]),SIGINT)!=0){
            printf("%s, Invalid Process ID:   \n",arg[2]);
            return;
            }else{
            printf("%s, Terminated \n",arg[2]);  
            return;
            }
            
         }
          if (strcmp(arg[1],"-SIGTERM")==0) {
           
            if (kill(atoi(arg[2]),SIGTERM)!=0){
            printf("%s, Invalid Process ID: \n",arg[2]);
            return;
            }else{
            printf("%s, Terminated \n",arg[2]);
            return;
            }
           
         }
        
         if (strcmp(arg[1],"-SIGSTOP")==0) {
           
            if (kill(atoi(arg[2]),SIGSTOP)!=0){
            printf("%s, Invalid Process ID:  \n",arg[2]);
            return;
            }else{
            printf("%s, Terminated \n",arg[2]);  
            return;
            }
          
         }
        
         if (strcmp(arg[1],"-SIGUSR1")==0) {
           
            if (kill(atoi(arg[2]),SIGUSR1)!=0){
            printf("%s, Invalid Process ID: \n ",arg[2]);
            return;
            
            }else{
              printf("%s, Terminated \n ",arg[2]);  
              return;
            }
           
         }
        
         if (strcmp(arg[1],"-SIGUSR2")==0) {
           
            if (kill(atoi(arg[2]),SIGUSR2)!=0){
            printf("%s, Invalid Process ID: \n ",arg[2]);
            return;
            }else{
              printf("%s, Terminated \n ",arg[2]);  
              return;
            }
          
         }else{
             printf("Invalid Signal \n");
             
         }
        
        
    }
    
  return;  
}
//as
void timeoutcommand(char**arg,int size);




void timeout(char**arg,int size){
    int internstatus = 0;
   
    if(size<3){
       printf("needs more arguments \n");
       return;
    }
     if(atoi(arg[1])==0){
        printf("%s, not a valid time interval \n",arg[1]);
        return;
    }
    //new command
    char **newArray = commandArg + 2; 
    int newsize = cmdcount - 2;
    
    //printf("%s, \n",newArray[0]);
    if(strcmp("cd",newArray[0])==0){ //cd never runs thru timeout
        printf("command cd has failed to run, No Such File or Directry \n");
        return;
    }
     if(strcmp("exit",newArray[0])==0){ //cd never runs thru timeout
        printf("command exit has failed to run, No Such File or Directry \n");
        return;
    }
    timeoutcommand(newArray,newsize);
    if(status == 0){ //we gonna run like normal
        if(strcmp("sleep",newArray[0])==0){
       // pid_t rpid;
        pid_t rid = fork();
       
        
        if (rid > 0){
            sleep(atoi(arg[1]));
            kill(rid,SIGKILL);
        }
        if(rid == 0){
              slept(newArray,newsize);
              return;
            }  
            printf("[Done]    %s %s \n",newArray[0],newArray[1]); // a check if the process was done
        }  
         
     return; 
    } else{ /// start process (exec) //on
          printf("command not found: \n");
         return;
       
        
        ///just for testing purposes below, if i wanted to test a program on my shell and time it out on shell
//        pid_t pid = fork();
//       
//        
//        if (pid > 0){
//            sleep(atoi(arg[1]));
//            kill(pid,SIGKILL);
//        }
//        if(pid == 0){
//            if(execvp(newArray[0],newArray)==-1){
//                perror("\n");
//                return;
//            }  
//            
//        }  
// 
     }  
    }
   
    
  
    






void waiting(char**arg,int siz){
    int stat;
    
 
    
if(siz==1)
    return;    
 
if(atoi(arg[1])==0){
    printf("(%s),Not a Valid Child of The Shell \n",arg[1]);
    return;
}   
    
    
    if(getpgid(atoi(arg[1]))!=getpid()){
       printf("(%s),Not a Valid Child of The Shell \n",arg[1]);  
       return;  
    }
  
    if(kill(atoi(arg[1]),0)==-1){
       printf("(%s),Not a Valid Child of The Shell \n",arg[1]);  
       return;  
    }else{
        while(kill(atoi(arg[1]),0)==0){
            sleep(1);
        }
        printf("Process Completed: \n");
       

      
}  
  return;
}   

    
 


void timeoutcommand(char**arg,int size){
    
    if (strcmp("ls",arg[0])==0){
        list(size,*arg);
        return;
    }
    if (strcmp("clear",arg[0])==0){
        clear();
        return;
    }
     if (strcmp("env",arg[0])==0){
        envro(size);
        return;
    }
     if (strcmp("stat",arg[0])==0){
        stats(arg,size);
        return;
    }
 
     if (strcmp("mkdir",arg[0])==0){
         making(arg,size);
        return;
    }
      if (strcmp("rmdir",arg[0])==0){
        deleting(arg,size);
        return;
    }
    if (strcmp("diff",arg[0])==0){
        differ(arg,size);
        return;
    }
    if (strcmp("kill",arg[0])==0){
        killer(arg,size);
        return;
    }
     if (strcmp("wait",arg[0])==0){
        waiting(arg,size);
        return;
        
    } if (strcmp("sleep",arg[0])==0){
         return;
    }
    if (strcmp("timeout",arg[0])==0){
        timeout(arg,size);
        return;
    } else{
        status = 1;
    }
    
    //if we are going to execute program to test timeout
  
    
}


void printing(){
    int i;
       
         printf("%s \n",commandArg[3]);
       
   
      return;
}