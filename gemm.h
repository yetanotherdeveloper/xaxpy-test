#include "xbyak/xbyak.h"
#include "xbyak/xbyak_util.h"

void iaxpy(const int *A,const int *x, int *y, int m, int n);
void saxpy(const float *A,const float *x, float *y, int m, int n);

struct iaxpyFunc : public Xbyak::CodeGenerator {
    iaxpyFunc()
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

// We can assume that matrix dimensions are divisible by 8

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

  Xbyak::util::Cpu current_cpu;
  if(current_cpu.has(Xbyak::util::Cpu::tAVX2)) {
    printf("AVX2 supported!\n");
  } else {
    printf("AVX2 not detected!\n");
  }

  xor(rax,rax);

  // TODO: Add RAX raw offset

  L("for_i");
    vmovups(ymm1,ptr [rdi + rax]);  // A    TODO: Add Another raw
    vmovups(ymm0,ptr [rsi + rbx*4]);  // X
    vmulps(ymm2,ymm1,ymm0);
    vaddps(ymm2,ptr[rdx + rax]);  // Y[.] + A[.]*X[.]  
    vmovups(ptr[rdx + rax],ymm2); // Y[.] <- Y[.] + A[.]*X[.]  

    add(rax,32);  // next 8 elements

    inc(rbx);
    cmp(rbx,r8);
    jnz("for_i");

  add(rax,)


    //push(rbx);
    //push(r15);

    //mov(r11,rdi);
    //xor(r15,r15);
    //L("for_j");
      //// A_off = A + i*n

      //// Y[i] += A[i*n+j]*X[j]
      //xor(rbx,rbx);
      //xor(r10,r10);
      //L("for_i");
        //mov(eax, ptr[r11 + rbx*4]);
        //mov(r9d, ptr[rsi + rbx*4]);
        ////imul(r13, r9d,eax) // r13d should contina full result
        //imul(eax, r9d); // r13d should contina full result
        //add(r10,rax);       // accumulate 
        //inc(rbx);
        //cmp(rbx,r8);
        //jnz("for_i");
      //add(ptr [rdx + r15*4], r10d);
      //mov(rax,r8);
      //shl(rax,2);
      //add(r11,rax);  // Go to next column                
      //inc(r15);     // Next output element
      //dec(rcx);
      //jnz("for_j");


      //pop(r15);
      //pop(rbx);

      printf("generating code\n");
#else
        printf("32bit not supported\n");
#endif
        ret();
    }
};

