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
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
                g_string_append(
                    gbuf, bnView2Str(bone->pool, (bnStringView)iter->data));
#pragma clang diagnostic pop
                iter = iter->next;
                if (iter != NULL) {
                        bnGStringAppendC(gbuf, ' ');
                }
        }
        bnGStringAppendC(gbuf, ')');
        iter = bnGetReturnValueList(lambda);
        bnGStringAppendC(gbuf, '(');
        while (iter != NULL) {
                g_string_append(
                    gbuf, bnView2Str(bone->pool, (bnStringView)iter->data));
                iter = iter->next;
                if (iter != NULL) {
                        bnGStringAppendC(gbuf, ' ');
                }
        }
        bnGStringAppendC(gbuf, ')');
        g_string_append_printf(gbuf, "<%d>", (line + env->lineOffset));
        return gbuf;
}

bnReference bnCreateLambdaInActiveCode(bnInterpreter* bone, bnEnviroment* env,
                                       bnFrame* frame, int* pPC) {
        bnReference lmbRef = bnNewLambda(bone, BN_LAMBDA_SCRIPT);
        bnObject* lmb = bnGetObject(bone->heap, lmbRef);
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
        bnWriteCode(bnGetEnviroment(lmb), BN_OP_NOP);
        for (int i = 0; i < namedReturnLen; i++) {
                bnAddReturnValue(lmb,
                                 g_ptr_array_index(env->codeArray, ++(*pPC)));
        }
        // collect all variables
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, frame->variableTable);
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                assert(v != NULL);
                g_hash_table_replace(bnGetCapturedMap(lmb), k, v);
        }
        int lambdaNest = 1;
        // generate code
        while (1) {
                assert(*pPC < env->codeArray->len);
                bnOpcode data = bnReadCode(env, ++(*pPC));
                // bug if index of string view equal
                // BN_OP_GEN_LAMBDA_END
                if (bnOperands(data) == 1) {
                        if (data == BN_OP_GOTO || data == BN_OP_GOTO_IF ||
                            data == BN_OP_GOTO_ELSE) {
                                bnWriteCode(bnGetEnviroment(lmb), data);
                                bnLabel* clone =
                                    bnNewLabel(((bnLabel*)g_ptr_array_index(
                                                    env->codeArray, ++(*pPC)))
                                                   ->pos);

                                bnWriteLabel(bnGetEnviroment(lmb), clone);
                                g_ptr_array_add(bnGetEnviroment(lmb)->labels,
                                                clone);
                        } else {
                                bnWriteCode(bnGetEnviroment(lmb), data);

                                g_ptr_array_add(bnGetEnviroment(lmb)->codeArray,
                                                g_ptr_array_index(
                                                    env->codeArray, ++(*pPC)));
                        }
                        continue;
                }
                if (data == BN_OP_GEN_LAMBDA_BEGIN) {
                        bnWriteCode(bnGetEnviroment(lmb), data);
                        line = bnReadCode(env, ++(*pPC));
                        bnWriteCode(bnGetEnviroment(lmb), line);
                        // add params
                        parameterLen = bnReadCode(env, ++(*pPC));
                        bnWriteCode(bnGetEnviroment(lmb), parameterLen);
                        for (int i = 0; i < parameterLen; i++) {
                                g_ptr_array_add(bnGetEnviroment(lmb)->codeArray,
                                                g_ptr_array_index(
                                                    env->codeArray, ++(*pPC)));
                        }
                        // add returns
                        namedReturnLen = bnReadCode(env, ++(*pPC));
                        bnWriteCode(bnGetEnviroment(lmb), namedReturnLen);
                        bnWriteCode(bnGetEnviroment(lmb), BN_OP_NOP);
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
                        bnWriteCode(bnGetEnviroment(lmb), data);

                } else {
                        bnWriteCode(bnGetEnviroment(lmb), data);
                }
        }
        return lmbRef;
}

void bnScopeInjection(bnInterpreter* bone, bnObject* src, bnFrame* dst) {
        // collect all hidden variables
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, src->table);
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                bnStringView kView = (bnStringView)k;
                const char* kStr = bnView2Str(bone->pool, kView);
                if (g_str_has_prefix(kStr, "$$_")) {
                        const char* origName = kStr + 3;
                        bnStringView origView = bnIntern(bone->pool, origName);
                        bnWriteVariable(dst, origView, v);
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
                bnStringView kView = (bnStringView)k;
                const char* kStr = bnView2Str(bone->pool, kView);
                if (g_str_has_prefix(kStr, "$$_")) {
                        const char* origName = kStr + 3;
                        bnStringView origView = bnIntern(bone->pool, origName);
                        bnDefine(dst, origView, v);
                        g_hash_table_iter_remove(&hashIter);
                }
        }
}

