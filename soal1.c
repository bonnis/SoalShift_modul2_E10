#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>

int main() {
  pid_t pid, sid, child_id;;
  int status;
  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir(".")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    DIR* dir = opendir("/home/bonnis/modul2/gambar");
    if (dir)
    {
        /* Directory exists. */
        closedir(dir);
    }
    else
    {
        child_id=fork();
        if (child_id == 0) {
            // this is child

        char *argv[4] = {"mkdir", "-p", "/home/bonnis/modul2/gambar", NULL};
        execv("/bin/mkdir", argv);

        } else {
            // this is parent

            // the parent waits for all the child processes
            while ((wait(&status)) > 0);
        }
    }
    char* ptr;
    char old[125];
    char new[125];
    struct dirent *ent;
    dir=opendir(".");
    if (dir != NULL) {
        while((ent = readdir(dir)) != NULL) {
                memset(old,0,sizeof old);
                memset(new,0,sizeof new);
                strcpy(old,ent->d_name);
                ptr = strstr(&old[strlen(old)-5],".png");
                if (ptr != NULL) {
                    strncpy(new,old,(strlen(old)-4));
                    strcat(new,"_grey.png");
                    rename(old,new);
                    child_id=fork();
                    if (child_id == 0) 
                    {
                        // this is child
                        char *argv[4] = {"mv", new, "/home/bonnis/modul2/gambar", NULL};
                        execv("/bin/mv", argv);
                    } 
                    else 
                    {
                        while ((wait(&status)) > 0);
                    }
                } else {
                    continue;
                }
        }
            closedir(dir);
    }
    sleep(15);
  }
  
  exit(EXIT_SUCCESS);
}