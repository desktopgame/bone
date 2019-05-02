#include "object.h"
#include "frame.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"

void bnInitObject(bnObject* self, bnObjectType type) {
        self->table = g_hash_table_new(g_int_hash, g_int_equal);
        self->mark = false;
        self->type = type;
}

bnObject* bnNewObject() {
        bnObject* ret = BN_MALLOC(sizeof(bnObject));
        bnInitObject(ret, BN_OBJECT_PROTO);
        return ret;
}

void bnDefine(bnObject* self, bnStringView name, bnObject* value) {
        g_hash_table_insert(self->table, (int)name, value);
}

void bnFuncCall(bnObject* self, bnInterpreter* bone, bnFrame* frame, int argc) {
        bnObject* obj = g_hash_table_lookup(
            self->table, bnIntern(bone->pool, BN_KWD_FUNCCALL));
        if (obj == NULL || obj->type != BN_OBJECT_LAMBDA) {
                return;
        }
        // create new frame
        bnFrame* sub = bnSubFrame(frame);
        for (int i = 0; i < argc; i++) {
                g_trash_stack_push(&sub->stack,
                                   g_trash_stack_pop(&frame->stack));
        }
        bnLambda* lambda = obj;
        if (lambda->type == BN_LAMBDA_NATIVE) {
                int code = setjmp(bone->__jmp);
                if (code == 0) {
                        lambda->u.vFunc(bone, sub);
                } else {
                        assert(code == BN_JMP_CODE_EXCEPTION);
                }
        } else {
                g_abort();
        }
        guint len = g_trash_stack_height(&sub->stack);
        bnObject* ret = len == 0 ? NULL : g_trash_stack_pop(&sub->stack);
        g_trash_stack_push(&frame->stack, ret);
}

void bnDeleteObject(bnObject* self) {}