int bnExecute(bnInterpreter* bone, bnEnviroment* env, bnFrame* frame) {
        static void* addr_table[] = {
            &&LABEL_OP_NOP,
            &&LABEL_OP_DUP,
            &&LABEL_OP_SWAP,
            &&LABEL_OP_GEN_INT,
            &&LABEL_OP_GEN_DOUBLE,
            &&LABEL_OP_GEN_STRING,
            &&LABEL_OP_GEN_ARRAY,
            &&LABEL_OP_GEN_LAMBDA_BEGIN,
            &&LABEL_OP_GEN_LAMBDA_END,
            &&LABEL_OP_SET_REGISTER_0,
            &&LABEL_OP_GET_REGISTER_0,
            &&LABEL_OP_PUSH_SELF,
            &&LABEL_OP_POP_SELF,
            &&LABEL_OP_SCOPE_INJECTION,
            &&LABEL_OP_OBJECT_INJECTION,
            &&LABEL_OP_STORE,
            &&LABEL_OP_LOAD,
            &&LABEL_OP_PUT,
            &&LABEL_OP_GET,
            &&LABEL_OP_GOTO,
            &&LABEL_OP_GOTO_IF,
            &&LABEL_OP_GOTO_ELSE,
            &&LABEL_OP_RETURN,
            &&LABEL_OP_FUNCCALL,
            &&LABEL_OP_GEN_CHAR,
            &&LABEL_OP_DEFER_PUSH,
            &&LABEL_OP_DEFER_BEGIN,
            &&LABEL_OP_DEFER_NEXT,
            &&LABEL_OP_DEFER_END,
            &&LABEL_OP_POP,
            &&LABEL_OP_CLEANUP_INJBUF,
        };
        GList* snapshotIter = NULL;
        bool inDefer = false;
        int PC = 0;
        int codelen = env->codeArray->len;

#define ADDR_NEXT(tpc) \
        (addr_table[(int)g_ptr_array_index(env->codeArray, (tpc))])

#define CHECK_DEFER()                                                       \
                                                                            \
        do {                                                                \
                if (frame->panic && !inDefer) {                             \
                        if (snapshotIter != NULL) {                         \
                                PC = ((bnSnapShot*)snapshotIter->data)->pc; \
                                snapshotIter = snapshotIter->next;          \
                                inDefer = true;                             \
                                goto* ADDR_NEXT(++PC);                      \
                        } else {                                            \
                                if (frame->prev) {                          \
                                        frame->prev->panic = frame->panic;  \
                                }                                           \
                                return 0;                                   \
                        }                                                   \
                }                                                           \
        } while (0)

#define SWITCH_NEXT(tpc)                          \
        do {                                      \
                if ((tpc + 1) < codelen) {        \
                        goto* ADDR_NEXT((++tpc)); \
                } else {                          \
                        return 0;                 \
                }                                 \
        } while (0)

#define LOG(str) ((void)0)
        //#define LOG(str) (fprintf(stderr, "%s %d/%d\n", str, PC, codelen))

        goto* ADDR_NEXT(PC);
LABEL_OP_NOP : {
        LOG("NOP");
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_DUP : {
        LOG("DUP");
        gpointer data = bnPopStack(frame->vStack);
        bnPushStack(frame->vStack, data);
        bnPushStack(frame->vStack, data);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_SWAP : {
        LOG("SWAP");
        void* a = bnPopStack(frame->vStack);
        void* b = bnPopStack(frame->vStack);
        bnPushStack(frame->vStack, a);
        bnPushStack(frame->vStack, b);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GEN_INT : {
        LOG("INT");
        int data = bnReadCode(env, ++PC);
        bnPushStack(frame->vStack, bnNewInteger(bone, data));
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GEN_DOUBLE : {
        LOG("GEN DOUBLE");
        int idx = bnReadCode(env, ++PC);
        double dv = g_array_index(env->doubleConstants, double, idx);
        bnPushStack(frame->vStack, bnNewDouble(bone, dv));
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GEN_STRING : {
        LOG("GEN STRING");
        bnStringView name = bnReadCode(env, ++PC);
        // create array for string
        const char* str = bnView2Str(bone->pool, name);
        int slen = strlen(str);
        bnPushStack(frame->vStack, bnNewInteger(bone, slen));
        bnReference arrFunc = bnReadVariable2(frame, bone->pool, "array");
        bnFrame* sub = bnFuncCall(arrFunc, bone, frame, 1);
        if (sub->panic) {
                frame->panic = sub->panic;
                CHECK_DEFER();
                SWITCH_NEXT(PC);
        }
        // fill by char
        bnReference aryRef = bnPopStack(frame->vStack);
        bnObject* ary = bnGetObject(bone->heap, aryRef);
        for (int i = 0; i < slen; i++) {
                bnSetArrayElementAt(ary, i, bnNewChar(bone, str[i]));
        }
        bnPushStack(frame->vStack, aryRef);
        bnDeleteFrame(sub);
        bnGC(bone);
        // create string by string function
        bnFrame* sub2 = bnFuncCall(bnReadVariable2(frame, bone->pool, "string"),
                                   bone, frame, 1);
        if (sub2->panic) {
                frame->panic = sub->panic;
                CHECK_DEFER();
                SWITCH_NEXT(PC);
        }
        bnDeleteFrame(sub2);
        bnGC(bone);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GEN_ARRAY : {
        LOG("GEN ARRAY");
        int size = bnReadCode(env, ++PC);
        bnPushStack(frame->vStack, bnNewInteger(bone, size));
        bnFrame* sub = bnFuncCall(bnReadVariable2(frame, bone->pool, "array"),
                                  bone, frame, 1);
        if (sub->panic) {
                frame->panic = sub->panic;
                CHECK_DEFER();
                SWITCH_NEXT(PC);
        }
        bnReference aryRef = bnPopStack(frame->vStack);
        bnObject* ary = bnGetObject(bone->heap, aryRef);
        for (int i = 0; i < size; i++) {
                bnSetArrayElementAt(ary, size - (i + 1),
                                    bnPopStack(frame->vStack));
        }
        bnDeleteFrame(sub);
        bnPushStack(frame->vStack, aryRef);
        bnGC(bone);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GEN_LAMBDA_BEGIN : {
        LOG("GEN LAMBDA BEGIN");
        bnPushStack(frame->vStack,
                    bnCreateLambdaInActiveCode(bone, env, frame, &PC));
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GEN_LAMBDA_END : {
        LOG("GEN LAMBDA END");
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_SET_REGISTER_0 : {
        LOG("SET REGISTER0");
        frame->register0 = bnPopStack(frame->vStack);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GET_REGISTER_0 : {
        LOG("GET REGISTER0");
        bnPushStack(frame->vStack, frame->register0);
        frame->register0 = NULL;
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_PUSH_SELF : {
        LOG("PUSH SELF");
        bnPushStack(frame->hierarcySelf, bnPopStack(frame->vStack));
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_POP_SELF : {
        LOG("POP SELF");
        bnPopStack(frame->hierarcySelf);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_SCOPE_INJECTION : {
        LOG("SCOPE INJECTION");
        bnReference ref = bnPopStack(frame->vStack);
        bnObject* obj = bnGetObject(bone->heap, ref);
        bnScopeInjection(bone, obj, frame);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_OBJECT_INJECTION : {
        LOG("OBJECT INJECTION");
        bnReference src = bnPopStack(frame->vStack);
        bnReference dst = bnPopStack(frame->vStack);
        bnObject* srcObj = bnGetObject(bone->heap, src);
        bnObject* dstObj = bnGetObject(bone->heap, dst);
        bnObjectInjection(bone, srcObj, dstObj);
        bnPushStack(frame->vStack, dst);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_STORE : {
        LOG("STORE");
        bnStringView name = bnReadCode(env, ++PC);
        bnReference ref = bnPopStack(frame->vStack);
        assert(ref != NULL);
        g_hash_table_replace(frame->variableTable, GINT_TO_POINTER((int)name),
                             ref);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_LOAD : {
        LOG("LOAD");
        bnStringView name = bnReadCode(env, ++PC);
        bnReference ref = bnReadVariable(frame, name);
        bnObject* value = bnGetObject(bone->heap, ref);
        const char* str = bnView2Str(bone->pool, name);
        assert(str != NULL);
        if (value == NULL) {
                char buf[256];
                sprintf(buf, "is'nt found local variable: %s", str);
                bnPanic(bone, bnNewString(bone, bnIntern(bone->pool, buf)));
                CHECK_DEFER();
                SWITCH_NEXT(PC);
        }
        assert(value != NULL);
        bnPushStack(frame->vStack, ref);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_PUT : {
        LOG("PUT");
        bnReference containerRef = bnPopStack(frame->vStack);
        bnObject* container = bnGetObject(bone->heap, containerRef);
        if (container == NULL) {
                bnPanic(bone, bnNewString2(bone, "receiver is null"));
                CHECK_DEFER();
                SWITCH_NEXT(PC);
        }
        bnReference value = bnPopStack(frame->vStack);
        bnStringView name = bnReadCode(env, ++PC);
        bnDefine(container, name, value);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GET : {
        LOG("GET");
        bnReference containerRef = bnPopStack(frame->vStack);
        bnObject* container = bnGetObject(bone->heap, containerRef);

        if (container == NULL) {
                bnPanic(bone, bnNewString2(bone, "receiver is null"));
                CHECK_DEFER();
                SWITCH_NEXT(PC);
        }
        bnStringView name = bnReadCode(env, ++PC);
        bnReference objRef = bnLookup(container, name);
        bnObject* obj = bnGetObject(bone->heap, objRef);
        const char* str = bnView2Str(bone->pool, name);
        assert(str != NULL);
        if (obj == NULL) {
                char buf[256];
                sprintf(buf, "is'nt found member: %s", str);
                bnPanic(bone, bnNewString(bone, bnIntern(bone->pool, buf)));
                CHECK_DEFER();
                SWITCH_NEXT(PC);
        }
        assert(obj != NULL);
        bnPushStack(frame->vStack, objRef);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GOTO : {
        LOG("GOTO");
        bnLabel* jmp = bnReadLabel(env, ++PC);
        PC = jmp->pos;
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GOTO_IF : {
        LOG("GOTO IF");
        bnLabel* jmp = bnReadLabel(env, ++PC);
        bnReference condRef = bnPopStack(frame->vStack);
        bnObject* cond = bnGetObject(bone->heap, condRef);
        if (bnGetBoolValue(cond)) {
                PC = jmp->pos;
        }
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GOTO_ELSE : {
        LOG("GOTO ELSE");
        bnLabel* jmp = bnReadLabel(env, ++PC);
        bnReference condRef = bnPopStack(frame->vStack);
        bnObject* cond = bnGetObject(bone->heap, condRef);
        if (!bnGetBoolValue(cond)) {
                PC = jmp->pos;
        }
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_RETURN : {
        LOG("RETURN");
        PC = env->codeArray->len;
        CHECK_DEFER();
}
LABEL_OP_FUNCCALL : {
        LOG("FUNCCALL");
        bnReference objRef = bnPopStack(frame->vStack);
        bnObject* obj = bnGetObject(bone->heap, objRef);
        if (obj->type != BN_OBJECT_LAMBDA) {
                frame->panic = bnNewString2(bone,
                                            "shoud be receiver is "
                                            "closure object");
                CHECK_DEFER();
                SWITCH_NEXT(PC);
        }
        int argc = bnReadCode(env, ++PC);

        bnPushStack(bone->callStack,
                    bnCreateStackFrameString(bone, env, obj, PC));
        bnFrame* sub = bnFuncCall(objRef, bone, frame, argc);
        if (sub->panic == NULL) {
                g_string_free(bnPopStack(bone->callStack), TRUE);
        }
        bnDeleteFrame(sub);
        bnGC(bone);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_GEN_CHAR : {
        LOG("GEN CHAR");
        char c = bnReadCode(env, ++PC);
        bnPushStack(frame->vStack, bnNewChar(bone, c));
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_DEFER_PUSH : {
        LOG("DEFER PUSH");
        bnLabel* jmp = bnReadLabel(env, ++PC);
        bnSnapShot* sn = bnNewSnapShot(jmp->pos);
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, frame->variableTable);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                assert(v != NULL);
                g_hash_table_replace(sn->table, k, v);
        }
        frame->snapshots = snapshotIter = g_list_append(frame->snapshots, sn);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_DEFER_BEGIN : {
        LOG("DEFER BEGIN");
        int deferNest = 1;
        while (1) {
                assert(PC < env->codeArray->len);
                bnOpcode data = bnReadCode(env, ++PC);
                // bug if index of string view equal
                // BN_OP_GEN_LAMBDA_END
                if (bnOperands(data) == 1) {
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
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_DEFER_NEXT : {
        LOG("DEFER NEXT");
        if (snapshotIter != NULL) {
                PC = ((bnSnapShot*)snapshotIter->data)->pc;
                snapshotIter = snapshotIter->next;
        } else {
                PC = env->codeArray->len;
                inDefer = false;
        }
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_DEFER_END : {
        LOG("DEFER END");
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_POP : {
        LOG("POP");
        bnPopStack(frame->vStack);
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}
LABEL_OP_CLEANUP_INJBUF : {
        LOG("CLEANUP INJBUF");
        bnCleanupInjectionBuffer(
            bone->pool, bnGetObject(bone->heap, bnPeekStack(frame->vStack)));
        CHECK_DEFER();
        SWITCH_NEXT(PC);
}

        return 0;
}