//external commands now work 100% of the time. no seg faults either. Added path++ and fixed
//path(). all we need now is cd and path--. Also formating so it looks nicer. let me know if
// you find any bugs.




#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "main.h"

void external_command(char *command, char *args[]){
	
	int status = 0;
	pid_t child;
	child = fork();
			
	if(child == 0){							
		descriptor = execv(command, args); 
		exit(1);
									
	}
	else{
		waitpid(-1,&status,NULL);	 
	}	
}
void path(){
	
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
void path_inc(char *arg){	
	pathDir[numPathArgs] = arg;	
	numPathArgs++;
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
		path();
	}
	if(strcmp(command, internal[3]) == 0){
		path_inc(arg);
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
	
					
	
	
	char *buf = malloc(sizeof(CHAR_MAX));
	char *internalComp = malloc(sizeof(CHAR_MAX));	
	char *arg = malloc(sizeof(CHAR_MAX));	
	
	write(1, PROMPT, strlen(PROMPT));
	//clear buffer
	int length = strlen(buf);
	for(int i = 0; i < length; i++){
		buf[i] = ' ';
	}	
	
	read(0,buf,CHAR_MAX);
	char *trimmed = trimwhitespace(buf);
	length = strlen(trimmed);
	

	//check for arguments
	for(int i = 0; i < length; i++){
			if(trimmed[i] == ' '){
				trimmed[i] = 0;				
				arg = &trimmed[i+1];
							
			}
	}
	//Run internal command
				
	for(int i = 0; i < 4; i++){  									
		strcpy(internalComp,internal[i]);						
		if(strcmp(trimmed, internalComp) == 0){		
			runCommand(trimmed, arg);
			FOUND = true;				
			break;
		}		
	}
	
	if(!FOUND){		 
		if(pathDir[0] == NULL){
			printf("\nNo path specified, cannot run command\n");
		}		
		int tempLength = 0;								
		char *temp = malloc(sizeof(CHAR_MAX));				
				
		//Parse and run command		
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
		//would like to throw error but am at a loss for 
		//how to throw a flag if execute is successful		
		/*if(!FOUND){
		printf("\nBad command or invalid path\n");
		}
		*/	
	}	
		
	FOUND = false;
}				
		
int main(){

	do{
		prompt();
	}while(!QUIT);

	return 0;
}
