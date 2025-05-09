#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <pwd.h>
#include <errno.h>

struct track_info {
    pid_t pid;
    pid_t ppid;
    uid_t uid;
    char comm[16];
    long state; // Will use 0 for running, 1 for sleeping, 2 for zombie
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    struct track_info info = {0};
    info.pid = pid;

    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, pid};
    struct kinfo_proc proc;
    size_t size = sizeof(proc);

    if (sysctl(mib, 4, &proc, &size, NULL, 0) != 0 || size == 0) {
        perror("sysctl");
        return 1;
    }

    info.ppid = proc.kp_eproc.e_ppid;
    info.uid  = proc.kp_eproc.e_ucred.cr_uid;
    snprintf(info.comm, sizeof(info.comm), "%s", proc.kp_proc.p_comm);

    switch (proc.kp_proc.p_stat) {
        case SRUN:  info.state = 0; break; // running
        case SSLEEP:info.state = 1; break; // sleeping
        case SZOMB: info.state = 2; break; // zombie
        default:    info.state = -1; break; // unknown
    }

    printf("PID: %d\n", info.pid);
    printf("PPID: %d\n", info.ppid);
    printf("UID: %d\n", info.uid);
    printf("Command: %s\n", info.comm);
    printf("State: %ld\n", info.state);

    return 0;
}
