#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <pthread.h>
#define LOOP_ITER 100
#define LOOP_ITER_DOUBLE 100.0

    int fd[2], fd1[2], i;
    int nbytes = 0;
void* function(void *args){
        char *readbuffer = (char*) malloc(sizeof(char));
	nbytes = read(fd1[0], readbuffer, sizeof(readbuffer));
	close(fd1[0]);
        write(fd[1], "9", sizeof(char));
	close(fd[1]);
}

int main() {
    unsigned long cpuMask;
    cpuMask = 2;
    sched_setaffinity(0, sizeof(cpuMask), &cpuMask);

    int pid;
    uint64_t start, end, switch_time, rw_time;
    int cycles_low, cycles_low1, cycles_high, cycles_high1;
    double mean, variance, std_deviation;
    long diff_array[LOOP_ITER];
    char *readbuffer = (char*) malloc(sizeof(char));
    pthread_t thread;
    /*  The loop that does everthing  */
    setbuf(stdout,NULL);
    for (i = 0; i < LOOP_ITER; i++) {
    		pipe(fd);
    		pipe(fd1);
		pid = pthread_create(&thread,NULL,function,NULL);	


        /*  Context Switch Overhead  */
        asm volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                "%rax", "%rbx", "%rcx", "%rdx");
        	write(fd1[1], "9", sizeof(char));
		close(fd1[1]);
        	nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
		close(fd[0]);
        asm volatile("RDTSCP\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t"
                "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
                "%rax", "%rbx", "%rcx", "%rdx");  
        start = ( ((uint64_t)cycles_high << 32) | cycles_low );
        end = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
        switch_time = end - start;
	
        
        /*  Read/Write pipe overhead  */

        rw_time = 460;
	pthread_cancel(&thread);	
        diff_array[i] = (switch_time - rw_time)/2;
    }

    /*  Calculating Mean, Variance and Standard deviation 
     *  
     */

    for(i = 0;i<LOOP_ITER;i++){
		mean += diff_array[i]/LOOP_ITER_DOUBLE;
	}	
	for(i = 0;i<LOOP_ITER;i++){
		variance += (pow((diff_array[i] - mean),2))/LOOP_ITER_DOUBLE;
	}	
    std_deviation = sqrt(variance);
	printf("\nContext Switch Kernel thread Overhead(Mean) : %lf",mean);
	printf("\nContext Switch Kernel thread Overhead(Variance) : %lf",variance);
	printf("\nContext Switch Kernel thread Overhead(Standard Deviation) : %lf",std_deviation);
	
    return 0; 
}
