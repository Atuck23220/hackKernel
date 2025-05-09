#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/syscalls.h>
#include <linux/track_info.h>
#include <linux/uaccess.h>

SYSCALL_DEFINE1(track_process_info, struct track_info __user *, user_buf) {
  struct task_struct *task;
  struct track_info info;

  // Copy user input (contains pid to query)
  if (copy_from_user(&info, user_buf, sizeof(info))) return -EFAULT;

  // Safely look up task by PID
  rcu_read_lock();
  task = find_task_by_vpid(info.pid);
  if (!task) {
    rcu_read_unlock();
    return -ESRCH;
  }

  get_task_struct(task);
  rcu_read_unlock();

  // Populate output fields
  info.ppid = task->real_parent->pid;
  info.uid = from_kuid(&init_user_ns, task_uid(task));
  get_task_comm(info.comm, task);

  // Handle state using safe fallback
  if (task->exit_state)
    info.state = task->exit_state;
  else
    info.state = task->__state;

  put_task_struct(task);

  // Return results to user
  if (copy_to_user(user_buf, &info, sizeof(info))) return -EFAULT;

  return 0;
}
