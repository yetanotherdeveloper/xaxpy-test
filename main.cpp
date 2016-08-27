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
    AddFunc a;  // Any arguments here will have them given to generator
    int (*add3)(int,int) = (int (*)(int a, int b))a.getCode();  // Here is signature of how to generated code will be called
    printf("3 + 2 = %d\n", add3(3,2));

    // IAXPY
    
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
    
    iaxpyFunc iaxpy_code;
    void (*iaxpy_func)(const int*,const int*, int*, int, int) = (void (*)(const int *A,const int *x, int *y, int m, int n))iaxpy_code.getCode();

    unsigned long long t0;
    unsigned long long t1;

    // Call reference C++ GEMM routine
    t0 = __rdtsc();
    iaxpy(A.get(),X.get(),Y1.get(),m,n);
    t1 =  __rdtsc();

    unsigned long long cppDelta = t1-t0;

    // Call Assembly routine
    t0 = __rdtsc();
    iaxpy_func(A.get(),X.get(),Y2.get(),m,n); 
    t1 =  __rdtsc();

    unsigned long long asmDelta = t1-t0;

    for(unsigned int i=0; i<m; ++i) {
      if(Y1[i] != Y2[i]) {
        printf("ERROR in computation!\n");
        exit(-1);        
      }
    }

    printf("IAXPY C++ code execution time: %lld\n", cppDelta);
    printf("IAXPY ASM code execution time: %lld\n", asmDelta);

    A.reset(nullptr);
    X.reset(nullptr);
    Y1.reset(nullptr);
    Y2.reset(nullptr);

    std::unique_ptr<float[]> fA(new float[m*n]);    
    for(int i=0; i<m*n; ++i) {
      fA[i] = i;
    }
    
    // X vector    
    std::unique_ptr<float[]> fX(new float[n]);    
    for(int i=0; i<n; ++i) {
      fX[i] = 2.0f*i;
    }

    // Y output vector
    std::unique_ptr<float[]> fY1(new float[m]);    
    std::unique_ptr<float[]> fY2(new float[m]);    
    for(int i=0; i<m; ++i) {
      fY1[i] = 10.0f*i;
      fY2[i] = 10.0f*i;
    }

    saxpyFunc saxpy_code;
    void (*saxpy_func)(const float*,const float*, float*, int, int) = (void (*)(const float *A,const float *x, float *y, int m, int n))saxpy_code.getCode();

    // Call reference C++ GEMM routine
    t0 = __rdtsc();
    saxpy(fA.get(),fX.get(),fY1.get(),m,n);
    t1 =  __rdtsc();
    cppDelta = t1-t0;

    printf("SAXPY C++ code execution time: %lld\n", cppDelta);

    // Call Assembly routine
    t0 = __rdtsc();
    saxpy_func(fA.get(),fX.get(),fY2.get(),m,n); 
    t1 =  __rdtsc();

    asmDelta = t1-t0;
    printf("SAXPY ASM code execution time: %lld\n", asmDelta);
}

