#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(){
    pid_t child_id;
    int status;
    
    child_id = fork();
    if(child_id == 0)
    {
        char *argv[3]={"unzip","campur2.zip",NULL};
        execv("/usr/bin/unzip",argv);
    }
    child_id = fork();
    if(child_id==0)
    {
        execlp("touch","touch","daftar.txt",NULL);
    }
    else
    {
        while ((wait(&status))>0);
    }
    int fd[2];
    pipe(fd);
    child_id=fork();
    if(child_id==0)
    {
        close(fd[0]);
        dup2(fd[1],1);
        execlp("ls","ls","campur2",NULL);
    }
    else
    {
        close(fd[1]);
        FILE* pipa=fdopen(fd[0],"r");
        FILE* berkas=fopen("daftar.txt","w");
        char buffer[100];
        while(fgets(buffer,sizeof(buffer),pipa)!=NULL)
        {
            if(strstr(&buffer[strlen(buffer)-5],".txt")!=NULL)
            {
                fprintf(berkas,"%s",buffer);
            }
        }
        fclose(pipa);
        fclose(berkas);
    }
}
