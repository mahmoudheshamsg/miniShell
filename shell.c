#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
char *args[1100];
char *curdir;
char *username;
int tokenize(){
    int quotation =0;
    char *buffer = NULL;
    size_t bufsize=0;
    int readsize = getline(&buffer,&bufsize ,stdin);
        if(readsize == 1)
        {
            return 1;
        }
        *(buffer+readsize-1)=' ';
        int j=0,i=0;
        if(*buffer =='"')
            *buffer = '\0',args[j++]=buffer+1,i++,quotation = 1;
        else if(*buffer !=' ')
            args[j++]=buffer,i++;
        for(; i < readsize-1;i++)
        {

            if(quotation)
            {
                if(*(buffer+i) =='"')
                    *(buffer+i) = '\0',quotation =0;
                continue;
            }

            if(*(buffer+i) == '"'){
                *(buffer+i) = '\0';
                quotation =1;
                args[j++] = buffer+i+1;
                continue;
            }

            if(*(buffer+i) ==' ' && *(buffer+i+1) ==' ')
            {
                *(buffer+i)= '\0';
                continue;
            }

            if(*(buffer+i) ==' ')
            {
                if(*(buffer+i+1) == '"')
                    continue;
                *(buffer+i) = '\0';
                args[j++] = buffer+i+1;

            }

        }
        *(buffer+readsize-1) = '\0';
        args[j] =NULL;
        if(quotation)
            return 1;
    return 0;
}
int cd()
{
    chdir(args[1]);
    return 1;
}
int main(){
    username = getlogin();

    int status;
    int it = 0;
    pid_t pid;
    while(1)
    {
        curdir = getcwd(NULL, 0);
        printf("%s$%s>",curdir,username);
        if(tokenize())
            continue;

        if(!strcmp(args[0],"quit"))
            return 0;
        else if(!strcmp(args[0],"cd" ))
        {
            cd();
            continue;
        }
        pid = fork();
        if(pid == 0)
        {
            printf("%s",args[0]);

           if( execvp(args[0],args) ==-1){
               printf("The command does not exist\n");
               return 1;

            }
            return 0;

        }else
        {
            waitpid(pid, &status, 0);
        }
    }
    return 1;
}
