#ifndef BONE_UTIL_JUMPSTACK_H
#define BONE_UTIL_JUMPSTACK_H
#include <setjmp.h>

/**
 * BN_JMP_PUSH is call a setjmp and bnPushJStack.
 */
#define BN_JMP_PUSH(stack) (setjmp(bnPushJStack(stack)->__buf))

/**
 * BN_JMP_DO is call a longjmp and bnPeekJStack.
 */
#define BN_JMP_DO(stack, code) (longjmp(bnPeekJStack(stack)->__buf, (code)))

/**
 * BN_JMP_POP is call a bnPopJStack.
 */
#define BN_JMP_POP(stack) (bnPopJStack(stack))

/**
 * bnJStackElement is element of bnJStack.
 */
typedef struct bnJStackElement {
        jmp_buf __buf;
        struct bnJStackElement* next;
} bnJStackElement;

/**
 * bnJStack is structure of stack for jmp_buf.
 */
typedef struct bnJStack {
        bnJStackElement* head;
} bnJStack;

/**
 * @return
 */
bnJStack* bnNewJStack();

/**
 * in almost, is'nt need use this function.
 * shoud be called by macro in if statement.
 * @param self
 * @return
 */
bnJStackElement* bnPushJStack(bnJStack* self);

/**
 * in almost, is'nt need use this function.
 * shoud be called by macro in if statement.
 * @param self
 * @return
 */
bnJStackElement* bnPeekJStack(bnJStack* self);

/**
 * in almost, is'nt need use this function.
 * @param self
 */
void bnPopJStack(bnJStack* self);

/**
 * @param self
 */
void bnDeleteJStack(bnJStack* self);
#endif