#ifndef BONE_RUNTIME_FRAME_H
#define BONE_RUNTIME_FRAME_H
#include "../bone.h"

typedef struct bnFrame {
        int pc;
        GTrashStack* stack;
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
 * free a bnFrame.
 * @param self
 */
void bnDeleteFrame(bnFrame* self);
#endif