#ifndef BONE_RUNTIME_VM_H
#define BONE_RUNTIME_VM_H
#include "enviroment.h"
#include "frame.h"

/**
 * bnExecute be run a bone.
 * return other than zero if breaked bone vm.
 * @param env
 * @param frame
 * @return
 */
int bnExecute(bnEnviroment* env, bnFrame* frame);
#endif