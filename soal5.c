#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

int main() 
{
    pid_t pid, sid;
    int status;

    pid = fork();

    if (pid < 0) 
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) 
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();

    if (sid < 0) 
    {
        exit(EXIT_FAILURE);
    }

    if ((chdir(".")) < 0) 
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //Membuat dan mengcompile program C untuk mengkill proses
    FILE* stop = fopen("Soal5b.c","w");
    fprintf(stop,"#include <unistd.h>\n#include <sys/wait.h>\nint main(){int dump;if(fork()==0)execlp(\"kill\",\"kill\",\"%d\",NULL);else{while(wait(&dump)>0);}execlp(\"rm\",\"rm\",\"kill5b.out\",NULL);}",getpid());
    fclose(stop);
    if(fork()==0)
    {
        execlp("gcc", "gcc", "Soal5b.c", "-o", "kill5b.out", NULL);
    }

    //folder inisial
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    char wkt[25];
    char dir[50] = "/home/bonnis/log/";
    snprintf(wkt, sizeof wkt, "%02d:%02d:%04d-%02d:%02d",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min);
    strcat(dir,wkt);

    if(fork()==0)
    {
        execlp("mkdir","mkdir","-p",dir,NULL);
    }
    else
    {
        while(wait(&status)>0);
    }
    
    //Loop
    int i=1;
    while(1) 
    {
        //Membuat folder baru jika lebih dari 30 menit atau folder hilang
        if(i>=31 || !(opendir(dir)))
        {
            i=1;
            t=time(NULL);
            tm=*localtime(&t);
            memset(dir,0,sizeof(dir));
            strcpy(dir,"/home/bonnis/log/");
            snprintf(wkt, sizeof wkt, "%02d:%02d:%04d-%02d:%02d",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min);
            strcat(dir,wkt);
            if(fork()==0)
            {
                execlp("mkdir","mkdir","-p",dir,NULL);
            }
            else
            {
                while(wait(&status)>0);
            }
        }//Mengopy file log
        if(fork()==0)
        {
            char log[15];
            snprintf(log, sizeof log, "/log%d.log", i);
            strcat(dir,log);
            execlp("cp", "cp", "/var/log/syslog", dir, NULL);
        }
        i++;

        sleep(60);
    }

    exit(EXIT_SUCCESS);
}