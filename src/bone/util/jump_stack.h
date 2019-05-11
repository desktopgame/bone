#ifndef BONE_UTIL_JUMPSTACK_H
#define BONE_UTIL_JUMPSTACK_H
#include <setjmp.h>

#define BN_JMP_PUSH(stack) (setjmp(bnPushJStack(stack)->__buf))
#define BN_JMP_DO(stack, code) (longjmp(bnPeekJStack(stack)->__buf, (code)))
#define BN_JMP_POP(stack) (bnPopJStack(stack))

typedef struct bnJStackElement {
        jmp_buf __buf;
        struct bnJStackElement* next;
} bnJStackElement;

typedef struct bnJStack {
        bnJStackElement* head;
} bnJStack;

bnJStack* bnNewJStack();

bnJStackElement* bnPushJStack(bnJStack* self);

bnJStackElement* bnPeekJStack(bnJStack* self);

void bnPopJStack(bnJStack* self);

void bnDeleteJStack(bnJStack* self);
#endif