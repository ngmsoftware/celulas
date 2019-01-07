#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 *   A few of AT&T asm ...
 *     (results were token by repeating 1000 times the operation on a 640 x 480$ */

void memset__(int *P, int sz, int color)
/*
 *   sz in words (remember sizeof(int) is a l not a w) so, sz = size(P1)*2;
 *
 *   gcc = 24
 *   me  = 24
 *                             Draw game! (gcc optimizes like that?!)
 */
{
    __asm__ __volatile__  ("
        pushf \n
        pushl %esi \n
        pushl %edi \n
        pushl %ecx \n
        pushw %es \n
        pushw %ds \n
        movw %es, %ax \n
        movw %ax, %ds \n
        cld \n
        movl 8(%ebp),%edi \n
        movl 12(%ebp),%ecx \n
        movl 16(%ebp),%eax \n
        rep stosw \n
        popw %ds \n
        popw %es \n
        popl %ecx \n
        popl %edi \n
        popl %esi \n
        popf \n
    ");
}


void memcmp__(int *P1, int *P2, int sz, int *result)
/*
 *   sz in words (remember sizeof(int) is a l not a w) so, sz = size(P1)*2;
 *   gcc = 22s
 *   me  = 14s
 *                    I Won! :-)
 */
{
    __asm__ __volatile__  ("
        pushf \n
        pushl %esi \n
        pushl %edi \n
        pushl %ecx \n
        pushw %es \n
        pushw %ds \n
        movw %es, %ax \n
        movw %ax, %ds \n
        cld \n
        movl 8(%ebp),%edi \n
        movl 12(%ebp),%esi \n
        movl 16(%ebp),%ecx \n
        repe cmpsw \n
        jne ne \n
        movl 20(%ebp),%eax \n
        movl $1,(%eax) \n
        jmp end \n
ne: \n
        movl 20(%ebp),%eax \n
        movl $0,(%eax) \n
end: \n
        popw %ds \n
        popw %es \n
        popl %ecx \n
        popl %edi \n
        popl %esi \n 
        popf \n
    ");
}

void memcpy__(int *P1, int *P2, int sz)
/*
 *   sz in words (remember sizeof(int) is a l not a w) so, sz = size(P1)*2;
 *   P1 - Source
 *   P2 - Destination
 *
 *   gcc = 29s   
 *   me  = 29s
 *                             Draw game (gcc optimizes like that?!)
 */
{
    __asm__ __volatile__  (" 
        pushf \n
        pushl %esi \n
        pushl %edi \n
        pushl %ecx \n
        pushw %es \n
        pushw %ds \n
        movw %es, %ax \n
        movw %ax, %ds \n
        cld \n
        movl 8(%ebp),%esi \n
        movl 12(%ebp),%edi \n
        movl 16(%ebp),%ecx \n
        rep movsw \n
        popw %ds \n
        popw %es \n
        popl %ecx \n 
        popl %edi \n
        popl %esi \n
        popf \n
    ");
}  
 
/*
 *   Enough of AT&T asm for now...
 */
