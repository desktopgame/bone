#ifndef BONE_UTIL_STACK_H
#define BONE_UTIL_STACK_H

typedef void (*bnStackElementDeleter)(void*);

typedef struct bnStackElement {
        void* value;
        struct bnStack* next;
} bnStackElement;

typedef struct bnStack {
        bnStackElement* head;
} bnStack;

bnStack* bnNewStack();

void bnPushStack(bnStack* self, void* data);

void* bnPopStack(bnStack* self);

void bnDeleteStack(bnStack* self, bnStackElementDeleter deleter);
#endif