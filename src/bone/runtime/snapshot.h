#ifndef BONE_RUNTIME_SNAPSHOT_H
#define BONE_RUNTIME_SNAPSHOT_H
#include "../glib.h"

/**
 * bnSnapShot is save a state of frame at execution.
 */
typedef struct bnSnapShot {
        int pc;
        GHashTable* table;
} bnSnapShot;

/**
 * return new instance of bnString.
 * @param pc
 * @return
 */
bnSnapShot* bnNewSnapShot(int pc);

/**
 * free a bnString.
 * @param self
 */
void bnDeleteSnapShot(bnSnapShot* self);
#endif