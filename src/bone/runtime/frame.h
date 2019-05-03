#ifndef BONE_RUNTIME_FRAME_H
#define BONE_RUNTIME_FRAME_H
#include "../bone.h"
#include "../util/stack.h"
struct bnObject;

typedef struct bnFrame {
        int pc;
        bnStack* vStack;
        GHashTable* variableTable;
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
 * free a bnFrame.
 * @param self
 */
void bnDeleteFrame(bnFrame* self);
#endif