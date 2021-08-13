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
#ifndef SUTL_VECTOR_H
#define SUTL_VECTOR_H

#include "Common.h"

/**
 * @defgroup Vector
 * A data structure similar to the \p std::vector from C++ STL.
 *
 * It has a layout similar to the following struct where t is the element type:
 *
 *     struct SUTLVector
 *     {
 *         size_t Size;
 *         size_t Capacity;
 *         size_t Elemsize;
 *         t[] Data;
 *     };
 *
 * The whole struct is dynamically allocated and the pointer to \p Data is returned to the user so
 * the subscript operator works properly.
 *
 * This implementation is inspired from stretchy_buffer in stb.
 * @{
 */

/**
 * @brief Gets the size of v.
 *
 * @param v The vector to get the size of.
 */
#define SUTLVectorSize(v)                          (*((size_t *)v - 3))

/**
 * @brief Gets the capacity of v.
 *
 * @param v The vector to get the capacity of.
 */
#define SUTLVectorCapacity(v)                      (*((size_t *)v - 2))

/**
 * @brief Creates a new vector of type \p t.
 *
 * @param t The type of element which the vector will store.
 *
 * @return A <tt>t *</tt> which points to index 0 in the vector.
 */
#define SUTLVectorNew(t)                        ((t *)SUTL_InternalVectorNew(sizeof(t)))

/**
 * @brief Frees a vector.
 *
 * @param v The vector to free. This must be a pointer returned from \p SUTLVectorNew.
 */
#define SUTLVectorFree(v)                       SHRN_FREE((size_t *)v - 3)

/**
 * @brief Reserves memory for \p size elements in \p v.
 *
 * @param v The vector to reserve memory in.
 * @param size The number of elements to reserve memory for. Must be greater than or equal to size
 * of \p v, otherwise nothing is changed.
 */
#define SUTLVectorReserve(v, size)              SUTL_InternalVectorReserve((void **)&v, size)

/**
 * @brief Resizes \p v to \p size elements.
 *
 * @param v The vector to resize.
 * @param size The new size of the vector. Doesn't affect the capacity of vector.
 */
#define SUTLVectorResize(v, size)               SUTL_InternalVectorResize((void **)&v, size)

/**
 * @brief Inserts \p elem at index \p at into \p v.
 *
 * @param v The vector to insert \p elem in.
 * @param at The index to insert the element at. This will be the index of the new element. Must be
 * less than or equal to the size of \p v, otherwise the insertion fails and \p NULL is returned.
 * @param elem The element to insert inside \p v. Must be an lvalue.
 *
 * @return The pointer to the inserted element. If insertion failed, it is \p NULL.
 */
#define SUTLVectorInsert(v, at, elem)           SUTL_InternalVectorInsertN((void **)&v, at, &elem, 1)

/**
 * @brief Inserts \p count elements from \p ptr at index \p at into \p v.
 *
 * @param v The vector to insert \p elem in.
 * @param at The index to insert the elements at. This will be the index of the first new element.
 * Must be less than or equal to the size of \p v, otherwise the insertion fails and \p NULL is
 * returned.
 * @param ptr Pointer to the elements which will be inserted.
 * @param count The number of elements to insert.
 *
 * @return The pointer to the first inserted element. If insertion failed, it is \p NULL.
 */
#define SUTLVectorInsertN(v, at, ptr, count)    SUTL_InternalVectorInsertN((void **)&v, at, ptr, count)

/**
 * @brief Erases element at index \p at in \p v.
 *
 * @param v The vector to erase element from.
 * @param at The index of element to erase. Must be less than the size of \p v, otherwise nothing
 * is changed.
 */
#define SUTLVectorErase(v, at)                  SUTL_InternalVectorEraseN((void **)&v, at, 1)

/**
 * @brief Erases \p count elements starting from \p at in \p v.
 *
 * @param v The vector to erase element from.
 * @param at The index of first element to erase. Must be less than <tt>size of v - count</tt>,
 * otherwise nothing is changed.
 * @param count The number of elements to erase. Must be less than or equal to size of \p v.
 */
