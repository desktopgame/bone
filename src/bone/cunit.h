#ifndef BONE_CUNIT_H
#define BONE_CUNIT_H

#if __APPLE__
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#else
#define CU_ASSERT(cond) assert(cond)
#endif
#endif