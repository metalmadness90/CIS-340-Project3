#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <wait.h>
#include <fcntl.h>


#include "main.h"

//external commands if command is a pipeline. 
void external_pipeline(char *words[], int numWords)
{
     int status;	
     int count = 0;
     int cmdStart = 0;
     int word = 0;
     int p[2];
     int p2[2];
     for (; word < numWords; word++)
     {
         // if word is "|"
         if (words[word][0] == '|' )
         {
	      int commandWords = word - cmdStart;
              char *command[commandWords+1];
              int i = 0;
              for (; cmdStart < word; cmdStart++)
              {
              	command[i] = words[cmdStart];
    	        i++;
	      }
    	      count++;
              command[commandWords] = 0;
              // run after parsed
              
              if(count == 2){
                  pipe(p2);
                  pid_t child2;
	          child2 = fork();
                  if(child2 == 0){
	              dup2(p[0],0);
                      dup2(p2[1], 1);
                      close(p[0]);
       		      close(p[1]);
                      execv(command[0],command);
		      printf("\nOne or more commands or path directories are invalid\n");		
		      exit(1);
		  }
                  else{
                      close(p[0]);
                      close(p[1]);
		  }	    	        
	      }
              else{
		    pipe(p);
                    pid_t child;
	            child = fork();
                    
                    if(child == 0){
                        close (p[0]);
              	        dup2 (p[1], 1);
                        execv(command[0],command);
		        printf("\nOne or more commands or path directories are invalid\n");		
		        exit(1);
                    }    
	      }
	      cmdStart = word + 1;
         }
     }
     // parse and run final command in proc
     char *args[word - cmdStart + 1];
     int i = 0;
     for (; i <= word - cmdStart; i++)
     {
         args[i] = words[cmdStart + i];
     }
     args[word - cmdStart + 1] = 0;
    //if 3 pipe arguments execute here 
    if(count == 2){
    	if(fork() == 0){
    	    dup2(p2[0], 0);  
    	    close(p2[1]);
    	    close(p2[0]);
    	    printf("\n");        
            execv(args[0], args);
	    printf("\nOne or more commands or path directories are invalid\n");        
	    exit(1);
        }
        else{
       	   sleep(1);
	   wait(&status);	
	}
    }
    else{
        //if there are only 2
        if(fork() == 0){
    	    dup2(p[0], 0);  
    	    close(p[1]);
    	    close(p[0]);
    	    printf("\n");        
            execv(args[0], args);
	    printf("\nOne or more commands or path directories are invalid\n");        
	    exit(1);
        }
        else{
            sleep(1);         
	    wait(&status);	        
	}
    }
}

//Standard trim function: http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way

char *trimwhitespace(char *str){
  
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}
		
// Search and parse correct path variable to string.
// needs work, trying to find a way to throw a flag for a bad path,
// while simultaneously keeping any strings that are options, eg. '-l'
char *pathCheck(char *string, int length){
	char *temp = malloc(sizeof(char));	
	int tempLength = 0;		
        
	// Check for redirection command
        if(string[0] == '<' || string[0] == '>'){
                REDIR = true;
                return string; 
        }
	for(int i = 0; i < numPathArgs; i++){					
		strcpy(temp,pathDir[i]);
		tempLength = strlen(temp);					
		temp[tempLength] = '/';
		tempLength++;
							
		for(int i = 0; i < length; i++){	
			temp[tempLength] = string[i];					
			tempLength++;						
		}				
		temp = trimwhitespace(temp);
		//make sure command exists in current path
		int check = open(temp, O_RDONLY);
		if(check >= 0){
			return temp;						
		}
		close(check);	
	}        	
        return string;
}

void external_command(char *command, char *args[], int numArgs){
	
	pid_t child;
	child = fork();
	int status;
		
	if(child == 0){							
		if(REDIR){                	
			for(int i = 0; i < numArgs; i++){
                		if(args[i][0] == '>'){
					int out, re_out;                        		
					close(1);
                        	        out = open(args[i+1], O_RDWR|O_CREAT|O_APPEND, 0600); 
					if(out < 0){
						printf("\nFile not found\n");
						exit(1);
					}				        
					re_out =  dup(fileno(stdout));       
		        	        dup2(re_out, fileno(stdout));
                        	        args[i] = NULL;
					break;                        
				}
                        	if(args[i][0] == '<'){
					int in, re_in;                        		
					close(0);
                                        in = open(args[i+1], O_RDONLY|O_APPEND); 
					if(in < 0){
						printf("\nFile not found\n");
						exit(1);
					}						        
					re_in =  dup(fileno(stdin));       
		        	        dup2(re_in, fileno(stdin)); 
					args[i] = NULL;					
					args[i+1] = NULL;	                                
					break;
                       		}    
			}		
		}				
		printf("\n");	
		execv(command, args); 		
		printf("\nInvalid command or path directory\n");
                exit(1);
								
	}
	else{ 
		sleep(1);
		wait(&status);	
	}	
}
//Change directory
void cd(char *arg[]){
	
	int descriptor = chdir(arg[1]);
	
	if(descriptor != 0){
		printf("\nDirectory not found\n");
	}
	else{
		currentDirectory = arg[1];
	}
}

