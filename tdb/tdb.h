#ifndef TDB_H
#define TDB_H

#include <inttypes.h>
#include "common_types.h"

void DebugRawByteCode(byte* code, int codelen);
void Decompile(byte* code, int codelen);

#endif