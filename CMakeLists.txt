cmake_minimum_required(VERSION 3.15)
project(OSLab2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Prefer warnings for better learning (can be adjusted as needed)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Optional libraries (glibc versions vary):
find_package(Threads)
find_library(RT_LIBRARY rt)

function(link_optional_rt target)
  if(RT_LIBRARY)
    target_link_libraries(${target} PRIVATE ${RT_LIBRARY})
  endif()
endfunction()

function(link_optional_threads target)
  if(Threads_FOUND)
    target_link_libraries(${target} PRIVATE Threads::Threads)
  endif()
endfunction()

# 1) Anonymous Pipes
add_executable(pipe pipe.c)

# 2) Named Pipes (FIFOs)
add_executable(writer writer.c)
add_executable(reader reader.c)

# 3) UNIX Domain Sockets
add_executable(unix_server unix_server.c)
add_executable(unix_client unix_client.c)

# 4) TCP Sockets
add_executable(tcp_server tcp_server.c)
add_executable(tcp_client tcp_client.c)

# 5) UDP Sockets
add_executable(udp_server udp_server.c)
add_executable(udp_client udp_client.c)

# 6) POSIX Shared Memory + Semaphore
add_executable(shm_writer shm_writer.c)
add_executable(shm_reader shm_reader.c)
link_optional_threads(shm_writer)
link_optional_threads(shm_reader)
link_optional_rt(shm_writer)
link_optional_rt(shm_reader)

# 7) POSIX Message Queues
add_executable(mq_writer mq_writer.c)
add_executable(mq_reader mq_reader.c)
link_optional_rt(mq_writer)
link_optional_rt(mq_reader)

# 8) Signals
add_executable(signal_demo signal_demo.c)

# Performance Test Executables
add_executable(pipe_perf pipe_perf.c)

add_executable(writer_perf writer_perf.c)
add_executable(reader_perf reader_perf.c)

add_executable(unix_server_perf unix_server_perf.c)
add_executable(unix_client_perf unix_client_perf.c)

add_executable(tcp_server_perf tcp_server_perf.c)
add_executable(tcp_client_perf tcp_client_perf.c)

add_executable(udp_server_perf udp_server_perf.c)
add_executable(udp_client_perf udp_client_perf.c)

add_executable(shm_writer_perf shm_writer_perf.c)
add_executable(shm_reader_perf shm_reader_perf.c)
link_optional_threads(shm_writer_perf)
link_optional_threads(shm_reader_perf)
link_optional_rt(shm_writer_perf)
link_optional_rt(shm_reader_perf)

add_executable(mq_writer_perf mq_writer_perf.c)
add_executable(mq_reader_perf mq_reader_perf.c)
link_optional_rt(mq_writer_perf)
link_optional_rt(mq_reader_perf)
