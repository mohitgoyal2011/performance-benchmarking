#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#define PERFORMANCE_OVERHEAD 34
#define LOOP_OVERHEAD 663442441
int main(){
    const int LEN = 1 << 28;    //  256MB

    int *a = (int*)malloc(LEN*sizeof(int)); //1 GB
    unsigned int hi1,lo1,hi2,lo2;
    unsigned int begin, end;
    unsigned long long i = 0;
    //To prevent cache line prefetching and other optimization, initializing each of 8 parts of allocated memory into different regions
    memset(a,1,LEN*sizeof(int)/8);
    memset(a+((1<<25)-1),2,LEN*sizeof(int)/8);
    memset(a+(2*(1<<25)-1),3,LEN*sizeof(int)/8);
    memset(a+(3*(1<<25)-1),4,LEN*sizeof(int)/8);
    memset(a+(4*(1<<25)-1),5,LEN*sizeof(int)/8);
    memset(a+(5*(1<<25)-1),6,LEN*sizeof(int)/8);
    memset(a+(6*(1<<25)-1),7,LEN*sizeof(int)/8);
    memset(a+(7*(1<<25)-1),8,LEN*sizeof(int)/8);
    long long arr[9];
    arr[0] = 0;
    long long store_values[8];
    for(i = 1;i<=8;i++){
	arr[i] = i*(1<<25);
    }
   int sum = 0;
	asm volatile ("cpuid\n\t"
		      "rdtsc\n\t"
		      "mov %%edx, %0\n\t"
		      "mov %%eax, %1\n\t"
		      : "=r" (hi1), "=r" (lo1)
		      :: "%rax", "%rbx", "%rcx", "%rdx"); 
/*33554432
67108864
100663296
134217728
167772160
201326592
234881024
268435456*/
     
    for (i = 0; i < 1<<25; i++){ //reading 1 GB in the loop 1024x256x1024x4B. Sizeof(int) is 4B. Assuming arr is in cache. So read is negligible
	sum +=a[i + 0]+a[268435456-1 - i];
	sum +=a[i + 33554432]+a[234881024-1 - i];
	sum +=a[i + 67108864]+a[201326592-1 - i];
	sum +=a[i + 100663296]+a[167772160-1 - i];
    }
	asm volatile ("rdtscp\n\t"
		  "mov %%edx, %0\n\t"
		  "mov %%eax, %1\n\t"
		  "cpuid\n\t"
		  : "=r" (hi2), "=r" (lo2)
		  :: "%rax", "%rbx", "%rcx", "%rdx");
	begin = (uint64_t)hi1 << 32 | lo1;
	end = (uint64_t)hi2 << 32 | lo2;
	
    printf("\n%lu", (end - begin - LOOP_OVERHEAD - PERFORMANCE_OVERHEAD));
    free(a);
}
