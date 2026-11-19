/* A2: Recursive digital root and exponentiation */
#include <stdio.h>
#include <stdlib.h>

static int digit_sum(long long n) {
    int sum=0;
    while (n>0) { sum += (int)(n%10); n/=10; }
    return sum;
}
int digital_root_recursive(long long n) {
    if (n<0) n=-n;
    if (n<10) return (int)n;
    return digital_root_recursive(digit_sum(n));
}
long long power_linear(long long base, unsigned int exp) {
    if (exp==0) return 1;
    return base * power_linear(base,exp-1);
}
long long power_fast(long long base, unsigned int exp) {
    if (exp==0) return 1;
    long long half=power_fast(base,exp/2);
    return (exp%2==0) ? half*half : base*half*half;
}
int main(void) {
    printf("digital_root(9875) = %d\n",digital_root_recursive(9875));
    printf("2^10 linear = %lld\n",power_linear(2,10));
    printf("2^10 fast = %lld\n",power_fast(2,10));
    return 0;
}