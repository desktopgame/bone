#ifndef BONE_RUNTIME_SNAPSHOT_H
#define BONE_RUNTIME_SNAPSHOT_H
#include "../glib.h"

typedef struct bnSnapShot {
        int pc;
        GHashTable* table;
} bnSnapShot;

bnSnapShot* bnNewSnapShot(int pc);

void bnDeleteSnapShot(bnSnapShot* self);
#endif