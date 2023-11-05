#pragma once

#include <stddef.h>
#include <stdint.h>

#include "intellisense.h"

#define IN
#define OUT
#define OPTIONAL

typedef int8_t INT8;
typedef int16_t INT16;
typedef int32_t INT32;
typedef int64_t INT64;
typedef __int128_t INT128;

typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;
typedef __uint128_t UINT128;

#define VOID void
typedef UINT8 BYTE;
typedef signed char CHAR;
typedef unsigned char UCHAR;
typedef UINT16 WCHAR;

typedef intptr_t INT_PTR;
typedef uintptr_t UINT_PTR;
typedef size_t SIZE_T;
typedef intptr_t SSIZE_T;

#define CONST const

typedef VOID* PVOID;
typedef INT8* PINT8;
typedef INT16* PINT16;
typedef INT32* PINT32;
typedef INT64* PINT64;
typedef INT128* PINT128;
typedef UINT8* PUINT8;
typedef UINT16* PUINT16;
typedef UINT32* PUINT32;
typedef UINT64* PUINT64;
typedef UINT128* PUINT128;
typedef BYTE* PBYTE;
typedef CHAR* PCHAR;
typedef UCHAR* PUCHAR;
typedef WCHAR* PWCHAR;
typedef SIZE_T* PSIZE_T;
typedef SSIZE_T* PSSIZE_T;
typedef INT_PTR* PINT_PTR;
typedef UINT_PTR* PUINT_PTR;
typedef VOID (*PFN_VOID)();

typedef VOID* CONST PCVOID;
typedef INT8* CONST PCINT8;
typedef INT16* CONST PCINT16;
typedef INT32* CONST PCINT32;
typedef INT64* CONST PCINT64;
typedef INT128* CONST PCINT128;
typedef UINT8* CONST PCUINT8;
typedef UINT16* CONST PCUINT16;
typedef UINT32* CONST PCUINT32;
typedef UINT64* CONST PCUINT64;
typedef UINT128* CONST PCUINT128;
typedef BYTE* CONST PCBYTE;
typedef CHAR* CONST PCCHAR;
typedef UCHAR* CONST PCUCHAR;
typedef WCHAR* CONST PCWCHAR;
typedef SIZE_T* CONST PCSIZE_T;
typedef SSIZE_T* CONST PCSSIZE_T;
typedef INT_PTR* CONST PCINT_PTR;
typedef UINT_PTR* CONST PCUINT_PTR;

