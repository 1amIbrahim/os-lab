# OS Lab 2 — IPC and Networking Demos

This folder contains small, self-contained C programs demonstrating common IPC and socket primitives.

All files compile independently. The commands below assume Linux with GCC and glibc. Install build tools:

```bash
sudo apt update && sudo apt install -y build-essential
```

Note: Some examples require extra dev packages on Debian/Ubuntu:
- POSIX message queues: `sudo apt install -y libposix-mq-dev` (on newer distros headers are in glibc)

If you are on WSL, use a Linux distro (Ubuntu) shell, not Windows CMD/PowerShell.

## Build and Run with CMake (recommended)
Install CMake if you don’t have it:
```bash
sudo apt install -y cmake
```

Configure and build all demos:
```bash
mkdir -p build
cd build
cmake ..
cmake --build . -j
```
This produces one executable per demo in the `build/` directory:
- pipe
- writer, reader
- unix_server, unix_client
- tcp_server, tcp_client
- udp_server, udp_client
- shm_writer, shm_reader
- mq_writer, mq_reader
- signal_demo

Run examples (from inside `build/`):
- Anonymous pipe:
```bash
./pipe
```
- FIFO (two terminals):
```bash
mkfifo myfifo
# Terminal 1
./reader
# Terminal 2
./writer
```
- UNIX domain sockets (two terminals):
```bash
# Terminal 1
./unix_server
# Terminal 2
./unix_client
```
- TCP sockets (two terminals):
```bash
# Terminal 1
./tcp_server
# Terminal 2
./tcp_client
```
- UDP sockets (two terminals):
```bash
# Terminal 1
./udp_server
# Terminal 2
./udp_client
```
- POSIX shared memory + semaphore (two terminals):
```bash
# Terminal 1
./shm_reader
# Terminal 2
./shm_writer
```
- POSIX message queues (two terminals):
```bash
# Terminal 1
./mq_reader
# Terminal 2
./mq_writer
```
- Signals (two terminals):
```bash
./signal_demo
# In another terminal, send a signal to the printed PID:
kill -SIGUSR1 <pid>
```

Clean build artifacts:
```bash
cmake --build . --target clean
cd .. && rm -rf build
```

Troubleshooting:
- Some systems require `-lrt` for POSIX mq/shm. The CMake config links it if available.
- If `/dev/mqueue` is missing for message queues: `sudo mount -t mqueue none /dev/mqueue` (may require systemd-based environment).
- If you see permissions issues with shared memory, you can manually remove leftover objects: `sudo rm -f /dev/shm/myshm`.

---

## 1) Anonymous Pipes
File: `pipe.c`
```bash
gcc -o pipe pipe.c
./pipe
```

## 2) Named Pipes (FIFOs)
Files: `writer.c`, `reader.c`
```bash
mkfifo myfifo
# Terminal 1
gcc -o reader reader.c
./reader
# Terminal 2
gcc -o writer writer.c
./writer
```
Cleanup:
```bash
rm -f myfifo
```

## 3) UNIX Domain Sockets
Files: `unix_server.c`, `unix_client.c`
```bash
# Terminal 1
gcc -o unix_server unix_server.c
./unix_server
# Terminal 2
gcc -o unix_client unix_client.c
./unix_client
```
Cleanup:
```bash
rm -f unix_sock
```

## 4) TCP Sockets (localhost:8080)
Files: `tcp_server.c`, `tcp_client.c`
```bash
# Terminal 1
gcc -o tcp_server tcp_server.c
./tcp_server
# Terminal 2
gcc -o tcp_client tcp_client.c
./tcp_client
```

## 5) UDP Sockets (localhost:9090)
Files: `udp_server.c`, `udp_client.c`
```bash
# Terminal 1
gcc -o udp_server udp_server.c
./udp_server
# Terminal 2
gcc -o udp_client udp_client.c
./udp_client
```

## 6) POSIX Shared Memory + Semaphore
Files: `shm_writer.c`, `shm_reader.c`
```bash
gcc -o shm_writer shm_writer.c -pthread -lrt
gcc -o shm_reader shm_reader.c -pthread -lrt
# Terminal 1
./shm_reader
# Terminal 2
./shm_writer
```
Cleanup (if needed, run as root if permissions block):
```bash
rm -f /dev/shm/myshm
# Or use the shm_unlink/mq_unlink alternatives below.
```

## 7) POSIX Message Queues
Files: `mq_writer.c`, `mq_reader.c`
```bash
gcc -o mq_writer mq_writer.c -lrt
gcc -o mq_reader mq_reader.c -lrt
# Terminal 1
./mq_reader
# Terminal 2
./mq_writer
```
Cleanup:
```bash
sudo rm -f /dev/mqueue/myq
```

## 8) Signals
File: `signal_demo.c`
```bash
gcc -o signal_demo signal_demo.c
./signal_demo
# In another terminal, send a signal to the printed PID:
kill -SIGUSR1 <pid>
```

---

Notes:
- Many of these demos omit robust error handling for brevity.
- On some systems you may need to link with `-lrt` for POSIX mq/shm; on newer glibc, it may be unnecessary.
- For production-grade code, check return values and handle errors.
