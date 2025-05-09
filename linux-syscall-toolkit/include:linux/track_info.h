#ifndef _TRACK_INFO_H
#define _TRACK_INFO_H

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#include <sys/types.h>
#endif

struct track_info {
  pid_t pid;
  pid_t ppid;
  uid_t uid;
  char comm[16];
  long state;
};

struct children_info {
  pid_t parent_pid;
  size_t num_children;
  pid_t children[128];  // Up to 128 children; resize if needed
};

struct user_session_info {
  uid_t uid;
  gid_t gid;
  char username[32];
  char home_dir[128];
  char shell[64];
};

#endif
