#include <stdlib.h>
#include <stdio.h>


void display_stack() {
void *esp;
void *ebp;
    asm ("movl %%esp, %0"
        : "=r" (esp)
        :
        :);
    asm("movl %%ebp,%0"
        :"=r" (ebp)
        :
        :);
    printf("ebp = %x \nesp = %x\n", ebp, esp);
}

int main() {
    display_stack();
    return 0;
}
