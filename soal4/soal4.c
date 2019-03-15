#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main() {

  pid_t pid, sid;
  int number;
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

  number=1;
  while(1) {
   char path[] = "/home/bonnis/Documents/makanan/makan_enak.txt";
   struct stat statRes;
   time_t waktu; time(&waktu);
   stat(path, &statRes);

   double selisih = difftime(waktu, statRes.st_atime);
   char tittle[100];

    if (selisih <= 30.0){
      sprintf(tittle, "makan_sehat%d.txt", number);
      if(fork()==0)
      {
        char *argv[] = {"touch", tittle, NULL};
        execv("/usr/bin/touch", argv);
      }
      number++;
    }
    sleep(5);
  }
  
  exit(EXIT_SUCCESS);
}


