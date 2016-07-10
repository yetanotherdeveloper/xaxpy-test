#include <cstdio>
#include "xbyak/xbyak.h"

struct AddFunc : public Xbyak::CodeGenerator {
    //AddFunc(int y)
    AddFunc(void)
    {
        mov(eax, 314);
        ret();
    }
};

int main()
{
    AddFunc a;
    int (*add3)(void) = (int (*)(void))a.getCode();
    printf("3 + 2 = %d\n", add3());
}

