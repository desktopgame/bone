#include "vm.h"
#include "../glib.h"
#include "../util/string.h"
#include "array.h"
#include "bool.h"
#include "char.h"
#include "double.h"
#include "heap.h"
#include "integer.h"
#include "lambda.h"
#include "opcode.h"
#include "snapshot.h"
#include "string.h"

GString* bnCreateStackFrameString(bnInterpreter* bone, bnEnviroment* env,
                                  bnObject* lambda, int PC) {
        int line = bnFindLineRange(env, PC);
        GString* gbuf = g_string_new("");
        const char* caller = bnView2Str(bone->pool, env->filename);
        assert(caller != NULL);
        g_string_append_printf(gbuf, "call at %s<%d>, ",
                               caller + bnLastPathComponent(caller),
                               bnFindLineRange(env, PC) + env->lineOffset);
        const char* definer =
            bnView2Str(bone->pool, bnGetLambdaFileName(lambda));
        assert(definer != NULL);
        GList* iter = bnGetParameterList(lambda);
        g_string_append_printf(gbuf, "defined at %s<%d>, ",
                               definer + bnLastPathComponent(definer),
                               bnGetLambdaLineNumber(lambda));

        g_string_append(gbuf, " #");
        bnGStringAppendC(gbuf, '(');
        while (iter != NULL) {
                g_string_append(gbuf, bnView2Str(bone->pool, iter->data));
                iter = iter->next;
                if (iter != NULL) {
                        bnGStringAppendC(gbuf, ' ');
                }
        }
        bnGStringAppendC(gbuf, ')');
        iter = bnGetReturnValueList(lambda);
        bnGStringAppendC(gbuf, '(');
        while (iter != NULL) {
                g_string_append(gbuf, bnView2Str(bone->pool, iter->data));
                iter = iter->next;
                if (iter != NULL) {
                        bnGStringAppendC(gbuf, ' ');
                }
        }
        bnGStringAppendC(gbuf, ')');
        g_string_append_printf(gbuf, "<%d>", (line + env->lineOffset));
        return gbuf;
}

