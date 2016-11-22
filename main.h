#ifndef MAIN_H
#define MAIN_H

#define bool char
#define false 0
#define true 1
#define PROMPT "\nshell$:"


//All internal commands
char *internal[5] = {"quit","cd","path","+","-"};
bool QUIT;
bool FOUND = false;
char *pathDir[50];
char *parsedArgs[50];
int numPathArgs = 0;
//Holds directory after cd. 
char *currentDirectory;
char *temp; 
int externalCheck;
#endif
