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
#ifndef SUTL_HASHSET_H
#define SUTL_HASHSET_H

#include "Common.h"
#include "Vector.h"

/**
 * @defgroup Hashset
 * A hashset implementation. This is similar to the \p std::unordered_set
 * from C++ STL.
 *
 * NOTE: In case of hashset, the value itself acts as a key so, the entry contains just one variable
 * that acts as both a key and a value.
 * @{
 */

/*
 * This must be same for all source files that pass
 * SUTLHashset around.
 */
#ifndef SUTL_HASHSET_BUCKET_COUNT
    /**
     * @brief The number of buckets a \p SUTLHashset will have. If it is less than or equal to 0
     * then it is set to 32 which is also the default value if it is not set.
     *
     * More buckets will decrease search time but data will be fragmented across locations in
     * memory. This might cause cache misses and hence slow down accessing the entries.
     *
     * Less buckets will increase search time but data will be less fragmented across locations in
     * memory. This will decrease cache misses and hence accessing the entries won't take much
     * time.
     *
     * Choose this value according to your use case.
     */
    #define SUTL_HASHSET_BUCKET_COUNT 32
#endif

/**
 * @brief It contains the state of a particular hashset instance.
 */
typedef struct SUTLHashset
{
    /**
     * @brief The number of entries in the hashset.
     */
    size_t Size;

    /**
     * @brief The size of the key type of the hashset.
     */
    size_t KeySize;

    /**
     * @brief Don't access this directly. A pointer to key type. This is used to pass parameters to
     * internal functions which allows passing rvalues to them.
     */
    void * ParamK;

    /**
     * @brief Don't access this directly. An array of vector of \p char which stores the keys the
     * hashset. This is parallel to \p Values. It uses char so that the layout remains the same for
     * hashsets with different key types.
     */
    char * Keys[SUTL_HASHSET_BUCKET_COUNT];

    /**
     * @brief The function pointer which hashes the key type of the hashset.
     */
    size_t( * Hash)(const void *);

    /**
     * @brief The function pointer which compares two keys.
     */
    int( * KeyComp)(const void *, const void *);
} SUTLHashset;

/**
 * @brief Creates a new \p SUTLHashset with key type as \p tk, key hash function as \p hash and key
 * compare function as \p cmp.
 *
 * @param tk The key type for the hashmap.
 * @param hash A function of the signature <tt>size_t(const void *)</tt> that hashes \p tk.
 * @param cmp A function of signature <tt>int(const void *, const void *)</tt> that compares two
 * \p tk s for equality. (Similar to the \p == operator)
 *
 * @return A \p SUTLHashmap created according to the parameters given.
 */
#define SUTLHashsetNew(tk, hash, cmp)      SUTL_InternalHashsetNew(sizeof(tk), hash, cmp)

/**
 * @brief Frees a \p SUTLHashset which was created using \p SUTLHashsetNew.
 *
 * @param hs The \p SUTLHashset to free.
 */
#define SUTLHashsetFree(hs)                SUTL_InternalHashsetFree(&hs)

/**
 * @brief Gets entry with key \p k in \p hs.
 *
 * @param tk The key type of \p hs.
 * @param hs The hashset to get from.
 * @param k The key to search for.
 *
 * @return A <tt>tk *</tt> that points to the required entry. If \p k doesn't exist in \p hs then
 * it is \p NULL.
 */
#define SUTLHashsetGet(tk, hs, k)          (*(tk *)hs.ParamK = k, (tk *)SUTL_InternalHashsetGet(&hs))

/**
 * @brief Inserts an entry with key \p k in \p hs.
 *
 * @param tk The key type of \p hs.
 * @param hs The hashset to insert to.
 * @param k The key to insert.
 *
 * @return A <tt>tk *</tt> that points to the inserted entry.
 */
#define SUTLHashsetInsert(tk, hs, k)       (*(tk *)hs.ParamK = k, (tk *)SUTL_InternalHashsetInsert(&hs))

/**
 * @brief Erases an entry with key \p k in \p hs.
 *
 * @param tk The key type of \p hs.
 * @param hs The hashset to erase from.
 * @param k The key of the entry to erase.
 */
#define SUTLHashsetErase(tk, hs, k)        (*(tk *)hs.ParamK = k, SUTL_InternalHashsetErase(&hs))

/**
 * @brief Executes \p expr for every entry in \p hs.
 *
 * @param tk The key type of \p hs.
 * @param hs The hashset to iterate.
 * @param name The prefix for current entry.
 * @param expr The code block to execute for every entry.
 */
