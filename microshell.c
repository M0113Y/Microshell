/************************************************************
* JON MOLLEY										3/21/2014
*
*
* PURPOSE: To create a microshell program that allows piping 
*          between commands.
*
* || is the pipe command...dont use |
*************************************************************/

#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>



int main() 
{
 //pipe file descriptors  0 read    1 write
 int pipefd[2]; 

 
 //buffer takes stdin  
 //first and last arrays to hold parsed strings  
 
 char buf[1024]; 
 char first[20];
 char last[20];
 
 //child ids and pointers for shell commands
 pid_t pid1, pid2; 
 int status; 
 int cnt;
 char* comarg[64];	//left side of pipe
 char* comarg2[64]; //right side of pipe
 char* ptr;
 char* ptr2;

 //flags for the pipe and quiting the program
 bool pipe_flag = false; 
 bool quit = false;

 printf("myshell> "); 
  
 while (fgets(buf,1024,stdin) != NULL) 
 { 

	/* parse buffer */

	 //check for pipe command
	 if ((ptr = strstr(buf, "||")) != NULL)
	 {
	   //copy left side of pipe
	   int pos = ptr - buf; 
	   strncpy(first, buf, pos);
     
	   //check after || and copy right side of pipe
	   if ((buf[pos + 2]) != '\0')
	   {  
		 int cnt2 = 0;
		 for (unsigned int i = pos + 2; i < strlen(buf); i++)
		 {
		   last[cnt2] = buf[i]; 
		   cnt2++;    
		 }
		 
		 pipe_flag = true; 
	   }
	 }


 buf[strlen(buf) - 1] = 0;  //set last char to 0

//if pipe command is true then parse both first and last arrays otherwise
//just parse the buffer

 if (pipe_flag)
 {
	   cnt = 0;
	   ptr = strtok(first, " ");       
	   comarg[cnt] = ptr;
  
	   while (ptr != NULL)
	   {
		 ptr = strtok(NULL, " "); 
		 cnt++;  
		 comarg[cnt] = ptr;
	   } 

	   comarg[cnt] = NULL; //set last spot to null
 

	   cnt = 0;
	   ptr2 = strtok(last, " \n");
	   comarg2[cnt] = ptr2;
  
	   while (ptr2 != NULL)
	   {
		 ptr2 = strtok(NULL, " \n"); 
		 cnt++;  
		 comarg2[cnt] = ptr2;
	   } 
   
	   comarg2[cnt] = NULL; // set last spot to null    

 }
 else 
 {
	   cnt = 0;
	   ptr = strtok(buf, " ");
	   comarg[cnt] = ptr;

	   while (ptr != NULL)
	   {
		 ptr = strtok(NULL, " "); 
		 cnt++;  
		 comarg[cnt] = ptr;
	   } 

	   comarg[cnt] = NULL; // set last spot to null 
 }


 //check to see if pipe failed 
 if (pipe(pipefd) == -1)
 {
	perror("pipe failed");
	exit(-1);
 }
  
 if ((pid1 = fork()) < 0) 
 {
	perror("fork error"); 
	exit(-1);
 } 
 else if (pid1 == 0) //child
 {	

   //only use descriptors if pipe flag is set to true
   if (pipe_flag)
   {
	 close(1); //close stdout
	 dup(pipefd[1]); //replace stdout with pipe write
	 close(pipefd[0]); //close close read		
	 close(pipefd[1]); 
   }

  //prevents child from printing an error on quit command
  if (strcmp(buf, "quit") == 0 || strcmp(buf, "q") == 0)
  {
 	quit = true;
  }

  if (!quit)
  {
	  //excute commands if found otherwise print error and exit
     if (execvp(comarg[0], comarg) < 0)	
     {
		printf("\nERROR: command not found...\n");
		exit(-1);
     }
   }

  exit(127); 
 } 
 
 // only use second child for pipe command
 if (pipe_flag)
 {
	 if ((pid2 = fork()) < 0)
	 {
	   printf("fork error"); 
	 }
	 else if (pid2 == 0) 
	 {
		close(0); //close stdin
		dup(pipefd[0]); //replace stdout with pipe read
		close(pipefd[1]); //close write	
		close(pipefd[0]);

		//if commands found use it for the right side of ||
		 if (execvp(comarg2[0], comarg2) < 0)	
		 {
			printf("\nPIPE ERROR: right side not found...\n");
		    exit(-1);
		 }

       exit(127); 
      }
 }


/* parent */ 

  //close read and write if pipe was called and check child 
  if (pipe_flag)
  {
	  close(pipefd[0]);
	  close(pipefd[1]);

    if ((pid2 = waitpid(pid2, &status, 0)) < 0)
    {
       printf("pipe 2 error");
    }

  }

  if ((pid1 = waitpid(pid1, &status, 0)) < 0) 
  {
     printf("waitpid error"); 	
  }


  // exit program  
  if (strcmp(buf, "quit") == 0 || strcmp(buf, "q") == 0)
  {
 	printf("\nexiting shell...\n\n");
	exit(0);
  }
  
     printf("myshell> ");
     pipe_flag = false;    
 }
} 

