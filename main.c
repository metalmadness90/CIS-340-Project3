#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <wait.h>

#include "main.h"

void external_command(char *command, char *args[]){
	
	int status = 0;
	pid_t child;
	child = fork();
			
	if(child == 0){							
		execv(command, args); 
		//externalCheck++;		
		exit(1);
									
	}
	else{
		wait(&status);	 
	}	
}
//Change directory
void cd(char *arg[]){
	
	int descriptor = chdir(arg[0]);
	
	if(descriptor != 0){
		printf("\nDirectory not found\n");
	}
	else{
		currentDirectory = arg[0];
	}
}
void path(char *arg[]){
	
	//If command includes + or -, do appropriate actions, else display current path
	
	if(strcmp(arg[0], internal[3]) == 0 && arg[0] != NULL){					
		pathDir[numPathArgs] = arg[1];	
		numPathArgs++;
		return;		
	}	
	//Find specified path and replace with path in front of it (if any, 
	//otherwise replace with NULL is fine.)		
	if(strcmp(arg[0], internal[4]) == 0 && arg[0] != NULL){
		for(int i = 0; i < numPathArgs; i++){
			if(arg[1] == NULL){
				return;
			}
			
			if(strcmp(arg[1], pathDir[i]) == 0){
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
	if(numPathArgs == 0 && strcmp(arg[0], internal[4]) != 0){
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
	int numArgs = 0;	
	for(int i = 0; i < length; i++){
			if(trimmed[i] == ' '){
				trimmed[i] = 0;				
				arg[numArgs] = &trimmed[i+1];
				numArgs++;			
			}
	}
	//Run internal command
				
	for(int i = 0; i < 4; i++){  									
		strcpy(internalComp,internal[i]);						
		if(strcmp(trimmed, internalComp) == 0){		
			runCommand(trimmed, arg);
			FOUND = true;				
			for(int i = 0; i < numArgs; i++){
				arg[i] = " ";
			}	
			break;
		}		
	}
	
	if(!FOUND){		 
		if(pathDir[0] == NULL){
			printf("\nNo path specified, cannot run command\n");
		}		
		int tempLength = 0;								
		char *temp = malloc(sizeof(CHAR_MAX));				
		externalCheck = 0;		
		//Parse and run command	through each path variable	
		for(int i = 0; i < numPathArgs; i++){
			for(int i = 0; i < tempLength; i++){
				temp[i] = ' ';
			}					
			strcpy(temp,pathDir[i]);
			tempLength = strlen(temp);					
			temp[tempLength] = '/';
			tempLength++;
							
			for(int i = 0; i < length; i++){	
				temp[tempLength] = trimmed[i];					
				tempLength++;						
			}				
			temp = trimwhitespace(temp);			
			parsedArgs[0] = temp;	
			parsedArgs[1] = NULL;						
			external_command(temp, parsedArgs);			
		}							
	}	
		
	FOUND = false;
}				
		
int main(){

	do{
		prompt();
	}while(!QUIT);

	return 0;
}
