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
#ifndef SUTL_STRING_H
#define SUTL_STRING_H

#include "Common.h"
#include "Vector.h"

/**
 * @defgroup String
 * A wrapper around vector for \p char with some additional macros.
 * @{
 */

/**
 * @brief This is basically a vector of \p char.
 */
typedef char * SUTLString;

/**
 * @brief Gets the size of \p str.
 *
 * @param str The string to get the size of.
 */
#define SUTLStringSize(str)     SUTLVectorSize(str)

/**
 * @brief Gets the capacity of \p str.
 *
 * @param str The string to get the capacity of.
 */
#define SUTLStringCapacity(str) SUTLVectorCapacity(str)

/**
 * @brief Creates a new string.
 *
 * @return A <tt>char *</tt> which points to index 0 in the string.
 */
#define SUTLStringNew()                         SUTLVectorNew(char)

/**
 * @brief Frees a string.
 *
 * @param str The string to free. This must be a pointer returned from \p SUTLVectorNew.
 */
#define SUTLStringFree(str)                     SUTLVectorFree(str)

/**
 * @brief Reserves memory for \p size characters in \p str.
 *
 * @param str The string to reserve memory in.
 * @param size The number of characters to reserve memory for. Must be greater than or equal to size
 * of \p str, otherwise nothing is changed.
 */
#define SUTLStringReserve(str, size)            SUTLVectorReserve(str, size)

/**
 * @brief Resizes \p str to \p size characters.
 *
 * @param str The string to resize.
 * @param size The new size of the string. Doesn't affect the capacity of string.
 */
#define SUTLStringResize(str, size)             SUTLVectorResize(str, size)

/**
 * @brief Appends \p c to \p str. Synonymous to \p SUTLStringInsert with \p at as size of \p str.
 *
 * @param str The string to append \p c to.
 * @param c The character to append to \p str. Must be an lvalue.
 *
 * @return The pointer to the inserted character. If insertion failed, it is \p NULL.
 */
#define SUTLStringAppendC(str, c)               SUTLVectorPush(str, c)

/**
 * @brief Appends null-terminated string \p ptr to \p str. Synonymous to \p SUTLStringInsertN with
 * \p at as size of \p str.
 *
 * @param str The string to append \p ptr to.
 * @param ptr Null-terminated string which will be appended.
 *
 * @return The pointer to the first inserted character. If insertion failed, it is \p NULL.
 */
#define SUTLStringAppendP(str, ptr)             SUTLVectorPushN(str, ptr, SHRN_STRLEN(ptr))

/**
 * @brief Appends \p count characters from \p ptr to \p str. Synonymous to \p SUTLStringInsertN with
 * \p at as size of \p str.
 *
 * @param str The string to append \p ptr to.
 * @param ptr Pointer to the characters which will be appended.
 * @param count The number of characters to append.
 *
 * @return The pointer to the first inserted character. If insertion failed, it is \p NULL.
 */
#define SUTLStringAppendN(str, ptr, count)      SUTLVectorPushN(str, ptr, count)

/**
 * @brief Pops a character from the back of \p str. Synonymous to SUTLStringErase with \p at as
 * size of \p str.
 *
 * @param str The string to pop a character from.
 */
#define SUTLStringPop(str)                      SUTLVectorPop(str)

/**
 * @brief Pops \p count characters from the back of \p str. Synonymous to \p SUTLStringEraseN with
 * \p at as size of \p str.
 *
 * @param str The string to pop characters from.
 * @param count The number of characters to pop. Must be less than or equal to size of \p str.
 */
#define SUTLStringPopN(str, count)                     SUTLVectorPopN(str, count)

/**
 * @brief Inserts \p c at index \p at into \p str.
 *
 * @param str The string to insert \p c in.
 * @param at The index to insert the character at. This will be the index of the new character. Must be
 * less than or equal to the size of \p str, otherwise the insertion fails and \p NULL is returned.
 * @param c The character to insert inside \p str. Must be an lvalue.
 *
 * @return The pointer to the inserted character. If insertion failed, it is \p NULL.
 */
