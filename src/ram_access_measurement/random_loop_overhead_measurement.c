#define _GNU_SOURCE

#include <stdint.h> /* for uint64_t */
#include <time.h>  /* for struct timespec */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <math.h> 

#define LOOP_ITER 1000
#define LOOP_ITER_DOUBLE 1000.0

void main(){
	unsigned long cpuMask;
	cpuMask = 2;
	sched_setaffinity(0,sizeof(cpuMask), &cpuMask);
	int i,k;
	unsigned long j = 0;
	unsigned int hi1,lo1,hi2,lo2;
	unsigned long begin;
	unsigned long end;
	double mean = 0;
	double variance = 0;
	double std_deviation = 0;
	unsigned long diff_array[LOOP_ITER];
        //Initializaiton specific to the experiment
	int memsize = 1024;
	for(i = 0;i<LOOP_ITER;i++){
  		asm volatile ("cpuid\n\t"
		 	      "rdtsc\n\t"
		  	      "mov %%edx, %0\n\t"
		              "mov %%eax, %1\n\t"
		              : "=r" (hi1), "=r" (lo1)
		              :: "%rax", "%rbx", "%rcx", "%rdx"); 
		
		for(k = 0;k<LOOP_ITER;k++){
	        	j = rand()*rand();
			j = j%(memsize); //Since the maximum number memsize can be is 256x1024x1024, we have to ensure that we cover the maximum range that is 32000x32000 > 256x1024x1024. This way it will cover all elements. If just rand() is used, we will only be accessing the first 32000 places
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
	printf("\nMeasurement Overhead(Mean) : %lf",mean);
	printf("\nMeasurement Overhead(Variance) : %lf",variance);
	printf("\nMeasurement Overhead(Standard Deviation) : %lf",std_deviation);
		
}

