#!/bin/bash

# Performance test script for IPC mechanisms
# Tests each IPC method by sending 1 million messages and measuring time

BUILD_DIR="/home/bsai23046/Documents/lab3/ipc/fwdlab2/build"
cd "$BUILD_DIR"

echo "=== IPC Performance Test ==="
echo "Testing each IPC mechanism with 1,000,000 message transfers"
echo "Message size: 16 bytes"
echo "=============================================="

# Function to run test and calculate statistics
run_test() {
    local test_name="$1"
    local command="$2"
    local iterations=3  # Run each test 3 times for better averaging
    
    echo ""
    echo "Testing: $test_name"
    echo "Running $iterations iterations..."
    
    local total_time=0
    local times=()
    local successful=0
    
    for i in $(seq 1 $iterations); do
        echo -n "  Run $i: "
        
        # Use timeout to prevent hanging tests
        local start_time=$(date +%s.%N)
        timeout 120s bash -c "$command" > /tmp/test_output_$i.log 2>&1
        local exit_code=$?
        local end_time=$(date +%s.%N)
        
        if [ $exit_code -eq 124 ]; then
            echo "TIMEOUT (>120s)"
            continue
        elif [ $exit_code -ne 0 ]; then
            echo "FAILED (exit code: $exit_code)"
            continue
        fi
        
        local duration=$(echo "$end_time - $start_time" | bc -l)
        times+=($duration)
        total_time=$(echo "$total_time + $duration" | bc -l)
        successful=$((successful + 1))
        echo "${duration}s"
    done
    
    if [ $successful -gt 0 ]; then
        local avg_time=$(echo "scale=6; $total_time / $successful" | bc -l)
        local msg_per_sec=$(echo "scale=2; 1000000 / $avg_time" | bc -l)
        echo "  Average time: ${avg_time}s"
        echo "  Messages/sec: $msg_per_sec"
        echo "  Successful runs: $successful/$iterations"
        
        # Show sample output from last successful run
        if [ -f "/tmp/test_output_$iterations.log" ]; then
            echo "  Sample output:"
            tail -n 5 "/tmp/test_output_$iterations.log" | sed 's/^/    /'
        fi
    else
        echo "  All runs failed!"
    fi
    
    # Clean up log files
    rm -f /tmp/test_output_*.log
}

# Test 1: Anonymous Pipes
run_test "Anonymous Pipes" "./pipe_perf"

# Test 2: Named Pipes (FIFOs)
run_test "Named Pipes (FIFOs)" "./writer_perf & WRITER_PID=\$!; sleep 0.5; ./reader_perf; wait \$WRITER_PID"

# Test 3: UNIX Domain Sockets
run_test "UNIX Domain Sockets" "rm -f /tmp/unix_sock_perf; ./unix_server_perf & SERVER_PID=\$!; sleep 0.5; ./unix_client_perf; wait \$SERVER_PID; rm -f /tmp/unix_sock_perf"

# Test 4: TCP Sockets
run_test "TCP Sockets" "./tcp_server_perf & SERVER_PID=\$!; sleep 0.5; ./tcp_client_perf; wait \$SERVER_PID"

# Test 5: UDP Sockets  
run_test "UDP Sockets" "./udp_server_perf & SERVER_PID=\$!; sleep 0.5; ./udp_client_perf; wait \$SERVER_PID"

# Test 6: Shared Memory
run_test "Shared Memory" "./shm_writer_perf & WRITER_PID=\$!; sleep 0.2; ./shm_reader_perf; wait \$WRITER_PID"

# Test 7: Message Queues
run_test "Message Queues" "./mq_writer_perf & WRITER_PID=\$!; sleep 0.2; ./mq_reader_perf; wait \$WRITER_PID"

echo ""
echo "=== Performance Test Complete ==="
echo "Note: Times include process startup overhead and synchronization"
echo "For pure IPC performance, consider the relative differences between methods"
echo "Shared Memory and Message Queues typically perform best for high-volume transfers"