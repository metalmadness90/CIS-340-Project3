#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>

#include "main.h"

void path(char *arg){
	pathDir = malloc(sizeof(CHAR_MAX));
	strcpy(pathDir, arg);
	//TODO: throw error for invalid path
	
	printf("\nCurrent path set: %s\n", arg);
	
}
void quit(){
	QUIT = true;
}

//Run command matching number

void runCommand(char *command, char *arg){
	if(strcmp(command, internal[0]) == 0){
		quit();
	}
	if(strcmp(command, internal[2]) == 0){
		path(arg);
	}
	//add if statement corresponding to command here.
	//numbers will correspond to command order
	// in 'internal' array.
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
	
	int status;
	char *buf = malloc(sizeof(CHAR_MAX));
	char *internalComp = malloc(sizeof(CHAR_MAX));	
	char *arg = malloc(sizeof(CHAR_MAX));	

	read(0,buf,CHAR_MAX);
	printf("\nbuf: %s\n", buf);
	char *trimmed = trimwhitespace(buf);
	int length = strlen(trimmed);
	int numArgs = 1;
	//arg[0] = trimmed;
	//check for arguments
	for(int i = 0; i < length; i++){
		trimmed[i];
			if(trimmed[i] == ' '){
				trimmed[i] = 0;				
				arg = &trimmed[i+1];
							
			}
	}
	printf("\ngood\n");	
	printf("\ntrimmed: %s, arg: %s\n", trimmed, arg);
	//Parent runs internal command, if command not found
	//child finds external command to run
		
	if(fork() != 0){
		wait(&status);		
		for(int i = 0; i < 4; i++){  									
			strcpy(internalComp,internal[i]);			
			printf("\n%d\n",i);			
			if(strcmp(trimmed, internalComp) == 0){		
				runCommand(trimmed, arg);
				FOUND = true;
				printf("\nrun\n");				
				break;
			}
			//else(		
		}
	
	}	
	//TODO have child find external commands	
	else{ 
		if(!FOUND){
			printf("\nchild\n");		
			if(execv(pathDir, trimmed) < 0){
				printf("\nCommand not found or path incorrect\n");
			}
		}
		exit(1);	
	}	
	FOUND = false;
}				
	
int main(){

	do{
		prompt();
	}while(!QUIT);

	return 0;
}
