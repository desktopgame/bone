#ifndef BONE_RUNTIME_VM_H
#define BONE_RUNTIME_VM_H
#include "enviroment.h"
#include "frame.h"
#include "interpreter.h"

/**
 * @param fp
 * @param stack
 * @param name
 */
void bnDebugStack(FILE* fp, bnStack* stack, const char* name);

/**
 * bnExecute be run a bone.
 * return other than zero if breaked bone vm.
 * @param bone
 * @param env
 * @param frame
 * @return
 */
int bnExecute(bnInterpreter* bone, bnEnviroment* env, bnFrame* frame);
#endif