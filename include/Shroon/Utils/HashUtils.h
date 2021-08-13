/*
 * Copyright 2021 Saroj Kumar.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SUTL_HASH_UTILS_H
#define SUTL_HASH_UTILS_H

#include "Common.h"
#include "String.h"

/**
 * @defgroup HashUtils
 * This file contains hashing and compare functions for integer types, floating point types and
 * string which can be used with \p SUTLHashmap and \p SUTLHashset.
 *
 * These functions have suffixes which tells what type they target.
 *
 * The following table shows which suffix targets which type:
 *
 *        Suffix    |   Type (const is implied here)
 *     -------------+-------------------------------
 *        uchar     |   unsigned char
 *        ushort    |   unsigned short
 *        uint      |   unsigned int
 *        ulong     |   unsigned long
 *        char      |   signed char
 *        short     |   signed short
 *        int       |   signed int
 *        long      |   signed long
 *        u8        |   uint8_t
 *        u16       |   uint16_t
 *        u32       |   uint32_t
 *        u64       |   uint64_t
 *        i8        |   int8_t
 *        i16       |   int16_t
 *        i32       |   int32_t
 *        i64       |   int64_t
 *        float     |   float
 *        double    |   double
 *        size      |   size_t
 *        ptr       |   void *
 *        string    |   char *
 * @{
 */

/**
 * @brief Hash function with suffix \p suffix.
 */
#define SUTL_HASHFN(suffix) SUTLHash_##suffix

/**
 * @brief Compare function with suffix \p suffix.
 */
#define SUTL_CMPFN(suffix)  SUTLCmp_##suffix

/**
 * @}
 *
 * @defgroup Internal
 * @{
 */
#define SUTL_HASHFN_DECL(suffix)    size_t SUTLHash_##suffix(const void * v);
#define SUTL_CMPFN_DECL(suffix)     int SUTLCmp_##suffix(const void * p0, const void * p1);

SUTL_HASHFN_DECL(uchar);
SUTL_HASHFN_DECL(ushort);
SUTL_HASHFN_DECL(uint);
SUTL_HASHFN_DECL(ulong);
SUTL_HASHFN_DECL(char);
SUTL_HASHFN_DECL(short);
SUTL_HASHFN_DECL(int);
SUTL_HASHFN_DECL(long);

SUTL_HASHFN_DECL(u8);
SUTL_HASHFN_DECL(u16);
SUTL_HASHFN_DECL(u32);
SUTL_HASHFN_DECL(u64);
SUTL_HASHFN_DECL(i8);
SUTL_HASHFN_DECL(i16);
SUTL_HASHFN_DECL(i32);
SUTL_HASHFN_DECL(i64);

SUTL_HASHFN_DECL(size);
SUTL_HASHFN_DECL(ptr);

SUTL_HASHFN_DECL(float);
SUTL_HASHFN_DECL(double);

SUTL_HASHFN_DECL(string);

SUTL_CMPFN_DECL(uchar);
SUTL_CMPFN_DECL(ushort);
SUTL_CMPFN_DECL(uint);
SUTL_CMPFN_DECL(ulong);
SUTL_CMPFN_DECL(char);
SUTL_CMPFN_DECL(short);
SUTL_CMPFN_DECL(int);
SUTL_CMPFN_DECL(long);

SUTL_CMPFN_DECL(u8);
SUTL_CMPFN_DECL(u16);
SUTL_CMPFN_DECL(u32);
SUTL_CMPFN_DECL(u64);
SUTL_CMPFN_DECL(i8);
SUTL_CMPFN_DECL(i16);
SUTL_CMPFN_DECL(i32);
SUTL_CMPFN_DECL(i64);

SUTL_CMPFN_DECL(size);
SUTL_CMPFN_DECL(ptr);

SUTL_CMPFN_DECL(float);
SUTL_CMPFN_DECL(double);

SUTL_CMPFN_DECL(string);
/**
 * @}
 */

