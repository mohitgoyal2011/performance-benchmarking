#include<stdio.h>
#include<stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define FILESIZE    1048576
#define ITER        10
#define ITER_DOUBLE 10.0

int main() {
    int fd, *addr, value;
    char cmd[100], str[2];
    unsigned long long final_values[10];
    int i = 0, status;
    uint64_t start, end;
    double mean = 0;
    double variance = 0;
    double std_deviation = 0;
    unsigned cycles_high, cycles_low, cycles_high1, cycles_low1;
    while (i < 10) {
	    sprintf(cmd, "dd if=/dev/zero of=%d bs=2M count=2 > /dev/null 2>&1", i);
	    status = system(cmd);
	    sprintf(str, "%d", i);
	    fd = open(str, O_RDONLY);
	    addr = (int *) mmap(0, FILESIZE, PROT_READ, MAP_PRIVATE, fd, 0);
	    asm volatile ("CPUID\n\t"
			    "RDTSC\n\t"
			    "mov %%edx, %0\n\t"
			    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
			    "%rax", "%rbx", "%rcx", "%rdx");
	    value = addr[0];
	    asm volatile("RDTSCP\n\t"
			    "mov %%edx, %0\n\t"
			    "mov %%eax, %1\n\t"
			    "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
			    "%rax", "%rbx", "%rcx", "%rdx");
	    start = ( ((uint64_t)cycles_high << 32) | cycles_low );
	    end = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
	    final_values[i] = end - start;
	    close(fd); 
	    i++;
    }

    for (i = 0; i < 10; i++) {
	sprintf(str, "rm -f %d", i);
	status = system(str);
    }       

    for (i = 0; i < ITER; i++) {
	mean += final_values[i]/ITER_DOUBLE;
    }
    for(i = 0 ; i < ITER; i++) {
	variance += (pow((final_values[i] - mean),2))/ITER_DOUBLE;
    }	
    std_deviation = sqrt(variance);
    printf("\nPage Fault Overhead(Mean) : %lf",mean);
    printf("\nPage Fault Overhead(Variance) : %lf",variance);
    printf("\nPage Fault Overhead(Standard Deviation) : %lf\n",std_deviation);


    return 0;
}

