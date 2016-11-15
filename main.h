#ifndef MAIN_H
#define MAIN_H

#define bool char
#define false 0
#define true 1
#include <malloc.h>

//All internal commands
char *internal[5] = {"quit","cs","path","path +","path -"};
bool QUIT;
bool FOUND = false;
char *pathDir;
//Holds directory after cd. Would like to add a default
char *currentDirectory;
#endif
