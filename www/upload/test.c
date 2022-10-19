#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void check_leaks()
{

    system("leaks a.out");
}

int main()
{
    atexit(&check_leaks);
    int pid = fork();
    char *hola = malloc(5);
    hola[0] = 'h';
    hola[1] = 'o';
    hola[2] = 'l';
    hola[3] = 'a';
    hola[4] = '\0';

    if (pid == 0)
    {
        //free(hola);
        printf("hola desde el fork()\n");
        
        sleep(2);
        exit(0);
    }
    free(hola);
    wait(NULL);
    
    return 1;
}