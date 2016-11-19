#define _GNU_SOURCE

#include <stdint.h> /* for uint64_t */
#include <time.h>  /* for struct timespec */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <math.h> 

#define LOOP_ITER 100
#define LOOP_ITER_DOUBLE 100.0

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
	int sum = 0;
	unsigned long diff_array[LOOP_ITER];
	//initialization specific to experiment
	long long j = 0;
	for(i = 0;i<LOOP_ITER;i++){
  		asm volatile ("cpuid\n\t"
		 	      "rdtsc\n\t"
		  	      "mov %%edx, %0\n\t"
		              "mov %%eax, %1\n\t"
		              : "=r" (hi1), "=r" (lo1)
  		              :: "%rax", "%rbx", "%rcx", "%rdx"); 
		for(j = 0;j<1<<25;j++){
		    sum+=j;	
		    sum+=j;	
		    sum+=j;	
		    sum+=j;	
		}
  		asm volatile ("rdtscp\n\t"
			  "mov %%edx, %0\n\t"
		  	  "mov %%eax, %1\n\t"
		  	  "cpuid\n\t"
		  	  : "=r" (hi2), "=r" (lo2)
		          :: "%rax", "%rbx", "%rcx", "%rdx");
  		begin = ((uint64_t)hi1 << 32) | lo1;
  		end = ((uint64_t)hi2 << 32) | lo2;
		diff_array[i]=(end-begin);
	}
	for(i = 0;i<LOOP_ITER;i++){
		mean += diff_array[i]/LOOP_ITER_DOUBLE;
	}	
	for(i = 0;i<LOOP_ITER;i++){
		variance += (pow((diff_array[i] - mean),2))/LOOP_ITER_DOUBLE;
	}	
        std_deviation = sqrt(variance);
	printf("\nLoop Overhead(Mean) : %lf",mean);
	printf("\nLoop Overhead(Variance) : %lf",variance);
	printf("\nLoop Overhead(Standard Deviation) : %lf",std_deviation);
		
}

