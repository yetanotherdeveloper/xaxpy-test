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

  shl (rcx,2);  // num of Output elements * size of float (4)

  push(rbx);

  xor(rax,rax);
  xor(r9,r9); // i
  xor(r10,r10); // j

  L("for_j");
    vxorps(ymm3,ymm3);  // 8 partial sums of multiplication
    xor(rbx,rbx);
    L("for_i");
      vmovups(ymm1,ptr [rdi + rax]);  // A
      vmovups(ymm0,ptr [rsi + rbx*4]);  // X
      vmulps(ymm2,ymm1,ymm0);
      vaddps(ymm3,ymm2);
      add(rax,32);  // next 8 elements
      add(rbx,8);
      cmp(rbx,r8);
      jnz("for_i");

    // Here We sum 8 partial sums and add it to Y[j]
    vextractf128(xmm0,ymm3,1);  // Get upper half (4 floats)
    vaddps(xmm0,xmm0,xmm3);     // and add it to lower 4 floats
    vhaddps(xmm0,xmm0,xmm0);    // make two sums (first two floats together and third and forth floats together)
    vhaddps(xmm0,xmm0,xmm0);    // Final sum 
    vaddss(xmm0,ptr[rdx + r10]); // Y[.] + A[.]*X[.]  
    vmovss(ptr[rdx + r10],xmm0); // Y[.] <- Y[.] + A[.]*X[.]  

    lea (r9,ptr [r9+r8*4]); // jump to next raw
    mov(rax,r9);

    add(r10,4);
    cmp(r10,rcx);
    jnz("for_j");
  
  pop(rbx);

  printf("Generating SGEMM code\n");
#else
        printf("32bit not supported\n");
#endif
  ret();
}
};

