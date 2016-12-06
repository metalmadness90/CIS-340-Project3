#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <wait.h>
#include <fcntl.h>

#include "main.h"

//Standard trim function: http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way

void printCmd(char *words[], int numWords)
{
    printf("\nprint command\n");
    char *args[numWords+1];
    for (int i = 0; i < numWords; i++)
    {
        //printf("%s ", words[i]);
        args[i] = words[i];
    }
    args[numWords] = 0;
    printf("\n");
    
    int p[2];
    pipe(p);
    if (fork() ==0)
    {
	printf("\nchild\n");        
	close (p[0]);
        dup2 (p[1], 1);
        execv(args[0],args);
        exit(1);
    }
    else 
    {
        close (p[1]);
        dup2 (p[0], 0);
    }
}

void parseCommands(char *words[], int numWords)
{
     int cmdStart = 0;
     int word = 0;
     for (; word < numWords; word++)
     {
         // if word is "|"
         if (words[word][0] == '|' || words[word][0] == 0)
         {
             printCmd(&words[cmdStart], word - cmdStart);
             cmdStart = word + 1;
         }
     }
     //printCmd(&words[cmdStart], word - cmdStart);
     printf("\ncheck\n");
     
     char *args[word - cmdStart + 1];
     int i = 0;
     for (; i <= word - cmdStart; i++)
     {
         args[i] = words[cmdStart + i];
     }
     printf("\nargs: %s\n", args[0]);
     args[2] = 0;
     execv(args[0], args);
}


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

char *pathCheck(char *string, int length){
	char *temp = malloc(sizeof(CHAR_MAX));	
	int tempLength = 0;		
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

void external_command(char *command, char *args[]){
	
	int status = 0;
	pid_t child;
	child = fork();
			
	if(child == 0){							
		execv(command, args); 		
		exit(1);
									
	}
	else{
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

	if(strcmp(arg[1], internal[3]) == 0 && arg[1] != NULL){					
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
	if(strcmp(arg[1], internal[4]) == 0 && arg[1] != NULL){
		for(int i = 0; i < numPathArgs; i++){
			if(arg[2] == NULL){
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
	if(numPathArgs == 0 && strcmp(arg[1], internal[4]) != 0){
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
	//for(int i = 0; i < 5; i++){
	//	printf("\narg: %s\n", arg[i]);
	//}	
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
		int numPipe = 0;
		for(int i = 0; i < numArgs; i++){
			if(arg[i][0] == '|'){
				numPipe++;
			}
		}
			//for(int i = 0; i < numArgs+1; i++){
		for(int i = 0; i < numArgs; i++){
			arg[i] = trimwhitespace(arg[i]);
			length = strlen(arg[i]);								 	
			if(arg[i][0] == '|'){				
				parsedArgs[i] = arg[i];
			}			
			else{				
				parsedArgs[i] = pathCheck(arg[i], length);							
				//printf("\nparsed: %s\n",parsedArgs[0]);								
			}				
			if(parsedArgs[i] == NULL){
				printf("\nError: One or more commands or path directories are invalid\n");
				break;
			}				
		}		
		/*for(int i = 0; i < numArgs; i++){
			printf("\nparsed: %s\n", parsedArgs[i]);
		}*/	
		if(numPipe != 0){	
			parseCommands(parsedArgs, numArgs);		
		}	
		else{
			external_command(parsedArgs[0], parsedArgs);
		}	
	}	
	for(int i = 0; i < numArgs; i++){
		arg[i] = " ";
	}		
	FOUND = false;
}				
		
int main(){

	do{
		prompt();
	}while(!QUIT);

	return 0;
}