#define SUTLStringInsertC(str, at, c)           SUTLVectorInsert(str, at, c)

/**
 * @brief Inserts null-terminated string \p ptr at index \p at into \p str.
 *
 * @param str The string to insert \p ptr in.
 * @param at The index to insert the characters at. This will be the index of the first new character.
 * Must be less than or equal to the size of \p str, otherwise the insertion fails and \p NULL is
 * returned.
 * @param ptr Null-terminated string which will be inserted.
 *
 * @return The pointer to the first inserted character. If insertion failed, it is \p NULL.
 */
#define SUTLStringInsertP(str, at, ptr)         SUTLVectorInsertN(str, at, ptr, SHRN_STRLEN(ptr))

/**
 * @brief Inserts \p count characters from \p ptr at index \p at into \p str.
 *
 * @param str The string to insert \p ptr in.
 * @param at The index to insert the characters at. This will be the index of the first new character.
 * Must be less than or equal to the size of \p str, otherwise the insertion fails and \p NULL is
 * returned.
 * @param ptr Pointer to the characters which will be inserted.
 * @param count The number of characters to insert.
 *
 * @return The pointer to the first inserted character. If insertion failed, it is \p NULL.
 */
#define SUTLStringInsertN(str, at, ptr, count)  SUTLVectorInsertN(str, at, ptr, count)

/**
 * @brief Erases character at index \p at in \p str.
 *
 * @param str The string to erase character from.
 * @param at The index of character to erase. Must be less than the size of \p str, otherwise
 * nothing is changed.
 */
#define SUTLStringErase(str, at)                SUTLVectorErase(str, at)

/**
 * @brief Erases \p count characters starting from \p at in \p str.
 *
 * @param str The string to erase character from.
 * @param at The index of first character to erase. Must be less than <tt>size of str - count</tt>,
 * otherwise nothing is changed.
 * @param count The number of characters to erase. Must be less than or equal to size of \p str.
 */
#define SUTLStringEraseN(str, at, count)        SUTLVectorEraseN(str, at, count)

/**
 * @brief Creates a new string of size \p size from index \p at of \p str upto \p size.
 *
 * @param str The string to create the new string from.
 * @param at The index from where \p str will be copied.
 * @param size The number of characters to copy from \p str. If this is 0 then characters upto the
 * end of \p str are copied.
 *
 * @return A new string of size \p size with characters from index \p at of \p str upto \p size.
 */
#define SUTLStringSlice(str, at, size) SUTL_InternalStringSlice(str, at, size)

/**
 * @brief Executes \p expr for each character of \p str.
 *
 * @param str The string to iterate.
 * @param name The name of the variable in which current character will be stored.
 * @param expr The code block to execute for each character.
 */
#define SUTLStringEach(str, name, expr) \
    {\
        size_t i = 0;\
        for (i = 0; i < SUTLVectorSize(v); i++)\
        {\
            char * name = v + i;\
            expr\
        }\
    }

/**
 * @}
 *
 * @defgroup Internal
 * For internal use of the library. Don't use these directly.
 * @{
 */
SUTLString SUTL_InternalStringSlice(SUTLString str, size_t at, size_t size);
/**
 * @}
 */

#ifdef SUTL_IMPLEMENTATION
    SUTLString SUTL_InternalStringSlice(SUTLString str, size_t at, size_t size)
    {
        size_t strSize = SUTLStringSize(str);

        if (at >= strSize)
        {
            SUTLErrorHandler("Invalid index specified for slicing string.");
            return NULL;
        }

        size = size ? size : strSize - at;

        SUTLString slice = SUTLStringNew();

        SUTLStringResize(slice, size);

        size = at + size > strSize ? strSize - at : size;

        SHRN_MEMCPY(slice, str + at, size);

        return slice;
    }
#endif

#endif
