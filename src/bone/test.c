#include "test.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include "bone.h"

static int bnParse() { return 0; }

void bnParseTest() { CU_ASSERT(bnParse() == 0); }