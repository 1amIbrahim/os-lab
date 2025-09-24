# IPC Performance Test Results

## Test Configuration
- **Number of Messages**: 1,000,000
- **Message Size**: 16 bytes each
- **Test Iterations**: 3 runs per IPC method
- **System**: Linux (Virtual Machine)

## Performance Results

| IPC Method | Avg Time (seconds) | Messages/sec | Performance Rank |
|------------|-------------------|--------------|------------------|
| **Shared Memory** | 0.34 | 2,918,370 | 🥇 **1st** (Fastest) |
| **Anonymous Pipes** | 0.73 | 1,369,634 | 🥈 **2nd** |
| **TCP Sockets** | 1.57 | 635,708 | 🥉 **3rd** |
| **Named Pipes (FIFO)** | 1.87 | 533,869 | **4th** |
| **UNIX Domain Sockets** | 2.13 | 469,664 | **5th** |
| **Message Queues** | 7.95 | 125,755 | **6th** |
| **UDP Sockets** | ⚠️ Timeout | ⚠️ Timeout | **7th** (Issues) |

## Analysis

### 🏆 **Best Performers**

1. **Shared Memory** - Fastest for high-volume data transfer
   - Direct memory access with minimal kernel overhead
   - Ideal for producer-consumer scenarios with large data volumes
   - Requires careful synchronization (semaphores/mutexes)

2. **Anonymous Pipes** - Excellent for parent-child communication
   - Very efficient for related processes
   - Simple API, kernel-level buffering
   - Perfect for shell-like process chains

### 🔄 **Good Performers**

3. **TCP Sockets** - Reliable but with network overhead
   - Connection-oriented, reliable delivery
   - Works across network boundaries
   - Good for client-server applications

4. **Named Pipes (FIFO)** - Good for unrelated processes
   - Persistent in filesystem
   - Works between unrelated processes
   - Slightly slower than anonymous pipes due to filesystem overhead

5. **UNIX Domain Sockets** - Local socket communication
   - More overhead than pipes but more flexible
   - Supports advanced features (credentials passing, etc.)
   - Good for local client-server communication

### 📉 **Slower Methods**

6. **Message Queues** - Feature-rich but slower
   - Persistent, priority-based messaging
   - Higher overhead due to POSIX compliance features
   - Best for reliable, asynchronous messaging

7. **UDP Sockets** - Performance issues observed
   - Connectionless, unreliable delivery
   - May have buffer overflow issues with high-volume tests
   - Better suited for sporadic, small message transfers

## Recommendations

### For High-Performance Applications:
- **Use Shared Memory** for maximum throughput
- **Use Anonymous Pipes** for simple parent-child communication
- Combine with proper synchronization mechanisms

### For General Applications:
- **TCP Sockets** for network communication
- **Named Pipes** for local inter-process communication
- **UNIX Domain Sockets** for local client-server patterns

### For Reliable Messaging:
- **Message Queues** when you need persistence and reliability
- Consider the performance trade-off vs. features

## Notes
- Times include process startup and synchronization overhead
- Shared memory requires careful synchronization design
- Results may vary based on system load and configuration
- UDP performance issues may be due to buffer limitations with high-volume transfers

## Build Instructions
```bash
cd /home/bsai23046/Documents/lab3/ipc/fwdlab2/build
cmake ..
make -j4
./performance_test.sh
```