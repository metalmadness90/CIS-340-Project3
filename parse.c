#include <unistd.h>
#include <stdio.h>

void printCmd(char *words[], int numWords)
{
    char *args[numWords+1];
    for (int i = 0; i < numWords; i++)
    {
        printf("%s ", words[i]);
        args[i] = words[i];
    }
    args[numWords] = 0;
    printf("\n");

    int p[2];
    pipe(p);
    if (fork() ==0)
    {
        close (p[0]);
        dup2 (p[1], 1);
        execv(args[0],args);
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
         if (words[word][0] == '|')
         {
             printCmd(&words[cmdStart], word - cmdStart);
             cmdStart = word + 1;
         }
     }
     //printCmd(&words[cmdStart], word - cmdStart);
     char *args[word - cmdStart + 1];
     int i = 0;
     for (; i <= word - cmdStart; i++)
     {
         args[i] = words[cmdStart + i];
     }
     args[2] = 0;
     execv(args[0], args);
}

int main()
{
     // char *rawCommand = "/bin/ls -l | grep \"t\"";
     int CMD_LEN = 8;
     char *words[CMD_LEN];
     words[0] = "/bin/cat";
     words[1] = "main.c";
     words[2] = "|";
     words[3] = "/bin/grep";
     words[4] = "t";
     words[5] = "|";
     words[6] = "/bin/grep";
     words[7] = "a";
    //  words[8] = "|";
    //  words[9] = "/bin/grep";
    //  words[10] = "c";



     parseCommands(words, CMD_LEN);
     
     return 0;
}
