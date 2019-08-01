#ifndef BONE_RUNTIME_STD_H
#define BONE_RUNTIME_STD_H
struct bnFrame;
struct bnInterpreter;

// only in debug build
#if DEBUG
void bnStdDebugAssert(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDebugDie(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDebugPrint(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDebugPrintln(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDebugBreak(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDebugDumpTable(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDebugShowInfo(struct bnInterpreter* bone, struct bnFrame* frame);
#endif

// Built-in

void bnStdSystemInclude(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemLoad(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemEval(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemObject(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemString(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemArray(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemExternVar(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemExternDef(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemPanic(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemRecover(struct bnInterpreter* bone, struct bnFrame* frame);

#endif