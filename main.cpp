#include <cstdio>
#include <memory>
#include "xbyak/xbyak.h"
#include "gemm.h"

// TODO: zrobic tu mnozenie macierzy albo dodawanie macierzy

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
    int m = 100;
    int n = 100;
    std::unique_ptr<float[]> A(new float[m*n]);    
    for(int i=0; i<m*n; ++i) {
      A[i] = 0.1f * i;
    }
    
    // X vector    
    std::unique_ptr<float[]> X(new float[m]);    
    for(int i=0; i<m; ++i) {
      A[i] = i;
    }

    // Y output vector
    std::unique_ptr<float[]> Y(new float[n]);    
    for(int i=0; i<n; ++i) {
      A[i] = 10.0f*i;
    }

    saxpy(A.get(),X.get(),Y.get(),m,n);
}