#ifdef SUTL_IMPLEMENTATION
    #define SUTL_HASHFN_DEF(suffix, expr) \
        size_t SUTLHash_##suffix(const void * v)\
        {\
            size_t hash;\
            expr\
            return hash;\
        }

    #define SUTL_HASHFN_DEF_PRIMITIVE(suffix, t) SUTL_HASHFN_DEF(suffix, hash = *(const t *)v;)

    #define SUTL_CMPFN_DEF(suffix, expr) \
        int SUTLCmp_##suffix(const void * p0, const void * p1)\
        {\
            int res = 0;\
            expr\
            return res;\
        }

    #define SUTL_CMPFN_DEF_PRIMITIVE(suffix, t) SUTL_CMPFN_DEF(suffix, res = *(const t *)p0 == *(const t *)p1;)

    SUTL_HASHFN_DEF_PRIMITIVE(uchar,    unsigned char)
    SUTL_HASHFN_DEF_PRIMITIVE(ushort,   unsigned short)
    SUTL_HASHFN_DEF_PRIMITIVE(uint,     unsigned int)
    SUTL_HASHFN_DEF_PRIMITIVE(ulong,    unsigned long)
    SUTL_HASHFN_DEF_PRIMITIVE(char,     signed char)
    SUTL_HASHFN_DEF_PRIMITIVE(short,    signed short)
    SUTL_HASHFN_DEF_PRIMITIVE(int,      signed int)
    SUTL_HASHFN_DEF_PRIMITIVE(long,     signed long)

    SUTL_HASHFN_DEF_PRIMITIVE(u8,       uint8_t)
    SUTL_HASHFN_DEF_PRIMITIVE(u16,      uint16_t)
    SUTL_HASHFN_DEF_PRIMITIVE(u32,      uint32_t)
    SUTL_HASHFN_DEF_PRIMITIVE(u64,      uint64_t)
    SUTL_HASHFN_DEF_PRIMITIVE(i8,       int8_t)
    SUTL_HASHFN_DEF_PRIMITIVE(i16,      int16_t)
    SUTL_HASHFN_DEF_PRIMITIVE(i32,      int32_t)
    SUTL_HASHFN_DEF_PRIMITIVE(i64,      int64_t)

    SUTL_HASHFN_DEF_PRIMITIVE(size,     size_t)
    SUTL_HASHFN_DEF(ptr,                hash = (size_t)*(void **)v;)

    SUTL_HASHFN_DEF_PRIMITIVE(float,    float);
    SUTL_HASHFN_DEF_PRIMITIVE(double,   double);

    SUTL_HASHFN_DEF(string,
        hash = SIZE_MAX;
        size_t * buf = (size_t *)v;
        size_t bufSize = SUTLStringSize(v) / sizeof(size_t);
        size_t remSize = SUTLStringSize(v) % sizeof(size_t);
        size_t i;
        for (i = 0; i < bufSize; i++)
            hash ^= buf[i];
        if (remSize)
            for (i = 0; i < remSize; i++)
                ((char *)hash)[i] ^= ((SUTLString)v)[bufSize * sizeof(size_t) + i];
    )

    SUTL_CMPFN_DEF_PRIMITIVE(uchar,     unsigned char)
    SUTL_CMPFN_DEF_PRIMITIVE(ushort,    unsigned short)
    SUTL_CMPFN_DEF_PRIMITIVE(uint,      unsigned int)
    SUTL_CMPFN_DEF_PRIMITIVE(ulong,     unsigned long)
    SUTL_CMPFN_DEF_PRIMITIVE(char,      signed char)
    SUTL_CMPFN_DEF_PRIMITIVE(short,     signed short)
    SUTL_CMPFN_DEF_PRIMITIVE(int,       signed int)
    SUTL_CMPFN_DEF_PRIMITIVE(long,      signed long)

    SUTL_CMPFN_DEF_PRIMITIVE(u8,        uint8_t)
    SUTL_CMPFN_DEF_PRIMITIVE(u16,       uint16_t)
    SUTL_CMPFN_DEF_PRIMITIVE(u32,       uint32_t)
    SUTL_CMPFN_DEF_PRIMITIVE(u64,       uint64_t)
    SUTL_CMPFN_DEF_PRIMITIVE(i8,        int8_t)
    SUTL_CMPFN_DEF_PRIMITIVE(i16,       int16_t)
    SUTL_CMPFN_DEF_PRIMITIVE(i32,       int32_t)
    SUTL_CMPFN_DEF_PRIMITIVE(i64,       int64_t)

    SUTL_CMPFN_DEF_PRIMITIVE(size,      size_t)
    SUTL_CMPFN_DEF_PRIMITIVE(ptr,       void *)

    SUTL_CMPFN_DEF_PRIMITIVE(float,     float)
    SUTL_CMPFN_DEF_PRIMITIVE(double,    double)

    SUTL_CMPFN_DEF(string, res = SHRN_STRCMP((const char *)p0, (const char *)p1) == 0;)

    #undef SUTL_CMPFN_DEF_PRIMITIVE
    #undef SUTL_CMPFN_DEF

    #undef SUTL_HASHFN_DEF_INTEGER
    #undef SUTL_HASHFN_DEF
#endif

#undef SUTL_CMPFN_DECL
#undef SUTL_HASHFN_DECL

#endif