void path(char *arg[]){
	
	//If command includes + or -, do appropriate actions, else display current path
	
	temp = getcwd(temp, PATH_MAX +1);
        if(arg[1] != NULL){
		if(strcmp(arg[1], internal[3]) == 0){					
			int check = chdir(arg[2]);	
			chdir(temp);
			//Make sure directory exists		
			if(check != 0){
				printf("\nInvalid path: Not a directory\n");
				return;
			}		
			pathDir[numPathArgs] = arg[2];	
			numPathArgs++;
			return;		
		}	
		
	//Find specified path and replace with path in front of it (if any, 
	//otherwise replace with NULL is fine.)		
		if(strcmp(arg[1], internal[4]) == 0){
			
                	for(int i = 0; i < numPathArgs; i++){				
				if(arg[2] == NULL){
					printf("\nNo path entered for removal\n");
					return;
				}
			
				if(strcmp(arg[2], pathDir[i]) == 0){
					FOUND = true;				
					pathDir[i] = pathDir[i+1];
						
					while(pathDir[i] != NULL){
						i++;
						pathDir[i] = pathDir[i+1];
					}					
					numPathArgs--;					
					return;
				}
			}	
			if(!FOUND){
				printf("\nCould not find specified path to remove\n");
			}	
		}	
		
			
	}
	if(numPathArgs == 0){
			printf("\nNo path set\n");
			return;	
	}
	for(int i = 0; i < numPathArgs; i++){		
		if(numPathArgs == 1 || i == (numPathArgs - 1)){
			printf("%s", pathDir[i]);
		}			
		else{		
			printf("%s:", pathDir[i]);
		}	
	}
	printf("\n");
	
}

void quit(){
	QUIT = true;
}

//Run command matching string in 'internal' array

void runCommand(char *command, char *arg[]){
	if(strcmp(command, internal[0]) == 0){
		quit();
	}
	if(strcmp(command, internal[2]) == 0){
		path(arg);
	}	
	if(strcmp(command, internal[1]) == 0){
		cd(arg);
	}	
}

void prompt(){
	
	char *buf = malloc(sizeof(CHAR_MAX));
	char *internalComp = malloc(sizeof(CHAR_MAX));	
	char *arg[50]; 	
	arg[1] = NULL;
	
	write(1, PROMPT, strlen(PROMPT));
	
	//Display current directory if different than starting directory
	if(currentDirectory != NULL){
		write(1, currentDirectory, strlen(currentDirectory));
		write(1, "$ ", 2);
	}	
	//clear buffer
	int length = strlen(buf);
	for(int i = 0; i < length; i++){
		buf[i] = ' ';
	}	
	read(0,buf,CHAR_MAX);
	char *trimmed = trimwhitespace(buf);
	length = strlen(trimmed);
        //parse arguments
	int numArgs = 1;	
		
	for(int i = 0; i < length; i++){
			if(trimmed[i] == ' '){
				trimmed[i] = 0;				
				arg[numArgs] = &trimmed[i+1];
				numArgs++;			
			}
	}
	arg[0] = trimmed;		
	
	//Run internal command
				
	for(int i = 0; i < 4; i++){  									
		strcpy(internalComp,internal[i]);						
		if(strcmp(arg[0], internalComp) == 0){		
			runCommand(arg[0], arg);
			FOUND = true;				
			break;
		}		
	}
	
	if(!FOUND){		 
		if(pathDir[0] == NULL){
			printf("\nNo path specified, cannot run command\n");
		}		
		// check for '|'
                int numPipe = 0;
		for(int i = 0; i < numArgs; i++){
			if(arg[i][0] == '|'){
				numPipe++;
			}
		}
		for(int i = 0; i < numArgs; i++){
			arg[i] = trimwhitespace(arg[i]);
			length = strlen(arg[i]);								 	
			
			parsedArgs[i] = pathCheck(arg[i], length); 					                		
		}			
		if(numPipe != 0){	
			external_pipeline(parsedArgs, numArgs);		
		}	
		else{
			external_command(parsedArgs[0], parsedArgs, numArgs);
		}	
		for(int i = 0; i < numArgs; i++){
			parsedArgs[i] = NULL;	
        		arg[i] = NULL;
		}		         
        }	
	for(int i = 0; i < numArgs; i++){	
        		arg[i] = NULL;
	}        
	FOUND = false;
	REDIR = false;
}				
		
int main(){

	do{				
		prompt();
	}while(!QUIT);

	return 0;
}
