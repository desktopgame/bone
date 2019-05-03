#include "stack.h"
#include "../bone.h"

static bnStackElement* new_elem(void* data);
static void push_impl(bnStackElement* head, void* data);
static void free_impl(bnStackElement* head, bnStackElementDeleter deleter);

bnStack* bnNewStack() {
        bnStack* ret = BN_MALLOC(sizeof(bnStack));
        ret->head = NULL;
        return ret;
}

void bnPushStack(bnStack* self, void* data) {
        if (self->head == NULL) {
                self->head = new_elem(data);
        } else {
                push_impl(self->head, data);
        }
}

void* bnPopStack(bnStack* self) {
        assert(self->head != NULL);
        bnStackElement* prev = self->head;
        bnStackElement* elem = self->head;
        while (1) {
                if (elem->next == NULL) {
                        void* ret = elem->value;
                        if (elem == self->head) {
                                BN_FREE(self->head);
                                self->head = NULL;
                        } else {
                                BN_FREE(elem);
                                prev->next = NULL;
                        }
                        return ret;
                }
                prev = elem;
                elem = elem->next;
        }
}

void* bnPeekStack(bnStack* self) {
        bnStackElement* iter = self->head;
        assert(iter != NULL);
        while (iter != NULL) {
                if (iter->next == NULL) {
                        return iter->value;
                }
                iter = iter->next;
        }
}

int bnGetStackSize(bnStack* self) {
        if (self->head == NULL) {
                return 0;
        }
        int count = 0;
        bnStackElement* iter = self->head;
        while (iter != NULL) {
                count++;
                iter = iter->next;
        }
        return count;
}

void bnDeleteStack(bnStack* self, bnStackElementDeleter deleter) {
        free_impl(self->head, deleter);
        BN_FREE(self);
}

static bnStackElement* new_elem(void* data) {
        bnStackElement* ret = BN_MALLOC(sizeof(bnStackElement));
        ret->next = NULL;
        ret->value = data;
        return ret;
}
static void push_impl(bnStackElement* head, void* data) {
        if (head->next == NULL) {
                bnStackElement* elem = new_elem(data);
                head->next = elem;
                return;
        }
        push_impl(head->next, data);
}
static void free_impl(bnStackElement* head, bnStackElementDeleter deleter) {
        if (head == NULL) {
                return;
        }
        if (head->next != NULL) {
                free_impl(head->next, deleter);
        }
        if (deleter != NULL) {
                deleter(head->value);
        }
        BN_FREE(head);
}