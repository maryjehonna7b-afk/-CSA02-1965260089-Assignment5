/* B1: Complete TODO; instrument recursive calls */
#include <stdio.h>
#include <stdlib.h>
long long calls;
long long power_linear(long long base,int exp) {
    ++calls;
    if (exp==0) return 1;
    return base*power_linear(base,exp-1);
}
long long power_fast(long long base,int exp) {
    ++calls;
    if (exp==0) return 1;
    long long half=power_fast(base,exp/2); /* compute ONCE */
    return (exp%2==0) ? half*half : base*half*half;
}
long long power_iterative(long long base,unsigned int exp) {
    long long result=1;
    while (exp>0) {
        if (exp&1U) result*=base;
        base*=base;
        exp>>=1U;
    }
    return result;
}
int digital_root(long long n) {
    if (n<0) n=-n;
    if (n<10) return (int)n;
    long long sum=0;
    while (n>0) { sum+=n%10; n/=10; }
    return digital_root(sum);
}
int main(void) {
    calls=0; long long a=power_linear(2,30); long long lc=calls;
    calls=0; long long b=power_fast(2,30); long long fc=calls;
    printf("linear: %lld in %lld calls\n",a,lc);
    printf("fast : %lld in %lld calls\n",b,fc);
    printf("iterative: %lld\n",power_iterative(2,30));
    printf("digital root of 9875: %d\n",digital_root(9875));
    return 0;
}