/* -*- Mode: C; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#include "util.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define ITERATIONS 1000000

int main(void) {
  struct timeval start, end;
  double elapsed;
  
  // 测试getpid系统调用性能
  gettimeofday(&start, NULL);
  for (int i = 0; i < ITERATIONS; i++) {
    pid_t pid = getpid();
    (void)pid; // 避免编译器优化
  }
  gettimeofday(&end, NULL);
  
  elapsed = (end.tv_sec - start.tv_sec) * 1000000.0 + 
            (end.tv_usec - start.tv_usec);
  atomic_printf("getpid: %d calls in %.2f ms (%.2f calls/sec)\n", 
         ITERATIONS, elapsed/1000.0, ITERATIONS/(elapsed/1000000.0));
  
  // 测试gettimeofday系统调用性能
  gettimeofday(&start, NULL);
  for (int i = 0; i < ITERATIONS; i++) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    (void)tv; // 避免编译器优化
  }
  gettimeofday(&end, NULL);
  
  elapsed = (end.tv_sec - start.tv_sec) * 1000000.0 + 
            (end.tv_usec - start.tv_usec);
  atomic_printf("gettimeofday: %d calls in %.2f ms (%.2f calls/sec)\n", 
         ITERATIONS, elapsed/1000.0, ITERATIONS/(elapsed/1000000.0));
  
  // 测试write系统调用性能
  gettimeofday(&start, NULL);
  for (int i = 0; i < ITERATIONS/10; i++) {
    write(STDOUT_FILENO, "", 0); // 空写操作
  }
  gettimeofday(&end, NULL);
  
  elapsed = (end.tv_sec - start.tv_sec) * 1000000.0 + 
            (end.tv_usec - start.tv_usec);
  atomic_printf("write: %d calls in %.2f ms (%.2f calls/sec)\n", 
         ITERATIONS/10, elapsed/1000.0, (ITERATIONS/10)/(elapsed/1000000.0));
  
  // 测试mmap/munmap系统调用性能
  gettimeofday(&start, NULL);
  for (int i = 0; i < ITERATIONS/100; i++) {
    void* ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, 
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr != MAP_FAILED) {
      munmap(ptr, 4096);
    }
  }
  gettimeofday(&end, NULL);
  
  elapsed = (end.tv_sec - start.tv_sec) * 1000000.0 + 
            (end.tv_usec - start.tv_usec);
  atomic_printf("mmap/munmap: %d calls in %.2f ms (%.2f calls/sec)\n", 
         ITERATIONS/100, elapsed/1000.0, (ITERATIONS/100)/(elapsed/1000000.0));
  
  atomic_puts("EXIT-SUCCESS");
  return 0;
} 