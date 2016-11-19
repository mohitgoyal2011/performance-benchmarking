#define _GNU_SOURCE
#include <stdint.h> /* for uint64_t */
#include <time.h>  /* for struct timespec */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>

#define LOOP_ITER 1000
#define LOOP_ITER_DOUBLE 1000.0
 
void main(int args, char *argv[]){
	unsigned long cpuMask;
	cpuMask = 2;
	sched_setaffinity(0,sizeof(cpuMask), &cpuMask);
	srand(time(NULL));
	int memsize = atoi(argv[1]);
	unsigned int hi1,lo1,hi2,lo2;
	unsigned long begin;
	unsigned long end;
	unsigned long loop_overhead;
	long i = 0;
	unsigned long j = 0;
	int* strarr = (int*)malloc(memsize*sizeof(int));
	for(i = 0;i<memsize;i++){
		strarr[i] = rand();
	}

  	//To measure the loop and rand function call overhead
	asm volatile ("cpuid\n\t"
		  "rdtscp\n\t"
		  "mov %%edx, %0\n\t"
		  "mov %%eax, %1\n\t"
		  : "=r" (hi1), "=r" (lo1)
		  :: "%rax", "%rbx", "%rcx", "%rdx"); 
	for(i = 0;i<LOOP_ITER;i++){
	        j = rand()*rand();
		j = j%(memsize);
	}
  	asm volatile ("rdtscp\n\t"
		  "mov %%edx, %0\n\t"
		  "mov %%eax, %1\n\t"
		  "cpuid\n\t"
		  : "=r" (hi2), "=r" (lo2)
		  :: "%rax", "%rbx", "%rcx", "%rdx");
  	begin = ((uint64_t)hi1 << 32) | lo1;
  	end = ((uint64_t)hi2 << 32) | lo2;
  	loop_overhead = end-begin;

		
	asm volatile ("cpuid\n\t"
		  "rdtscp\n\t"
		  "mov %%edx, %0\n\t"
		  "mov %%eax, %1\n\t"
		  : "=r" (hi1), "=r" (lo1)
		  :: "%rax", "%rbx", "%rcx", "%rdx"); 
	for(i = 0;i<LOOP_ITER;i++){
		j = rand()*rand();
		j = j%(memsize);
		int a = strarr[j];
	}
  	asm volatile ("rdtscp\n\t"
		  "mov %%edx, %0\n\t"
		  "mov %%eax, %1\n\t"
		  "cpuid\n\t"
		  : "=r" (hi2), "=r" (lo2)
		  :: "%rax", "%rbx", "%rcx", "%rdx");
  	begin = ((uint64_t)hi1 << 32) | lo1;
        end = ((uint64_t)hi2 << 32) | lo2;
	double result = end - begin - loop_overhead;
	
        printf("\n%lu %lu %lf", begin, end, result/LOOP_ITER_DOUBLE );
	free(strarr);	

}

