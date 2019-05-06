#ifndef BONE_RUNTIME_FRAME_H
#define BONE_RUNTIME_FRAME_H
#include "../bone.h"
#include "../util/stack.h"
struct bnObject;
struct bnInterpreter;

typedef struct bnFrame {
        int pc;
        bnStack* vStack;
        bnStack* hierarcySelf;
        GHashTable* variableTable;
        bnStringView panicName;
        struct bnObject* panic;
        struct bnObject* register0;
        struct bnFrame* prev;
        struct bnFrame* next;
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
struct bnObject* bnExportAllVariable(struct bnInterpreter* bone, bnFrame* self);

/**
 * free a bnFrame.
 * @param self
 */
void bnDeleteFrame(bnFrame* self);
#endif