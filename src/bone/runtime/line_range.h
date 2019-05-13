#ifndef BONE_RUNTIME_LINERANGE_H
#define BONE_RUNTIME_LINERANGE_H

typedef struct bnLineRange {
        int start;
        int end;
        int line;
} bnLineRange;

bnLineRange* bnNewLineRange();

void bnDeleteLineRange(bnLineRange* self);
#endif