struct bnObject* bnCreateLambdaInActiveCode(bnInterpreter* bone,
                                            bnEnviroment* env, bnFrame* frame,
                                            int* pPC) {
        bnObject* lmb = bnNewLambda(bone, BN_LAMBDA_SCRIPT);
        int line = bnReadCode(env, ++(*pPC));
        bnSetLambdaFileName(lmb, env->filename);
        bnSetLambdaLineNumber(lmb, line);
        bnEnviroment* lmbEnv = bnNewEnviroment(env->filename);
        lmbEnv->lineOffset = line;
        bnSetEnviroment(lmb, lmbEnv);
        // is instance base?
        int parameterLen = bnReadCode(env, ++(*pPC));
        for (int i = 0; i < parameterLen; i++) {
                bnAddParameter(lmb,
                               g_ptr_array_index(env->codeArray, ++(*pPC)));
        }
        // length of named return
        int namedReturnLen = bnReadCode(env, ++(*pPC));
        g_ptr_array_add(bnGetEnviroment(lmb)->codeArray, BN_OP_NOP);
        for (int i = 0; i < namedReturnLen; i++) {
                bnAddReturnValue(lmb,
                                 g_ptr_array_index(env->codeArray, ++(*pPC)));
        }
        // collect all variables
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, frame->variableTable);
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                g_hash_table_replace(bnGetCapturedMap(lmb), k, v);
        }
        int lambdaNest = 1;
        // generate code
        while (1) {
                assert(*pPC < env->codeArray->len);
                gpointer data = bnReadCode(env, ++(*pPC));
                // bug if index of string view equal
                // BN_OP_GEN_LAMBDA_END
                if (bnOperands((bnOpcode)data) == 1) {
                        if (data == BN_OP_GOTO || data == BN_OP_GOTO_IF ||
                            data == BN_OP_GOTO_ELSE) {
                                g_ptr_array_add(bnGetEnviroment(lmb)->codeArray,
                                                data);
                                bnLabel* clone =
                                    bnNewLabel(((bnLabel*)g_ptr_array_index(
                                                    env->codeArray, ++(*pPC)))
                                                   ->pos);
                                g_ptr_array_add(bnGetEnviroment(lmb)->codeArray,
                                                clone);
                                g_ptr_array_add(bnGetEnviroment(lmb)->labels,
                                                clone);
                        } else {
                                g_ptr_array_add(bnGetEnviroment(lmb)->codeArray,
                                                data);
                                g_ptr_array_add(bnGetEnviroment(lmb)->codeArray,
                                                g_ptr_array_index(
                                                    env->codeArray, ++(*pPC)));
                        }
                        continue;
                }
                if (data == BN_OP_GEN_LAMBDA_BEGIN) {
                        g_ptr_array_add(bnGetEnviroment(lmb)->codeArray, data);
                        line = bnReadCode(env, ++(*pPC));
                        g_ptr_array_add(bnGetEnviroment(lmb)->codeArray, line);
                        // add params
                        parameterLen =
                            g_ptr_array_index(env->codeArray, ++(*pPC));
                        g_ptr_array_add(bnGetEnviroment(lmb)->codeArray,
                                        parameterLen);
                        for (int i = 0; i < parameterLen; i++) {
                                g_ptr_array_add(bnGetEnviroment(lmb)->codeArray,
                                                g_ptr_array_index(
                                                    env->codeArray, ++(*pPC)));
                        }
                        // add returns
                        namedReturnLen =
                            g_ptr_array_index(env->codeArray, ++(*pPC));
                        g_ptr_array_add(bnGetEnviroment(lmb)->codeArray,
                                        namedReturnLen);
                        g_ptr_array_add(bnGetEnviroment(lmb)->codeArray,
                                        BN_OP_NOP);
                        for (int i = 0; i < namedReturnLen; i++) {
                                bnAddReturnValue(
                                    lmb, g_ptr_array_index(env->codeArray,
                                                           ++(*pPC)));
                        }
                        lambdaNest++;
                } else if (data == BN_OP_GEN_LAMBDA_END) {
                        if (--lambdaNest == 0) {
                                break;
                        }
                        g_ptr_array_add(bnGetEnviroment(lmb)->codeArray, data);

                } else {
                        g_ptr_array_add(bnGetEnviroment(lmb)->codeArray, data);
                }
        }
        return lmb;
}

void bnScopeInjection(bnInterpreter* bone, bnObject* src, bnFrame* dst) {
        // collect all hidden variables
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, src->table);
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                bnStringView kView = k;
                const char* kStr = bnView2Str(bone->pool, kView);
                if (g_str_has_prefix(kStr, "$$_")) {
                        const char* origName = kStr + 3;
                        bnStringView origView = bnIntern(bone->pool, origName);
                        g_hash_table_replace(dst->variableTable, origView, v);
                        g_hash_table_iter_remove(&hashIter);
                }
        }
}

void bnObjectInjection(bnInterpreter* bone, bnObject* src, bnObject* dst) {
        // collect all hidden variables
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, src->table);
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                bnStringView kView = k;
                const char* kStr = bnView2Str(bone->pool, kView);
                if (g_str_has_prefix(kStr, "$$_")) {
                        const char* origName = kStr + 3;
                        bnStringView origView = bnIntern(bone->pool, origName);
                        g_hash_table_replace(dst->table, origView, v);
                        g_hash_table_iter_remove(&hashIter);
                }
        }
}

