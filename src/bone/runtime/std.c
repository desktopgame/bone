#include "std.h"

#ifdef __APPLE__
#include <signal.h>
#define CAN_BREAK (1)
#endif

#include "../bone.h"
#include "../parse/ast2il.h"
#include "../parse/parser.h"
#include "bool.h"
#include "enviroment.h"
#include "frame.h"
#include "integer.h"
#include "interpreter.h"
#include "object.h"
#include "string.h"
#include "vm.h"

// only in debug build
#if DEBUG
void bnStdDebugAssert(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        bnBool* cond = a;
        if (!cond->value) {
                abort();
        }
}

void bnStdDebugDie(bnInterpreter* bone, bnFrame* frame) { abort(); }

void bnStdDebugPrint(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        bnString* str = a;
        const char* cstr = bnView2Str(bone->pool, str->value);
        printf("  [%s]", cstr);
        fflush(stdout);
}

void bnStdDebugPrintln(bnInterpreter* bone, bnFrame* frame) {
        bnStdDebugPrint(bone, frame);
        printf("\n");
}

void bnStdDebugBreak(bnInterpreter* bone, bnFrame* frame) {
#if CAN_BREAK
        raise(SIGINT);
#else
        // please set break point at here
        abort();
#endif
}

void bnStdDebugDumpTable(bnInterpreter* bone, bnFrame* frame) {
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, frame->variableTable);
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                const char* kStr = (const char*)bnView2Str(bone->pool, k);
                fprintf(stdout, "%s:", kStr);
                bnPrintObject(stdout, v);
                fprintf(stdout, "\n");
        }
}
#endif

// Built-in

void bnStdSystemInclude(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        bnStringView pathView = ((bnString*)a)->value;
        const char* pathStr = bnView2Str(bone->pool, pathView);
        // parse file
        bnAST* ast = bnParseFile(bone->pool, pathStr);
        if (ast == NULL) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        bnILToplevel* iltop = bnAST2IL(ast);
        if (iltop == NULL) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        // gen code
        bnFrame* sub = bnNewFrame();
        bnEnviroment* env = bnNewEnviroment();
        bnGenerateILTopLevel(bone, iltop, env);
        bnInjectFrame(frame->variableTable, sub);
        bnExecute(bone, env, sub);
        bnInjectFrame(sub->variableTable, frame);
        bnDeleteAST(ast);
        bnDeleteILTopLevel(iltop);
        bnDeleteEnviroment(env);
}

void bnStdSystemLoad(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        bnStringView pathView = ((bnString*)a)->value;
        const char* pathStr = bnView2Str(bone->pool, pathView);
        // parse file
        bnAST* ast = bnParseFile(bone->pool, pathStr);
        if (ast == NULL) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        bnILToplevel* iltop = bnAST2IL(ast);
        if (iltop == NULL) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        // gen code
        bnFrame* sub = bnNewFrame();
        bnEnviroment* env = bnNewEnviroment();
        bnGenerateILTopLevel(bone, iltop, env);
        bnWriteDefaults(bone, sub, bone->pool);
        // get default hash
        GList* keys = NULL;
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, sub->variableTable);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                keys = g_list_append(keys, v);
        }
        bnExecute(bone, env, sub);
        // remove default
        GList* listIter = keys;
        while (listIter != NULL) {
                g_hash_table_remove(sub->variableTable, listIter->data);
                listIter = listIter->next;
        }
        g_list_free(keys);
        bnInjectFrame(sub->variableTable, frame);
        bnDeleteAST(ast);
        bnDeleteILTopLevel(iltop);
        bnDeleteEnviroment(env);
}

void bnStdSystemObject(bnInterpreter* bone, bnFrame* frame) {
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewObject(bone->heap));
}

// Bool

void bnStdBoolFuncCall(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolPositive(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolNegative(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolChilda(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolNot(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolPlus(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolMinus(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolMultiply(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolDivide(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolModulo(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolBitAnd(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolBitOr(bnInterpreter* bone, bnFrame* frame);

// void bnStdBoolLogicAnd(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdBoolLogicOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolExcOr(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolLShift(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolRShift(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolGT(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolGE(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolLT(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolLE(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolEqual(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolNotEqual(bnInterpreter* bone, bnFrame* frame);

// Integer

void bnStdIntegerFuncCall(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnStdIntegerPositive(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnStdIntegerNegative(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnStdIntegerChilda(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnStdIntegerNot(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnStdIntegerPlus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai + bi));
}

void bnStdIntegerMinus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai - bi));
}

void bnStdIntegerMultiply(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai * bi));
}

void bnStdIntegerDivide(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai / bi));
}

void bnStdIntegerModulo(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai % bi));
}

void bnStdIntegerBitAnd(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai & bi));
}

void bnStdIntegerBitOr(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai | bi));
}

// void bnStdIntegerLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// void bnStdIntegerLogicOr(bnInterpreter* bone, bnFrame* frame){}

void bnStdIntegerExcOr(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerLShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerRShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerGT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai > bi));
}

void bnStdIntegerGE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai >= bi));
}

void bnStdIntegerLT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai < bi));
}

void bnStdIntegerLE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai <= bi));
}

void bnStdIntegerEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai == bi));
}

void bnStdIntegerNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai != bi));
}

void bnStdIntegerToString(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        char buf[100];
        sprintf(buf, "%d", ((bnInteger*)a)->value);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewString(bone, bnIntern(bone->pool, buf)));
}

// String

void bnStdStringFuncCall(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringPositive(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringNegative(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringChilda(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringNot(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringPlus(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringMinus(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringMultiply(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringDivide(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringModulo(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringBitAnd(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringBitOr(bnInterpreter* bone, bnFrame* frame) {}

// void bnStdStringLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// void bnStdStringLogicOr(bnInterpreter* bone, bnFrame* frame){}

void bnStdStringExcOr(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringLShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringRShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringGT(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringGE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringLT(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringLE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING || b->type != BN_OBJECT_STRING) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        bnStringView ai = ((bnString*)a)->value;
        bnStringView bi = ((bnString*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai == bi));
}

void bnStdStringNotEqual(bnInterpreter* bone, bnFrame* frame) {}