#define SUTLVectorEraseN(v, at, count)          SUTL_InternalVectorEraseN((void **)&v, at, count)

/**
 * @brief Pushes \p elem at the end of \p v. Synonymous to \p SUTLInsert with \p at as
 * size of \p v.
 *
 * @param v The vector to push \p elem in.
 * @param elem The element to push to \p v. Must be an lvalue.
 *
 * @return The pointer to the inserted element. If insertion failed, it is \p NULL.
 */
#define SUTLVectorPush(v, elem)                 SUTLVectorInsert(v, SUTLVectorSize(v), elem)

/**
 * @brief Pushes \p count elements from \p ptr to the end of \p v. Synonymous to
 * \p SUTLInsertN with \p at as size of \p v.
 *
 * @param v The vector to push \p elem in.
 * @param ptr Pointer to the elements which will be inserted.
 * @param count The number of elements to push.
 *
 * @return The pointer to the first inserted element. If insertion failed, it is \p NULL.
 */
#define SUTLVectorPushN(v, ptr, count)         SUTLVectorInsertN(v, SUTLVectorSize(v), ptr, count)

/**
 * @brief Pops element from the back of \p v. Synonymous to SUTLErase with \p at as size of
 * \p v.
 *
 * @param v The vector to pop element from.
 */
#define SUTLVectorPop(v)                        SUTLVectorErase(v, SUTLVectorSize(v) - 1)

/**
 * @brief Pops \p count elements from the back of \p v. Synonymous to \p SUTLEraseN with
 * \p at as size of \p v.
 *
 * @param v The vector to pop elements from.
 * @param count The number of elements to pop. Must be less than or equal to size of \p v.
 */
#define SUTLVectorPopN(v, count)                SUTLVectorEraseN(v, SUTLVectorSize(v) - count, count)

/**
 * @brief Executes \p expr for each element of \p v.
 *
 * @param t The type of element stored in \p v.
 * @param v The vector to iterate.
 * @param name The name of the variable in which current element will be stored.
 * @param expr The code block to execute for each element.
 */
