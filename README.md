# linux-syscall-toolkit  
_A custom syscall + system info toolkit for Linux, with optional macOS analogs_

This toolkit demonstrates **custom Linux syscalls** alongside **cross-platform macOS equivalents**. Designed as a hands-on systems programming portfolio piece, it bridges kernel development and user-space system inspection.

---

## 🎯 Purpose
This project explores the **real-world mechanics of system introspection** across platforms. It pairs **custom Linux kernel syscalls** with **macOS user-space tools** to build a lightweight toolkit for inspecting process behavior and user session state.

- Hands-on Linux syscall development
- End-to-end syscall wiring and user-space testing
- Platform-aware tooling for both Linux and macOS
- Offers a consistent interface to system information for debugging and monitoring

From retrieving child processes of a parent PID to inspecting the currently logged-in user's session, this toolkit demonstrates that **OS-specific tools can still speak a common systems language** — and that user identity, session scope, and process state remain critical foundations of low-level systems insight.

---

## 🐧 Custom Linux Syscalls
We implemented **four syscalls** in a custom-built Linux kernel:

### 1. `aaron_tuck` — syscall 450
- A basic syscall that returns 0
- Used to test syscall plumbing and kernel rebuild

### 2. `track_process_info` — syscall 451
- Retrieves info for a given PID:
  - PID, PPID
  - UID
  - Command name
  - Process state (running/sleeping/zombie)

### 3. `get_children_pids` — syscall 452
- Returns a list of children PIDs for a given parent
- Capped at 128 child processes

### 4. `user_session_info` — syscall 453
- Returns current user UID, GID, and session details:
  - Username
  - Home directory
  - Default shell

Each syscall was added to:
- `syscalls.h`
- `syscall_64.tbl`
- A dedicated `.c` file in `kernel/`
- A shared header: `include/linux/track_info.h`

✅ Kernel rebuilt cleanly and booted as `aarontuck_v2`

---

## 📁 Toolkit Structure (Linux)
```
hackKernel/
├── syscall-export/                # Exported kernel syscall source files
│   ├── sys_aaron_tuck.c
│   ├── sys_track_process.c
│   ├── sys_get_children_pids.c
│   ├── sys_user_session_info.c
├── syscall-tests/                # User-space syscall test programs
│   ├── track_test.c
│   ├── children_test.c
│   ├── user_session_test.c
├── include/linux/track_info.h    # Shared struct definitions
```

### 🛠️ Build Instructions (Linux)
```bash
cd ~/my-kernel
make -j$(nproc)
sudo make modules_install
sudo make install
```
Then reboot into the new kernel (`aarontuck_v2`).

Compile test files:
```bash
gcc -o track_test track_test.c
gcc -o children_test children_test.c
gcc -o user_session_test user_session_test.c
```

---

## 🍎 macOS Companion Tools (Optional)
These user-space tools mimic Linux syscall behavior using macOS system APIs:

### `proc_info.c`
- Uses `sysctl()` to get PID info
- Mimics `track_process_info`

### `user_session.c`
- Uses `getuid()`, `getpwuid()`, and `getenv()` to retrieve:
  - UID, username
  - Home directory
  - Shell
- Mimics `user_session_info`

```
macProcInfoTool/
├── proc_info.c
├── user_session.c
```

Compile with:
```bash
clang proc_info.c -o proc_info
clang user_session.c -o user_session
```

---

## 🧪 Example Usage (Linux)
```bash
./track_test 1234
./children_test 1
./user_session_test
```

## 🧪 Example Usage (macOS)
```bash
./proc_info 1234
./user_session
```

---

## 💡 Future Enhancements
- Add logging kernel module
- Track file descriptors or open files per PID
- Group/supplement session info (e.g., groups, login type)
