#ifndef BONE_UTIL_JUMPSTACK_H
#define BONE_UTIL_JUMPSTACK_H
#include <setjmp.h>

/**
 * 現在のコンテキストを保存します。
 * @param stack
 */
#define BN_JMP_PUSH(stack) (setjmp(bnPushJStack(stack)->__buf))

/**
 * 最後に保存されたコンテキストへジャンプします。
 * @param stack
 * @param code
 */
#define BN_JMP_DO(stack, code) (longjmp(bnPeekJStack(stack)->__buf, (code)))

/**
 * 最後に保存されたコンテキストを破棄します。
 * @param stack
 */
#define BN_JMP_POP(stack) (bnPopJStack(stack))

/**
 * bnJStackElementは連結リストの構造でコンテキストを保存する構造体です。
 */
typedef struct bnJStackElement {
        jmp_buf __buf;
        struct bnJStackElement* next;
} bnJStackElement;

/**
 * bnJStackは連結リストの先頭を保存する構造体です。
 */
typedef struct bnJStack {
        bnJStackElement* head;
} bnJStack;

/**
 * 新しいbnNewJStackインスタンスを生成して返します。
 * @return
 */
bnJStack* bnNewJStack();

/**
 * 現在のコンテキストを保存する新しい連結リストの要素を作成して、
 * 現在のコンテキストを保存します。
 * 戻り値としてコンテキストの保存された要素を返します。
 * @param self
 * @return
 */
bnJStackElement* bnPushJStack(bnJStack* self);

/**
 * 最後に保存された要素を返します。
 * @param self
 * @return
 */
bnJStackElement* bnPeekJStack(bnJStack* self);

/**
 * 最後に保存された要素を返します。
 * @param self
 */
void bnPopJStack(bnJStack* self);

/**
 * 全ての要素を解放します。
 * @param self
 */
void bnDeleteJStack(bnJStack* self);
#endif