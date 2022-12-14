#ifndef TDB_H
#define TDB_H

#include <inttypes.h>
typedef uint8_t byte;

void DebugRawByteCode(byte* code, int codelen);
void DebugFile(char* filename);
void Decompile(byte* code, int codelen);

#endif