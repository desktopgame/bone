#ifndef BONE_UTIL_STACK_H
#define BONE_UTIL_STACK_H

/**
 * delete function for element in stack.
 */
typedef void (*bnStackElementDeleter)(void*);

/**
 * single link list.
 */
typedef struct bnStackElement {
        void* value;
        struct bnStackElement* next;
} bnStackElement;

/**
 * stack.
 */
typedef struct bnStack {
        bnStackElement* head;
} bnStack;

/**
 * return new instance of bnStack.
 * @param type
 * @return
 */
bnStack* bnNewStack();

/**
 * push to stack.
 * @param self
 * @param data
 */
void bnPushStack(bnStack* self, void* data);

/**
 * pop from stack.
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
 * return size of stack.
 * @param self
 * @return
 */
int bnGetStackSize(bnStack* self);

/**
 * free a bnStack.
 * @param self
 * @param deleter
 */
void bnDeleteStack(bnStack* self, bnStackElementDeleter deleter);
#endif