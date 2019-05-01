#ifndef BONE_RUNTIME_FRAME_H
#define BONE_RUNTIME_FRAME_H
#include "../bone.h"

typedef struct bnFrame {
        int pc;
        GTrashStack* stack;
        GHashTable* variableTable;
} bnFrame;

/**
 * return new instance of bnFrame.
 * @param type
 * @return
 */
bnFrame* bnNewFrame();

/**
 * free a bnFrame.
 * @param self
 */
void bnDeleteFrame(bnFrame* self);
#endif