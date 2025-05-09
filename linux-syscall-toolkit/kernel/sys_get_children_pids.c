#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/syscalls.h>
#include <linux/track_info.h>
#include <linux/uaccess.h>

SYSCALL_DEFINE1(get_children_pids, struct children_info __user *, user_buf) {
  struct task_struct *parent_task;
  struct task_struct *child;
  struct children_info info = {0};
  size_t count = 0;

  // Copy user-supplied struct to kernel
  if (copy_from_user(&info, user_buf, sizeof(info))) return -EFAULT;

  rcu_read_lock();
  parent_task = find_task_by_vpid(info.parent_pid);
  if (!parent_task) {
    rcu_read_unlock();
    return -ESRCH;
  }

  list_for_each_entry(child, &parent_task->children, sibling) {
    if (count >= 128) break;
    info.children[count++] = child->pid;
  }
  rcu_read_unlock();

  info.num_children = count;

  // Copy the results back to user-space
  if (copy_to_user(user_buf, &info, sizeof(info))) return -EFAULT;

  return 0;
}
