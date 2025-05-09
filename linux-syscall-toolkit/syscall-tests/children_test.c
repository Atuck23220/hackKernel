// children_test.c
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

struct children_info {
  pid_t parent_pid;
  size_t num_children;
  pid_t children[128];
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <parent_pid>\n", argv[0]);
    return 1;
  }

  pid_t target_pid = atoi(argv[1]);
  struct children_info info;
  memset(&info, 0, sizeof(info));
  info.parent_pid = target_pid;

  long res = syscall(452, &info);
  if (res != 0) {
    perror("syscall get_children_pids failed");
    return 1;
  }

  printf("Parent PID: %d\n", info.parent_pid);
  printf("Number of children: %zu\n", info.num_children);
  for (size_t i = 0; i < info.num_children; i++) {
    printf("Child PID: %d\n", info.children[i]);
  }

  return 0;
}
