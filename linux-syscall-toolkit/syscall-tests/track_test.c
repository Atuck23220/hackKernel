#include <stdio.h>           // for printf
#include <unistd.h>          // for syscall()
#include <sys/syscall.h>     // for syscall number definitions
#include "track_info.h"      // our shared struct

#define SYS_track_process_info 451  

int main() {
    struct track_info info;

    // Set PID to track; use getpid() for process
    info.pid = getpid();

    // Call the syscall
    int result = syscall(SYS_track_process_info, &info);

    // Check the result
    if (result == 0) {
        printf("PID   : %d\n", info.pid);
        printf("PPID  : %d\n", info.ppid);
        printf("UID   : %d\n", info.uid);
        printf("COMM  : %s\n", info.comm);
        printf("STATE : %ld\n", info.state);
    } else {
        perror("Syscall failed");
    }

    return 0;
}
