//external commands now work 100% of the time. no seg faults either.




#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "main.h"

void external_command(char *command, char *args[]){
	
	int status;
	//int temp; Never used
	pid_t child;
	child = fork();
			
	if(child == 0){							
		descriptor = execv(command, args); 
		exit(1);
									
	}
	else{
		waitpid(-1,&status,0);	 
	}	
}
void path(){
	
	if(numPathArgs == 0){
		printf("\nNo path set\n");
		return;	
	}	
	
	printf("\n");
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
	
					
	
	int status = 0;
	char *buf = malloc(sizeof(CHAR_MAX));
	char *internalComp = malloc(sizeof(CHAR_MAX));	
	char *arg = malloc(sizeof(CHAR_MAX));	
	
	//clear buffer
	int length = strlen(buf);
	for(int i = 0; i < length; i++){
		buf[i] = ' ';
	}	
	
	//clean up command line
	printf("Fido>");
	scanf("%s",buf);
	char *trimmed = trimwhitespace(buf);
	length = strlen(trimmed);
	//int numArgs = 1; Never used

	//check for arguments
	for(int i = 0; i < length; i++){
		trimmed[i];
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
			printf("\ntemp: %s\n", temp);			
						
			parsedArgs[0] = temp;
			parsedArgs[1] = NULL;
			printf("\nparsed: %s\n", parsedArgs[0]);			
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
