#ifndef BONE_RUNTIME_VM_H
#define BONE_RUNTIME_VM_H
#include "enviroment.h"
#include "frame.h"
#include "interpreter.h"
struct bnObject;
struct bnLambda;

void bnScopeInjection(bnInterpreter* bone, struct bnObject* src, bnFrame* dst);

void bnObjectInjection(bnInterpreter* bone, struct bnObject* src,
                       struct bnObject* dst);

GString* bnCreateStackFrameString(bnInterpreter* bone, bnEnviroment* env,
                                  struct bnLambda* lambda, int PC);

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