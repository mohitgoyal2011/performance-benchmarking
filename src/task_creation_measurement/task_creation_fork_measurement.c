#define _GNU_SOURCE

#include <stdint.h> /* for uint64_t */
#include <time.h>  /* for struct timespec */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <math.h> 

#define LOOP_ITER 10000
#define LOOP_ITER_DOUBLE 10000.0

#define PERFORMANCE_OVERHEAD 34

/* assembly code to read the TSC */
void main(){
	unsigned long cpuMask;
	cpuMask = 2;
	sched_setaffinity(0,sizeof(cpuMask), &cpuMask);
	int i;
	unsigned int hi1,lo1,hi2,lo2;
	unsigned long begin;
	unsigned long end;
	double mean = 0;
	double variance = 0;
	double std_deviation = 0;
	unsigned long diff_array[LOOP_ITER];
	//Initialization for experiment specific code
	int pid;
	for(i = 0;i<LOOP_ITER;i++){
  		asm volatile ("cpuid\n\t"
		 	      "rdtsc\n\t"
		  	      "mov %%edx, %0\n\t"
		              "mov %%eax, %1\n\t"
		              : "=r" (hi1), "=r" (lo1)
		              :: "%rax", "%rbx", "%rcx", "%rdx"); 
		pid = fork();
  		asm volatile ("rdtscp\n\t"
			  "mov %%edx, %0\n\t"
		  	  "mov %%eax, %1\n\t"
		  	  "cpuid\n\t"
		  	  : "=r" (hi2), "=r" (lo2)
		          :: "%rax", "%rbx", "%rcx", "%rdx");
		if(pid == 0){
			exit(0);
		}		
  		begin = ((uint64_t)hi1 << 32) | lo1;
  		end = ((uint64_t)hi2 << 32) | lo2;
		long result=(end-begin-PERFORMANCE_OVERHEAD);
		if(result<0){
			i--;
			continue;
		}
		diff_array[i]=result;
	}
	for(i = 0;i<LOOP_ITER;i++){
		mean += diff_array[i]/LOOP_ITER_DOUBLE;
	}	
	for(i = 0;i<LOOP_ITER;i++){
		variance += (pow((diff_array[i] - mean),2))/LOOP_ITER_DOUBLE;
	}	
        std_deviation = sqrt(variance);
	printf("\nFork Overhead(Mean) : %lf",mean);
	printf("\nFork Overhead(Variance) : %lf",variance);
	printf("\nFork Overhead(Standard Deviation) : %lf",std_deviation);
		
}

