// user_session_test.c
#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

struct user_session_info {
  uid_t uid;
  gid_t gid;
  char username[32];
  char home_dir[128];
  char shell[64];
};

int main() {
  struct user_session_info info;

  // Call syscall 453 to fill in UID and GID
  long res = syscall(453, &info);
  if (res != 0) {
    perror("syscall user_session_info failed");
    return 1;
  }

  // Get additional info from /etc/passwd
  struct passwd *pw = getpwuid(info.uid);
  if (pw) {
    strncpy(info.username, pw->pw_name, sizeof(info.username) - 1);
    strncpy(info.home_dir, pw->pw_dir, sizeof(info.home_dir) - 1);
    strncpy(info.shell, pw->pw_shell, sizeof(info.shell) - 1);
  } else {
    strncpy(info.username, "unknown", sizeof(info.username));
    strncpy(info.home_dir, "/home/unknown", sizeof(info.home_dir));
    strncpy(info.shell, "/bin/sh", sizeof(info.shell));
  }

  // Output results
  printf("UID: %d\n", info.uid);
  printf("GID: %d\n", info.gid);
  printf("Username: %s\n", info.username);
  printf("Home Directory: %s\n", info.home_dir);
  printf("Shell: %s\n", info.shell);

  return 0;
}
