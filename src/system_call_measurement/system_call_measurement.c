/* System Call Measurement - We are using a driver code loop to check the actual results because we saw that there is some sort of caching going on both in the case
of getpid() */
#define _GNU_SOURCE

#include <stdint.h> /* for uint64_t */
#include <time.h>  /* for struct timespec */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <math.h> 
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>

#define PERFORMANCE_OVERHEAD 34


void main(){
	unsigned long cpuMask;
	cpuMask = 2;
	sched_setaffinity(0,sizeof(cpuMask), &cpuMask);
	int i;
	unsigned int hi1,lo1,hi2,lo2;
	unsigned long begin;
	unsigned long end;
	//declare experiment specific variables here
	pid_t pid;
  		asm volatile ("cpuid\n\t"
		 	      "rdtsc\n\t"
		  	      "mov %%edx, %0\n\t"
		              "mov %%eax, %1\n\t"
		              : "=r" (hi1), "=r" (lo1)
		              :: "%rax", "%rbx", "%rcx", "%rdx"); 
	        //Code to measure here
	        pid = syscall(SYS_getpid);
  		asm volatile ("rdtscp\n\t"
			  "mov %%edx, %0\n\t"
		  	  "mov %%eax, %1\n\t"
		  	  "cpuid\n\t"
		  	  : "=r" (hi2), "=r" (lo2)
		          :: "%rax", "%rbx", "%rcx", "%rdx");
  		begin = ((uint64_t)hi1 << 32) | lo1;
  		end = ((uint64_t)hi2 << 32) | lo2;
		printf("\n%lu %lu %lu",begin, end, end-begin-PERFORMANCE_OVERHEAD);
		
}

