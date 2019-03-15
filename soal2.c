#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <grp.h>
#include <pwd.h>

int main() {
  pid_t pid, sid;

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
    // main program here
    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id == 0){

      char *argv[4] = {"chmod", "777", "elen.ku", NULL};
      execv("/bin/chmod", argv);
    }
    else{
      while ((wait(&status))>0);

      char path[] = "/home/nitama/shift2/hatiku/elen.ku";
      struct stat statRes;

      stat(path, &statRes);
      struct group *grp;
      struct passwd *pwd;

      grp = getgrgid(statRes.st_uid);
      pwd = getpwuid(statRes.st_gid);

      if(strcmp(grp->gr_name, "www-data")==0 && strcmp(pwd->pw_name, "www-data"$
        remove("/home/nitama/shift2/hatiku/elen.ku");
      }
    }
    sleep(3);
  }
  exit(EXIT_SUCCESS);
}