#define SUTLHashsetEach(tk, hs, name, expr) \
    {\
        size_t i, j;\
        for (i = 0; i < SUTL_HASHSET_BUCKET_COUNT; i++)\
        {\
            for (j = 0; j < SUTLVectorSize(hs.Keys[i]) / hs.KeySize; j++)\
            {\
                tk * name = ((tk *)hs.Keys[i] + j);\
                expr\
            }\
        }\
    }

/**
 * @}
 *
 * @defgroup Internal
 * For internal use of the library. Don't use these directly.
 * @{
 */
SUTLHashset SUTL_InternalHashsetNew(size_t keysize, size_t( * hash)(const void *), int ( * keycomp)(const void *, const void *));
void SUTL_InternalHashsetFree(SUTLHashset * hs);
void * SUTL_InternalHashsetInsert(SUTLHashset * hs);
void SUTL_InternalHashsetErase(SUTLHashset * hs);
void * SUTL_InternalHashsetGet(SUTLHashset * hs);
/**
 * @{
 */

#ifdef SUTL_IMPLEMENTATION
    SUTLHashset SUTL_InternalHashsetNew(size_t keysize, size_t( * hash)(const void *), int ( * keycomp)(const void *, const void *))
    {
        SUTLHashset hs;

        /*
         * Initialize the members of `hs`.
         */
        hs.Size = 0;
        hs.KeySize = keysize;
        hs.ParamK = SUTLVectorNew(char);
        hs.Hash = hash;
        hs.KeyComp = keycomp;

        /*
         * Allocate enough memory to store 1 key in `ParamK`.
         */
        SUTLVectorResize(hs.ParamK, hs.KeySize);

        size_t i;

        /*
         * Initialize all the buckets.
         */
        for (i = 0; i < SUTL_HASHSET_BUCKET_COUNT; i++)
            hs.Keys[i] = SUTLVectorNew(char);

        return hs;
    }

    void SUTL_InternalHashsetFree(SUTLHashset * hs)
    {
        size_t i;

        /*
         * Free all the buckets.
         */
        for (i = 0; i < SUTL_HASHSET_BUCKET_COUNT; i++)
            SUTLVectorFree(hs->Keys[i]);

        /*
         * Free the memory allocated for parameter variable.
         */
        SUTLVectorFree(hs->ParamK);
    }

    void * SUTL_InternalHashsetInsert(SUTLHashset * hs)
    {
        /*
         * Calculate the index of the key stored in `ParamK`.
         */
        size_t index = hs->Hash(hs->ParamK) % SUTL_HASHSET_BUCKET_COUNT;

        hs->Size++;

        void * entry = NULL;

        size_t i;

        /*
         * Check if the entry with key stored in `ParamK` already exists.
         */
        for (i = 0; i < SUTLVectorSize(hs->Keys[index]) / hs->KeySize; i++)
            if (hs->KeyComp(hs->ParamK, hs->Keys[index] + i * hs->KeySize))
                /*
                 * Initialize `entry` to the entry that already exists.
                 */
                entry = hs->Keys[index] + i * hs->KeySize;

        /*
         * If the entry already exists return it, otherwise proceed to add the new entry.
         */
        if (entry)
            return entry;

        /*
         * Push the new key at the end of the bucket with index `index` and return it.
         */
        return SUTLVectorPushN(hs->Keys[index], hs->ParamK, hs->KeySize);
    }

    void SUTL_InternalHashsetErase(SUTLHashset * hs)
    {
        /*
         * Calculate the index of the key stored in `ParamK`.
         */
        size_t index = hs->Hash(hs->ParamK) % SUTL_HASHSET_BUCKET_COUNT;

        size_t i;

        /*
         * Search the bucket with index `index` for the key stored in `ParamK`.
         */
        for (i = 0; i < SUTLVectorSize(hs->Keys[index]); i++)
        {
            if (hs->KeyComp(hs->ParamK, hs->Keys[index] + i * hs->KeySize))
            {
                /*
                 * Erase the key from the bucket.
                 */
                SUTLVectorEraseN(hs->Keys[index], i * hs->KeySize, hs->KeySize);

                hs->Size--;

                break;
            }
        }
    }

    void * SUTL_InternalHashsetGet(SUTLHashset * hs)
    {
        /*
         * Calculate the index of the key stored in `ParamK`.
         */
        size_t index = hs->Hash(hs->ParamK) % SUTL_HASHSET_BUCKET_COUNT;

        size_t i;

        /*
         * Search the bucket with index `index` for the key stored in `ParamK`.
         */
        for (i = 0; i < SUTLVectorSize(hs->Keys[index]) / hs->KeySize; i++)
            if (hs->KeyComp(hs->ParamK, hs->Keys[index] + i * hs->KeySize))
                /*
                 * Return the value.
                 */
                return hs->Keys[index] + i * hs->KeySize;

        /*
         * If the search fails then `NULL` is returned.
         */
        return NULL;
    }
#endif

#endif
