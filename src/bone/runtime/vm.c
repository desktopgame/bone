#include "vm.h"
#include "../glib.h"
#include "../util/string.h"
#include "array.h"
#include "char.h"
#include "heap.h"
#include "integer.h"
#include "lambda.h"
#include "opcode.h"
#include "snapshot.h"
#include "string.h"

GString* bnCreateStackFrameString(bnInterpreter* bone, bnEnviroment* env,
                                  bnLambda* lambda, int PC) {
        int line = bnFindLineRange(env, PC);
        GString* gbuf = g_string_new("");
        const char* caller = bnView2Str(bone->pool, env->filename);
        g_string_append_printf(gbuf, "call at %s<%d>, ",
                               caller + bnLastPathComponent(caller),
                               bnFindLineRange(env, PC) + env->lineOffset);
        const char* definer = bnView2Str(bone->pool, lambda->filename);
        GList* iter = ((bnLambda*)lambda)->parameters;
        g_string_append_printf(gbuf, "defined at %s<%d>, ",
                               definer + bnLastPathComponent(definer),
                               lambda->lineno);

        g_string_append(gbuf, " #");
        g_string_append_c(gbuf, '(');
        while (iter != NULL) {
                g_string_append(gbuf, bnView2Str(bone->pool, iter->data));
                iter = iter->next;
                if (iter != NULL) {
                        g_string_append_c(gbuf, ' ');
                }
        }
        g_string_append_c(gbuf, ')');
        iter = ((bnLambda*)lambda)->returns;
        g_string_append_c(gbuf, '(');
        while (iter != NULL) {
                g_string_append(gbuf, bnView2Str(bone->pool, iter->data));
                iter = iter->next;
                if (iter != NULL) {
                        g_string_append_c(gbuf, ' ');
                }
        }
        g_string_append_c(gbuf, ')');
        g_string_append_printf(gbuf, "<%d>", (line + env->lineOffset));
        return gbuf;
}

