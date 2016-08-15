#include <cstdio>
#include <memory>
#include "xbyak/xbyak.h"
#include "gemm.h"
#include <immintrin.h>

struct AddFunc : public Xbyak::CodeGenerator {
    AddFunc()
    {
        xor(rax,rax);
        add(rax,rdi);
        add(rax,rsi);
        ret();
    }
};

int main()
{
    AddFunc a;  // Jesli tu sa jakies arguemnty to sa to argumenty do generatora (kod w C)
    int (*add3)(int,int) = (int (*)(int a, int b))a.getCode();  // A tutaj , to juz jak to bedzie wolane
    printf("3 + 2 = %d\n", add3(3,2));

    // SAXPY
    
    // A matrix to be created and filled in
    int m = 100000;
    int n = 10000;
    std::unique_ptr<int[]> A(new int[m*n]);    
    for(int i=0; i<m*n; ++i) {
      A[i] = i;
    }
    
    // X vector    
    std::unique_ptr<int[]> X(new int[n]);    
    for(int i=0; i<n; ++i) {
      X[i] = 2*i;
    }

    // Y output vector
    std::unique_ptr<int[]> Y1(new int[m]);    
    std::unique_ptr<int[]> Y2(new int[m]);    
    for(int i=0; i<m; ++i) {
      Y1[i] = 10*i;
      Y2[i] = 10*i;
    }
    
    saxpyFunc saxpy_code;
    void (*saxpy_func)(const int*,const int*, int*, int, int) = (void (*)(const int *A,const int *x, int *y, int m, int n))saxpy_code.getCode();

    unsigned long long t0;
    unsigned long long t1;

    // Call reference C++ GEMM routine
    t0 = __rdtsc();
    saxpy(A.get(),X.get(),Y1.get(),m,n);
    t1 =  __rdtsc();

    unsigned long long cppDelta = t1-t0;

    // Call Assembly routine
    t0 = __rdtsc();
    saxpy_func(A.get(),X.get(),Y2.get(),m,n); 
    t1 =  __rdtsc();

    unsigned long long asmDelta = t1-t0;

    for(unsigned int i=0; i<m; ++i) {
      if(Y1[i] != Y2[i]) {
        printf("ERROR in computation!\n");
        exit(-1);        
      }
    }

    printf("IGEMM C++ code execution time: %lld\n", cppDelta);
    printf("IGEMM ASM code execution time: %lld\n", asmDelta);
}

