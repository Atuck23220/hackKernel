#include <linux/cred.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/track_info.h>
#include <linux/uaccess.h>

SYSCALL_DEFINE1(user_session_info, struct user_session_info __user *,
                user_buf) {
  struct user_session_info info = {0};

  const struct cred *cred = current_cred();  // Get current task's credentials

  info.uid = from_kuid(&init_user_ns, cred->uid);
  info.gid = from_kgid(&init_user_ns, cred->gid);
  //info.pid = current->pid;
  //info.ppid = current->real_parent->pid;
  //get_task_comm(info.comm, current);

  if (copy_to_user(user_buf, &info, sizeof(info))) return -EFAULT;

  return 0;
}
