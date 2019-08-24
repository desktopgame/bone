#include "args.h"

static int _argc;
static char** _argv;

void bnInitArgs(int argc, char* argv[]) {
        _argc = argc;
        _argv = argv;
}

int bnArgc() { return _argc; }

char** bnArgv() { return _argv; }
