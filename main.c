#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>

#include "main.h"


void quit(){
	QUIT = true;
}

//Run command matching number

void runCommand(int i){
	if(i == 0){
		quit();
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

	scanf("%s", buf);
	char *trimmed = trimwhitespace(buf);
	
	//Parent runs internal command, if command not found
	//child finds external command to run
	fork();	
	if(fork != 0){

		for(int i = 0; i < 4; i++){  									
			strcpy(internalComp,internal[i]);			
			if(strcmp(trimmed, internalComp) == 0){		
				runCommand(i);
				break;
			}
		}
	}	
	//TODO have child find external commands	
	else{}
}				
	
int main(){

	do{
		prompt();
	}while(!QUIT);

	return 0;
}
