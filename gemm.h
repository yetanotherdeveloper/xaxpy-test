#include "xbyak/xbyak.h"

void saxpy(const int *A,const int *x, int *y, int m, int n);

struct saxpyFunc : public Xbyak::CodeGenerator {
    saxpyFunc()
    {
#if defined(__x86_64__)
// calling convenrtion RDI, RSI, RDX, RCX, R8, R9
// XMM0-7 (ints are passed that way)
//      RDI - PTR A
//      RSI - PTR X
//      RDX - PTR Y
//      RCX - M
//      R8  - N

// Regsters that need to be preserved: RBX,RBP, R12-R15

    push(rbx);
    push(r15);

    mov(r11,rdi);
    xor(r15,r15);
    L("for_j");
      // A_off = A + i*n

      // Y[i] += A[i*n+j]*X[j]
      xor(rbx,rbx);
      xor(r10,r10);
      L("for_i");
        mov(eax, ptr[r11 + rbx*4]);
        mov(r9d, ptr[rsi + rbx*4]);
        //imul(r13, r9d,eax) // r13d should contina full result
        imul(eax, r9d); // r13d should contina full result
        add(r10,rax);       // accumulate 
        inc(rbx);
        cmp(rbx,r8);
        jnz("for_i");
      add(ptr [rdx + r15*4], r10d);
      mov(rax,r8);
      shl(rax,2);
      add(r11,rax);  // Go to next column                
      inc(r15);     // Next output element
      dec(rcx);
      jnz("for_j");


      pop(r15);
      pop(rbx);

      printf("generating code\n");
#else
        printf("32bit not supported\n");
#endif
        ret();
    }
};