#define SUTLVectorEach(t, v, name, expr) \
    {\
        size_t i = 0;\
        for (i = 0; i < SUTLVectorSize(v); i++)\
        {\
            t * name = v + i;\
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
void * SUTL_InternalVectorNew(size_t elemsize);
void SUTL_InternalVectorReserve(void ** v, size_t size);
void SUTL_InternalVectorResize(void ** v, size_t size);
void * SUTL_InternalVectorInsertN(void ** v, size_t at, const void * ptr, size_t count);
void SUTL_InternalVectorEraseN(void ** v, size_t at, size_t count);
/**
 * @}
 */

#ifdef SUTL_IMPLEMENTATION
    #define SUTLVectorElemsize(v)   (*((size_t *)v - 1))
    #define SUTLVectorOffset(v, i)  (void *)((uint8_t *)v + SUTLVectorElemsize(v) * (i))

    void * SUTL_InternalVectorNew(size_t elemsize)
    {
        /*
         * Allocate memory for internal variables size, capacity and element size.
         */
        void * mem = SHRN_MALLOC(sizeof(size_t) * 3);

        if (!mem)
        {
            SUTLErrorHandler("Memory allocation failed.");
            return mem;
        }

        /*
         * Make sure `mem` points to the start of the array instead of internal variables.
         */
        mem = (size_t *)mem + 3;

        /*
         * Initialize the internal variables.
         */
        SUTLVectorSize(mem) = 0;
        SUTLVectorCapacity(mem) = 0;
        SUTLVectorElemsize(mem) = elemsize;

        return mem;
    }

    void SUTL_InternalVectorReserve(void ** v, size_t size)
    {
        /*
         * Can't reserve memory less than the memory assigned to elements. This can cause loss of
         * data.
         *
         * However, note that this would work if `size` is less than both the capacity and the size
         * of the vector. This allows the extra memory to be shrunk down to the size of the used
         * memory.
         */
        if (size < SUTLVectorSize(*v))
        {
            SUTLErrorHandler("Can't reserve memory less than the amount of memory already in use.");
            return;
        }

        size_t requiredSize =
            sizeof(size_t) * 3              /* For variables size, capacity and element size. */
            + size * SUTLVectorElemsize(*v) /* For elements allocated in the vector. */
        ;

        /*
         * Resize the memory of vector (including the internal variables) to the `requiredSize`.
         */
        *v = SHRN_REALLOC((size_t *)*v - 3, requiredSize);

        if (!*v)
        {
            SUTLErrorHandler("Memory reallocation failed.");
            return;
        }

        /*
         * Make sure `v` points to the start of the array instead of internal variables.:
         */
        *v = (size_t *)*v + 3;

        SUTLVectorCapacity(*v) = size;
    }

    void SUTL_InternalVectorResize(void ** v, size_t size)
    {
        /*
         * Reserve memory only if current memory is not more than required memory.
         */
        if (SUTLVectorSize(*v) < size)
            if (SUTLVectorCapacity(*v) < size)
                /*
                 * For now, it reserves size for 2 extra elements.
                 *
                 * This behaviour might change later.
                 */
                SUTL_InternalVectorReserve(v, size + 2);

        /*
         * Change the size anyway.
         *
         * If current size is less than required size than uninitialized elements are added.
         * If current size is same as required size than nothing happens.
         * If current size is greater than required size than extra elements become "free space".
         */
        SUTLVectorSize(*v) = size;
    }

    void * SUTL_InternalVectorInsertN(void ** v, size_t at, const void * ptr, size_t count)
    {
        /*
         * `at` becomes the index of the first added element hence it must be less than or equal to
         * the original size of vector.
         */
        if (at > SUTLVectorSize(*v))
        {
            SUTLErrorHandler("Insert index must be less than or equal to size.");
            return NULL;
        }

        size_t originalSize = SUTLVectorSize(*v);

        /*
         * Resize the vector to the sum of current size and count. From here, the size of vector
         * is the new size.
         */
        SUTL_InternalVectorResize(v, originalSize + count);

        /*
         * No need to shift elements if `at` is the end of vector.
         */
        if (at != originalSize)
            /*
             * Shift elements from `at` to the right by `count` to make space for the new element.
             */
            SHRN_MEMMOVE(
                SUTLVectorOffset(*v, at + count),
                SUTLVectorOffset(*v, at),
                (originalSize - at) * SUTLVectorElemsize(*v)
            );

        /*
         * Set the contents of the inserted elements to be the contents at `ptr` and return the
         * pointer to the first inserted element.
         */
        return SHRN_MEMCPY(SUTLVectorOffset(*v, at), ptr, SUTLVectorElemsize(*v) * count);
    }

    void SUTL_InternalVectorEraseN(void ** v, size_t at, size_t count)
    {
        /*
         * There must be `count` elements available from index `at`.
         */
        if (at + count > SUTLVectorSize(*v))
        {
            if (at > SUTLVectorSize(*v))
            {
                SUTLErrorHandler("Elements requested to be erased don't exist.");
                return;
            }

            count = SUTLVectorSize(*v) - at;
        }

        /*
         * No need to shift elements if the elements to be removed are at the end of vector.
         */
        if (at + count != SUTLVectorSize(*v))
            /*
             * Shift elements from `at` to the left by `count` to overwrite the elements to erase
             * with the data of elements after it.
             */
            SHRN_MEMMOVE(
                SUTLVectorOffset(*v, at),
                SUTLVectorOffset(*v, at + count),
                (SUTLVectorSize(*v) - count - at) * SUTLVectorElemsize(*v)
            );

        SUTLVectorSize(*v) -= count;
    }

    #undef SUTLVectorOffset
    #undef SUTLVectorElemsize
#endif

#endif