int bnExecute(bnInterpreter* bone, bnEnviroment* env, bnFrame* frame) {
        bnObject* BN_TRUE = g_hash_table_lookup(frame->variableTable,
                                                bnIntern(bone->pool, "true"));
        bnObject* BN_FALSE = g_hash_table_lookup(frame->variableTable,
                                                 bnIntern(bone->pool, "false"));
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
                                int data =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                bnPushStack(frame->vStack,
                                            bnNewInteger(bone, data));
                                break;
                        }
                        case BN_OP_GEN_DOUBLE:
                                break;
                        case BN_OP_GEN_CHAR: {
                                char c =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                bnPushStack(frame->vStack, bnNewChar(bone, c));
                                break;
                        }
                        case BN_OP_GEN_STRING: {
                                bnStringView name =
                                    g_ptr_array_index(env->codeArray, ++PC);
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
                                bnArray* ary = bnPopStack(frame->vStack);
                                for (int i = 0; i < slen; i++) {
                                        g_ptr_array_index(ary->arr, i) =
                                            bnNewChar(bone, str[i]);
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
                                int size =
                                    g_ptr_array_index(env->codeArray, ++PC);
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
                                bnArray* ary = bnPopStack(frame->vStack);
                                for (int i = 0; i < size; i++) {
                                        g_ptr_array_index(ary->arr,
                                                          size - (i + 1)) =
                                            bnPopStack(frame->vStack);
                                }
                                bnDeleteFrame(sub);
                                bnPushStack(frame->vStack, ary);
                                bnGC(bone);
                                break;
                        }
                        case BN_OP_GEN_LAMBDA_BEGIN: {
                                bnLambda* lmb =
                                    bnNewLambda(bone, BN_LAMBDA_SCRIPT);
                                int line =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                lmb->filename = env->filename;
                                lmb->lineno = line;
                                lmb->u.vEnv = bnNewEnviroment(env->filename);
                                lmb->u.vEnv->lineOffset = line;
                                // is instance base?
                                int parameterLen =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                for (int i = 0; i < parameterLen; i++) {
                                        lmb->parameters = g_list_append(
                                            lmb->parameters,
                                            g_ptr_array_index(env->codeArray,
                                                              ++PC));
                                }
                                // length of named return
                                int namedReturnLen =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                g_ptr_array_add(lmb->u.vEnv->codeArray,
                                                BN_OP_NOP);
                                for (int i = 0; i < namedReturnLen; i++) {
                                        lmb->returns = g_list_append(
                                            lmb->returns,
                                            g_ptr_array_index(env->codeArray,
                                                              ++PC));
                                }
                                // collect all variables
                                GHashTableIter hashIter;
                                g_hash_table_iter_init(&hashIter,
                                                       frame->variableTable);
                                gpointer k, v;
                                while (
                                    g_hash_table_iter_next(&hashIter, &k, &v)) {
                                        g_hash_table_replace(lmb->outer, k, v);
                                }
                                int lambdaNest = 1;
                                // generate code
                                while (1) {
                                        assert(PC < env->codeArray->len);
                                        gpointer data = g_ptr_array_index(
                                            env->codeArray, ++PC);
                                        // bug if index of string view equal
                                        // BN_OP_GEN_LAMBDA_END
                                        if (bnOperands((bnOpcode)data) == 1) {
                                                if (data == BN_OP_GOTO ||
                                                    data == BN_OP_GOTO_IF ||
                                                    data == BN_OP_GOTO_ELSE) {
                                                        g_ptr_array_add(
                                                            lmb->u.vEnv
                                                                ->codeArray,
                                                            data);
                                                        bnLabel* clone = bnNewLabel(
                                                            ((bnLabel*)
                                                                 g_ptr_array_index(
                                                                     env->codeArray,
                                                                     ++PC))
                                                                ->pos);
                                                        g_ptr_array_add(
                                                            lmb->u.vEnv
                                                                ->codeArray,
                                                            clone);
                                                        g_ptr_array_add(
                                                            lmb->u.vEnv->labels,
                                                            clone);
                                                } else {
                                                        g_ptr_array_add(
                                                            lmb->u.vEnv
                                                                ->codeArray,
                                                            data);
                                                        g_ptr_array_add(
                                                            lmb->u.vEnv
                                                                ->codeArray,
                                                            g_ptr_array_index(
                                                                env->codeArray,
                                                                ++PC));
                                                }
                                                continue;
                                        }
                                        if (data == BN_OP_GEN_LAMBDA_BEGIN) {
                                                g_ptr_array_add(
                                                    lmb->u.vEnv->codeArray,
                                                    data);
                                                line = g_ptr_array_index(
                                                    env->codeArray, ++PC);
                                                g_ptr_array_add(
                                                    lmb->u.vEnv->codeArray,
                                                    line);
                                                // add params
                                                parameterLen =
                                                    g_ptr_array_index(
                                                        env->codeArray, ++PC);
                                                g_ptr_array_add(
                                                    lmb->u.vEnv->codeArray,
                                                    parameterLen);
                                                for (int i = 0;
                                                     i < parameterLen; i++) {
                                                        g_ptr_array_add(
                                                            lmb->u.vEnv
                                                                ->codeArray,
                                                            g_ptr_array_index(
                                                                env->codeArray,
                                                                ++PC));
                                                }
                                                // add returns
                                                namedReturnLen =
                                                    g_ptr_array_index(
                                                        env->codeArray, ++PC);
                                                g_ptr_array_add(
                                                    lmb->u.vEnv->codeArray,
                                                    namedReturnLen);
                                                g_ptr_array_add(
                                                    lmb->u.vEnv->codeArray,
                                                    BN_OP_NOP);
                                                for (int i = 0;
                                                     i < namedReturnLen; i++) {
                                                        lmb->returns =
                                                            g_list_append(
                                                                lmb->returns,
                                                                g_ptr_array_index(
                                                                    env->codeArray,
                                                                    ++PC));
                                                }
                                                lambdaNest++;
                                        } else if (data ==
                                                   BN_OP_GEN_LAMBDA_END) {
                                                if (--lambdaNest == 0) {
                                                        break;
                                                }
                                                g_ptr_array_add(
                                                    lmb->u.vEnv->codeArray,
                                                    data);

                                        } else {
                                                g_ptr_array_add(
                                                    lmb->u.vEnv->codeArray,
                                                    data);
                                        }
                                }
                                bnPushStack(frame->vStack, lmb);
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
                                // collect all hidden variables
                                GHashTableIter hashIter;
                                g_hash_table_iter_init(&hashIter, obj->table);
                                gpointer k, v;
                                while (
                                    g_hash_table_iter_next(&hashIter, &k, &v)) {
                                        bnStringView kView = k;
                                        const char* kStr =
                                            bnView2Str(bone->pool, kView);
                                        if (g_str_has_prefix(kStr, "$$_")) {
                                                const char* origName = kStr + 3;
                                                bnStringView origView =
                                                    bnIntern(bone->pool,
                                                             origName);
                                                g_hash_table_replace(
                                                    frame->variableTable,
                                                    origView, v);
                                                g_hash_table_iter_remove(
                                                    &hashIter);
                                        }
                                }
                                break;
                        }
                        case BN_OP_OBJECT_INJECTION: {
                                bnObject* src = bnPopStack(frame->vStack);
                                bnObject* dst = bnPopStack(frame->vStack);
                                // collect all hidden variables
                                GHashTableIter hashIter;
                                g_hash_table_iter_init(&hashIter, src->table);
                                gpointer k, v;
                                while (
                                    g_hash_table_iter_next(&hashIter, &k, &v)) {
                                        bnStringView kView = k;
                                        const char* kStr =
                                            bnView2Str(bone->pool, kView);
                                        if (g_str_has_prefix(kStr, "$$_")) {
                                                const char* origName = kStr + 3;
                                                bnStringView origView =
                                                    bnIntern(bone->pool,
                                                             origName);
                                                g_hash_table_replace(
                                                    dst->table, origView, v);
                                                g_hash_table_iter_remove(
                                                    &hashIter);
                                        }
                                }
                                bnPushStack(frame->vStack, dst);
                                break;
                        }
                        case BN_OP_STORE: {
                                bnStringView name =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                bnObject* value = bnPopStack(frame->vStack);
                                g_hash_table_replace(frame->variableTable,
                                                     GINT_TO_POINTER((int)name),
                                                     value);
                                assert(value != NULL);
                                break;
                        }
                        case BN_OP_LOAD: {
                                bnStringView name =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                bnObject* value = g_hash_table_lookup(
                                    frame->variableTable,
                                    GINT_TO_POINTER((int)name));
                                const char* str = bnView2Str(bone->pool, name);
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
                                bnStringView name =
                                    g_ptr_array_index(env->codeArray, ++PC);
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
                                bnStringView name =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                gpointer data =
                                    g_hash_table_lookup(container->table, name);
                                bnObject* obj = data;
                                const char* str = bnView2Str(bone->pool, name);
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
                                bnLabel* jmp =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                PC = jmp->pos;
                                break;
                        }
                        case BN_OP_GOTO_IF: {
                                bnLabel* jmp =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                bnObject* cond = bnPopStack(frame->vStack);
                                if (bnObject2CBool(cond)) {
                                        PC = jmp->pos;
                                }
                                break;
                        }
                        case BN_OP_GOTO_ELSE: {
                                bnLabel* jmp =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                bnObject* cond = bnPopStack(frame->vStack);
                                if (!bnObject2CBool(cond)) {
                                        PC = jmp->pos;
                                }
                                break;
                        }
                        case BN_OP_RETURN: {
                                PC = env->codeArray->len;
                                break;
                        }
                        case BN_OP_FUNCCALL: {
                                bnLambda* lambda = bnPopStack(frame->vStack);
                                int argc =
                                    g_ptr_array_index(env->codeArray, ++PC);

                                bnPushStack(bone->callStack,
                                            bnCreateStackFrameString(
                                                bone, env, lambda, PC));
                                bnFrame* sub =
                                    bnFuncCall(lambda, bone, frame, argc);
                                if (sub->panic == NULL) {
                                        g_string_free(
                                            bnPopStack(bone->callStack), TRUE);
                                }
                                bnDeleteFrame(sub);
                                bnGC(bone);
                                break;
                        }
                        case BN_OP_DEFER_PUSH: {
                                bnLabel* jmp =
                                    g_ptr_array_index(env->codeArray, ++PC);
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