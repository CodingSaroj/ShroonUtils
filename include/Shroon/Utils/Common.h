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
#ifndef SUTL_COMMON_H
#define SUTL_COMMON_H

#include <stdint.h>
#include <stddef.h>

/**
 * @file Common.h
 * This file defines special macros which will be used instead of normal standard library functions.
 *
 * This allows the possibility of a custom implementation of the standard functions (Useful for
 * using this library without linking the standard library).
 *
 * These macros use prefix \p SHRN instead of \p SUTL which allows the custom functions to be
 * defined only once for all Shroon projects.
 */

#ifdef SHRN_NO_USE_STDLIB_H
    #ifndef SHRN_MALLOC
        #error "`SHRN_MALLOC` must be defined if `SHRN_NO_USE_STDLIB_H` is defined."
    #endif

    #ifndef SHRN_REALLOC
        #error "`SHRN_REALLOC` must be defined if `SHRN_NO_USE_STDLIB_H` is defined."
    #endif

    #ifndef SHRN_FREE
        #error "`SHRN_FREE` must be defined if `SHRN_NO_USE_STDLIB_H` is defined."
    #endif
#else
    #include <stdlib.h>

    #define SHRN_MALLOC(size)           malloc(size)
    #define SHRN_REALLOC(oldptr, size)  realloc(oldptr, size)
    #define SHRN_FREE(ptr)              free(ptr)
#endif

#ifdef SHRN_NO_USE_STRING_H
    #ifndef SHRN_MEMCPY
        #warning "`SHRN_MEMCPY` should be defined if `SHRN_NO_USE_STRING_H` is defined, otherwise, a (possibly less performant) custom implementation is used."

        void * SUTL_InternalMemcpy(void * dst, const void * src, size_t size)

        #ifdef SUTL_IMPLEMENTATION
            void * SUTL_InternalMemcpy(void * dst, const void * src, size_t size)
            {
                while (size--)
                    ((uint8_t *)dst)[size] = ((uint8_t *)src)[size];

                return dst;
            }
        #endif

        #define SHRN_MEMCPY(dst, src, size) SUTL_InternalMemcpy(dst, src, size)
    #endif

    #ifndef SHRN_MEMMOVE
        #warning "`SHRN_MEMMOVE` should be defined if `SHRN_NO_USE_STRING_H` is defined, otherwise, a (possibly less performant) custom implementation is used."

        void * SUTL_InternalMemmove(void * dst, const void * src, size_t size)

        #ifdef SUTL_IMPLEMENTATION
            void * SUTL_InternalMemmove(void * dst, const void * src, size_t size)
            {
                void * mem = malloc(size);

                size_t i = size;

                while (i--)
                    ((uint8_t *)mem)[i] = ((uint8_t *)src)[i];

                i = size;

                while (i--)
                    ((uint8_t *)dst)[i] = ((uint8_t *)mem)[i];

                free(mem);

                return dst;
            }
        #endif

        #define SHRN_MEMMOVE(dst, src, size) SUTL_InternalMemmove(dst, src, size)
    #endif

    #ifndef SHRN_MEMSET
        #warning `SHRN_MEMSET` should be defined if `SHRN_NO_USE_STRING_H` is defined, otherwise, a (possibly less performant) custom implementation is used.

        void * SUTL_InternalMemset(void * ptr, const void * val, size_t size)

        #ifdef SUTL_IMPLEMENTATION
            void * SUTL_InternalMemset(void * ptr, const void * val, size_t size)
            {
                while (size--)
                    ((uint8_t *)ptr)[size] = val;

                return ptr;
            }
        #endif

        #define SHRN_MEMSET(ptr, val, size) SUTL_InternalMemcpy(ptr, val, size)
    #endif

    #ifndef SHRN_STRLEN
        #warning "`SHRN_STRLEN` should be defined if `SHRN_NO_USE_STRING_H` is defined, otherwise, a (possibly less performant) custom implementation is used."

        size_t SUTL_InternalStrlen(const char * str);

        #ifdef SUTL_IMPLEMENTATION
            size_t SUTL_InternalStrlen(const char * str)
            {
                size_t size = 0;

                while (str[size])
                    size++;

                return size;
            }
        #endif

        #define SHRN_STRLEN(str) SUTL_InternalStrlen(str)
    #endif

    #ifndef SHRN_STRCMP
        #warning "`SHRN_STRCMP` should be defined if `SHRN_NO_USE_STRING_H` is defined, otherwise, a (possibly less performant) custom implementation is used."

        int SUTL_InternalStrcmp(const char * str0, const char * str1);

        #ifdef SUTL_IMPLEMENTATION
            int SUTL_InternalStrcmp(const char * str0, const char * str1)
            {
                size_t len = SHRN_STRLEN(str0);
                size_t len1 = SHRN_STRLEN(str1);

                len = len1 < len ? len1 : len;
                
                for (size_t i = 0; i < len; i++)
                    if (str0[i] != str1[i])
                        return str0[i] - str1[i];

                return 0;
            }
        #endif
        
        #define SHRN_STRCMP(str0, str1) SUTL_InternalStrcmp(str0, str1)
    #endif
#else
    #include <string.h>

    #define SHRN_MEMCPY(dst, src, size)     memcpy(dst, src, size)
    #define SHRN_MEMMOVE(dst, src, size)    memmove(dst, src, size)
    #define SHRN_MEMSET(ptr, val, size)     memset(ptr, val, size)
    #define SHRN_STRLEN(str)                strlen(str)
    #define SHRN_STRCMP(str0, str1)         strcmp(str0, str1)
#endif

#include "ErrorHandler.h"

#endif
