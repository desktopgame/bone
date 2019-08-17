#ifndef BONE_RUNTIME_FRAME_H
#define BONE_RUNTIME_FRAME_H
#include "../bone.h"
#include "../util/stack.h"
#include "storage.h"
struct bnObject;
struct bnLambda;
struct bnInterpreter;

typedef struct bnFrame {
        int pc;
        int depth;
        bnStack* vStack;
        bnStack* hierarcySelf;
        GHashTable* variableTable;
        bnReference currentCall;
        bnReference panic;
        bnReference register0;
        struct bnFrame* prev;
        struct bnFrame* next;
        GList* snapshots;
} bnFrame;

/**
 * return new instance of bnFrame.
 * @param type
 * @return
 */
bnFrame* bnNewFrame();

/**
 * @param self
 * @return
 */
bnFrame* bnSubFrame(bnFrame* self);

/**
 * @param self
 * @return
 */
struct bnObject* bnReturnValue(bnFrame* self);

/**
 * @param src
 * @param dst
 */
void bnInjectFrame(GHashTable* src, bnFrame* dst);

/**
 * @param bone
 * @param self
 * @return
 */
bnReference bnExportAllVariable(struct bnInterpreter* bone, bnFrame* self);

/**
 * @param frame
 * @param name
 * @param ref
 */
void bnWriteVariable(bnFrame* frame, bnStringView name, bnReference ref);

/**
 * @param frame
 * @param pool
 * @param name
 * @param obj
 */
void bnWriteVariable2(bnFrame* frame, struct bnStringPool* pool,
                      const char* name, bnReference ref);

/**
 * @param frame
 * @param name
 * @return
 */
bnReference bnReadVariable(bnFrame* frame, bnStringView name);

/**
 * @param frame
 * @param pool
 * @param name
 * @return
 */
bnReference bnReadVariable2(bnFrame* frame, struct bnStringPool* pool,
                            const char* name);

/**
 * free a bnFrame.
 * @param self
 */
void bnDeleteFrame(bnFrame* self);
#endif