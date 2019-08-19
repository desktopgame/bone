#ifndef BONE_UTIL_STACK_H
#define BONE_UTIL_STACK_H

/**
 * bnStackElementDeleterはスタックの中身を解放する関数ポインタ型です。
 */
typedef void (*bnStackElementDeleter)(void*);

/**
 * bnStackElementはスタックの要素です。
 */
typedef struct bnStackElement {
        void* value;
        struct bnStackElement* next;
} bnStackElement;

/**
 * bnStackはスタックの先頭を保存します。
 */
typedef struct bnStack {
        bnStackElement* head;
} bnStack;

/**
 * 新しいスタックインスタンスを生成して返します。
 * @param type
 * @return
 */
bnStack* bnNewStack();

/**
 * スタックの末尾へ要素を追加します。
 * @param self
 * @param data
 */
void bnPushStack(bnStack* self, void* data);

/**
 * スタックの末尾の要素を返します。
 * @param self
 * @return
 */
void* bnPopStack(bnStack* self);

/**
 * peek from stack.
 * @param self
 * @return
 */
void* bnPeekStack(bnStack* self);

/**
 * スタックの長さを返します。
 * @param self
 * @return
 */
int bnGetStackSize(bnStack* self);

/**
 * スタックとその中身を解放します。
 * @param self
 * @param deleter
 */
void bnDeleteStack(bnStack* self, bnStackElementDeleter deleter);
#endif