int bnExecute(bnInterpreter* bone, bnEnviroment* env, bnFrame* frame) {
        GList* snapshotIter = NULL;
        bool inDefer = false;
        for (int PC = 0; PC < env->codeArray->len; PC++) {
                bnOpcode code = (bnOpcode)g_ptr_array_index(env->codeArray, PC);
                switch (code) {
                        case BN_OP_NOP:
                                break;
                        case BN_OP_DUP: {
                                gpointer data = bnPopStack(frame->vStack);
                                bnPushStack(frame->vStack, data);
                                bnPushStack(frame->vStack, data);
                                break;
                        }
                        case BN_OP_SWAP: {
                                void* a = bnPopStack(frame->vStack);
                                bnObject* obj = a;
                                void* b = bnPopStack(frame->vStack);
                                bnPushStack(frame->vStack, a);
                                bnPushStack(frame->vStack, b);
                                break;
                        }
                        case BN_OP_GEN_INT: {
                                int data = bnReadCode(env, ++PC);
                                bnPushStack(frame->vStack,
                                            bnNewInteger(bone, data));
                                break;
                        }
                        case BN_OP_GEN_DOUBLE: {
                                int idx = bnReadCode(env, ++PC);
                                double dv = g_array_index(env->doubleConstants,
                                                          double, idx);
                                bnPushStack(frame->vStack,
                                            bnNewDouble(bone, dv));
                                break;
                        }
                        case BN_OP_GEN_CHAR: {
                                char c = bnReadCode(env, ++PC);
                                bnPushStack(frame->vStack, bnNewChar(bone, c));
                                break;
                        }
                        case BN_OP_GEN_STRING: {
                                bnStringView name = bnReadCode(env, ++PC);
                                // create array for string
                                const char* str = bnView2Str(bone->pool, name);
                                int slen = strlen(str);
                                bnPushStack(frame->vStack,
                                            bnNewInteger(bone, slen));
                                bnObject* arrFunc = g_hash_table_lookup(
                                    frame->variableTable,
                                    bnIntern(bone->pool, "array"));
                                bnFrame* sub =
                                    bnFuncCall(arrFunc, bone, frame, 1);
                                if (sub->panic) {
                                        frame->panic = sub->panic;
                                        break;
                                }
                                // fill by char
                                bnObject* ary = bnPopStack(frame->vStack);
                                for (int i = 0; i < slen; i++) {
                                        bnSetArrayElementAt(
                                            ary, i, bnNewChar(bone, str[i]));
                                }
                                bnPushStack(frame->vStack, ary);
                                bnDeleteFrame(sub);
                                bnGC(bone);
                                // create string by string function
                                bnFrame* sub2 = bnFuncCall(
                                    g_hash_table_lookup(
                                        frame->variableTable,
                                        bnIntern(bone->pool, "string")),
                                    bone, frame, 1);
                                if (sub2->panic) {
                                        frame->panic = sub->panic;
                                        break;
                                }
                                bnDeleteFrame(sub2);
                                bnGC(bone);
                                break;
                        }
                        case BN_OP_GEN_ARRAY: {
                                int size = bnReadCode(env, ++PC);
                                bnPushStack(frame->vStack,
                                            bnNewInteger(bone, size));
                                bnFrame* sub = bnFuncCall(
                                    g_hash_table_lookup(
                                        frame->variableTable,
                                        bnIntern(bone->pool, "array")),
                                    bone, frame, 1);
                                if (sub->panic) {
                                        frame->panic = sub->panic;
                                        break;
                                }
                                bnObject* ary = bnPopStack(frame->vStack);
                                for (int i = 0; i < size; i++) {
                                        bnSetArrayElementAt(
                                            ary, size - (i + 1),
                                            bnPopStack(frame->vStack));
                                }
                                bnDeleteFrame(sub);
                                bnPushStack(frame->vStack, ary);
                                bnGC(bone);
                                break;
                        }
                        case BN_OP_GEN_LAMBDA_BEGIN: {
                                bnPushStack(frame->vStack,
                                            bnCreateLambdaInActiveCode(
                                                bone, env, frame, &PC));
                                break;
                        }
                        case BN_OP_GEN_LAMBDA_END: {
                                break;
                        }
                        case BN_OP_SET_REGISTER_0:
                                frame->register0 = bnPopStack(frame->vStack);
                                break;
                        case BN_OP_GET_REGISTER_0:
                                bnPushStack(frame->vStack, frame->register0);
                                frame->register0 = NULL;
                                break;
                        case BN_OP_PUSH_SELF: {
                                bnPushStack(frame->hierarcySelf,
                                            bnPopStack(frame->vStack));
                                break;
                        }
                        case BN_OP_POP_SELF: {
                                bnPopStack(frame->hierarcySelf);
                                break;
                        }
                        case BN_OP_SCOPE_INJECTION: {
                                bnObject* obj = bnPopStack(frame->vStack);
                                bnScopeInjection(bone, obj, frame);
                                break;
                        }
                        case BN_OP_OBJECT_INJECTION: {
                                bnObject* src = bnPopStack(frame->vStack);
                                bnObject* dst = bnPopStack(frame->vStack);
                                bnObjectInjection(bone, src, dst);
                                bnPushStack(frame->vStack, dst);
                                break;
                        }
                        case BN_OP_STORE: {
                                bnStringView name = bnReadCode(env, ++PC);
                                bnObject* value = bnPopStack(frame->vStack);
                                g_hash_table_replace(frame->variableTable,
                                                     GINT_TO_POINTER((int)name),
                                                     value);
                                assert(value != NULL);
                                break;
                        }
                        case BN_OP_LOAD: {
                                bnStringView name = bnReadCode(env, ++PC);
                                bnObject* value = g_hash_table_lookup(
                                    frame->variableTable,
                                    GINT_TO_POINTER((int)name));
                                const char* str = bnView2Str(bone->pool, name);
                                assert(str != NULL);
                                if (value == NULL) {
                                        char buf[256];
                                        sprintf(
                                            buf,
                                            "is'nt found local variable: %s",
                                            str);
                                        bnPanic(bone,
                                                bnNewString(
                                                    bone,
                                                    bnIntern(bone->pool, buf)));
                                        break;
                                }
                                assert(value != NULL);
                                bnPushStack(frame->vStack, value);
                                break;
                        }
                        case BN_OP_PUT: {
                                bnObject* container = bnPopStack(frame->vStack);
                                if (container == NULL) {
                                        bnPanic(bone,
                                                bnNewString(
                                                    bone, "receiver is null"));
                                        break;
                                }
                                bnObject* value = bnPopStack(frame->vStack);
                                bnStringView name = bnReadCode(env, ++PC);
                                g_hash_table_replace(container->table, name,
                                                     value);
                                break;
                        }
                        case BN_OP_GET: {
                                bnObject* container = bnPopStack(frame->vStack);
                                if (container == NULL) {
                                        bnPanic(bone,
                                                bnNewString2(
                                                    bone, "receiver is null"));
                                        break;
                                }
                                bnStringView name = bnReadCode(env, ++PC);
                                gpointer data =
                                    g_hash_table_lookup(container->table, name);
                                bnObject* obj = data;
                                const char* str = bnView2Str(bone->pool, name);
                                assert(str != NULL);
                                if (obj == NULL) {
                                        char buf[256];
                                        sprintf(buf, "is'nt found member: %s",
                                                str);
                                        bnPanic(bone,
                                                bnNewString(
                                                    bone,
                                                    bnIntern(bone->pool, buf)));
                                        break;
                                }
                                assert(data != NULL);
                                bnPushStack(frame->vStack, data);
                                break;
                        }
                        case BN_OP_GOTO: {
                                bnLabel* jmp = bnReadLabel(env, ++PC);
                                PC = jmp->pos;
                                break;
                        }
                        case BN_OP_GOTO_IF: {
                                bnLabel* jmp = bnReadLabel(env, ++PC);
                                bnObject* cond = bnPopStack(frame->vStack);
                                if (bnGetBoolValue(cond)) {
                                        PC = jmp->pos;
                                }
                                break;
                        }
                        case BN_OP_GOTO_ELSE: {
                                bnLabel* jmp = bnReadLabel(env, ++PC);
                                bnObject* cond = bnPopStack(frame->vStack);
                                if (!bnGetBoolValue(cond)) {
                                        PC = jmp->pos;
                                }
                                break;
                        }
                        case BN_OP_RETURN: {
                                PC = env->codeArray->len;
                                break;
                        }
                        case BN_OP_FUNCCALL: {
                                bnObject* obj = bnPopStack(frame->vStack);
                                if (obj->type != BN_OBJECT_LAMBDA) {
                                        frame->panic =
                                            bnNewString2(bone,
                                                         "shoud be receiver is "
                                                         "closure object");
                                        break;
                                }
                                int argc = bnReadCode(env, ++PC);

                                bnPushStack(bone->callStack,
                                            bnCreateStackFrameString(bone, env,
                                                                     obj, PC));
                                bnFrame* sub =
                                    bnFuncCall(obj, bone, frame, argc);
                                if (sub->panic == NULL) {
                                        g_string_free(
                                            bnPopStack(bone->callStack), TRUE);
                                }
                                bnDeleteFrame(sub);
                                bnGC(bone);
                                break;
                        }
                        case BN_OP_DEFER_PUSH: {
                                bnLabel* jmp = bnReadLabel(env, ++PC);
                                bnSnapShot* sn = bnNewSnapShot(jmp->pos);
                                GHashTableIter hashIter;
                                gpointer k, v;
                                g_hash_table_iter_init(&hashIter,
                                                       frame->variableTable);
                                while (
                                    g_hash_table_iter_next(&hashIter, &k, &v)) {
                                        g_hash_table_replace(sn->table, k, v);
                                }
                                frame->snapshots = snapshotIter =
                                    g_list_append(frame->snapshots, sn);
                                break;
                        }
                        case BN_OP_DEFER_BEGIN: {
                                int deferNest = 1;
                                while (1) {
                                        assert(PC < env->codeArray->len);
                                        gpointer data = g_ptr_array_index(
                                            env->codeArray, ++PC);
                                        // bug if index of string view equal
                                        // BN_OP_GEN_LAMBDA_END
                                        if (bnOperands((bnOpcode)data) == 1) {
                                                ++PC;
                                                continue;
                                        }
                                        if (data == BN_OP_DEFER_BEGIN) {
                                                deferNest++;
                                        } else if (data == BN_OP_DEFER_END) {
                                                if (--deferNest == 0) {
                                                        break;
                                                }
                                        }
                                }
                                break;
                        }
                        case BN_OP_DEFER_NEXT: {
                                if (snapshotIter != NULL) {
                                        PC = ((bnSnapShot*)snapshotIter->data)
                                                 ->pc;
                                        snapshotIter = snapshotIter->next;
                                } else {
                                        PC = env->codeArray->len;
                                        inDefer = false;
                                }
                                break;
                        }
                        case BN_OP_DEFER_END: {
                                break;
                        }
                        case BN_OP_POP: {
                                bnPopStack(frame->vStack);
                                break;
                        }
                        case BN_OP_CLEANUP_INJBUF: {
                                bnCleanupInjectionBuffer(
                                    bone->pool, bnPeekStack(frame->vStack));
                                break;
                        }
                }
                if (frame->panic && !inDefer) {
                        if (snapshotIter != NULL) {
                                PC = ((bnSnapShot*)snapshotIter->data)->pc;
                                snapshotIter = snapshotIter->next;
                                inDefer = true;
                        } else {
                                if (frame->prev) {
                                        frame->prev->panic = frame->panic;
                                }
                                break;
                        }
                }
        }
        return 0;
}