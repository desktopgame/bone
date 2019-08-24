#include "jump_stack.h"
#include <assert.h>
#include <stdbool.h>
#include "memory.h"

static bnJStackElement* new_elem();
static bnJStackElement* push_elem(bnJStackElement* elem);
static void delete_elem(bnJStackElement* elem);

bnJStack* bnNewJStack() {
        bnJStack* ret = BN_MALLOC(sizeof(bnJStack));
        ret->head = NULL;
        return ret;
}

bnJStackElement* bnPushJStack(bnJStack* self) {
        if (self->head == NULL) {
                self->head = new_elem();
                return self->head;
        } else {
                return push_elem(self->head);
        }
}

bnJStackElement* bnPeekJStack(bnJStack* self) {
        assert(self->head != NULL);
        bnJStackElement* iter = self->head;
        while (iter != NULL) {
                if (iter->next == NULL) {
                        return iter;
                }
                iter = iter->next;
        }
        assert(false);
}

void bnPopJStack(bnJStack* self) {
        assert(self->head != NULL);
        bnJStackElement* iter = self->head;
        while (iter != NULL) {
                if (iter->next == NULL) {
                        delete_elem(iter->next);
                        iter->next = NULL;
                        break;
                }
                iter = iter->next;
        }
}

void bnDeleteJStack(bnJStack* self) {
        delete_elem(self->head);
        BN_FREE(self);
}

static bnJStackElement* new_elem() {
        bnJStackElement* ret = BN_MALLOC(sizeof(bnJStackElement));
        ret->next = NULL;
        return ret;
}

static bnJStackElement* push_elem(bnJStackElement* elem) {
        if (elem->next == NULL) {
                bnJStackElement* last = new_elem();
                elem->next = last;
                return last;
        }
        return push_elem(elem->next);
}

static void delete_elem(bnJStackElement* elem) {
        if (elem == NULL) {
                return;
        }
        bnJStackElement* n = elem->next;
        BN_FREE(elem);
        delete_elem(n);
}