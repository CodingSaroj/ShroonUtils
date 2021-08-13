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
#ifndef SUTL_SUTL_HASHMAP_H
#define SUTL_SUTL_HASHMAP_H

#include "Common.h"
#include "Vector.h"

/**
 * @defgroup Hashmap
 * A hashmap (also called hashtable) implementation. This is similar to the \p std::unordered_map
 * from C++ STL.
 * @{
 */

/*
 * This must be same for all source files that pass
 * SUTLHashmap around.
 */
#if !defined(SUTL_HASHMAP_BUCKET_COUNT) || SUTL_HASHMAP_BUCKET_COUNT <= 0
    /**
     * @brief The number of buckets a \p SUTLHashmap will have. If it is less than or equal to 0
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
    #define SUTL_HASHMAP_BUCKET_COUNT 32
#endif

/**
 * @brief It contains the state of a particular hashmap instance.
 */
typedef struct SUTLHashmap
{
    /**
     * @brief The number of entries in the hashmap.
     */
    size_t Size;

    /**
     * @brief The size of the key type of the hashmap.
     */
    size_t KeySize;

    /**
     * @brief The size of the value type the hashmap.
     */
    size_t ValueSize;

    /**
     * @brief Don't access this directly. A pointer to key type. This is used to pass parameters to
     * internal functions which allows passing rvalues to them.
     */
    void * ParamK;

    /**
     * @brief Don't access this directly. A pointer to value type. This is used to pass parameters
     * to internal functions which allows passing rvalues to them.
     */
    void * ParamV;

    /**
     * @brief Don't access this directly. An array of vector of \p char which stores the keys the
     * hashmap. This is parallel to \p Values. It uses char so that the layout remains the same for
     * hashmaps with different key types.
     */
    char * Keys[SUTL_HASHMAP_BUCKET_COUNT];

    /**
     * @brief Don't access this directly. An array of vector of \p char which stores the keys the
     * hashmap. This is parallel to \p Keys. It uses char so that the layout remains the same for
     * hashmaps with different value types.
     */
    char * Values[SUTL_HASHMAP_BUCKET_COUNT];

    /**
     * @brief The function pointer which hashes the key type of the hashmap.
     */
    size_t( * Hash)(const void *);

    /**
     * @brief The function pointer which compares two keys.
     */
    int( * KeyComp)(const void *, const void *);
} SUTLHashmap;

/**
 * @brief Creates a new \p SUTLHashmap with key type as \p tk,value type as \p v, key hash function
 * as \p hash and key compare function as \p cmp.
 *
 * @param tk The key type for the hashmap.
 * @param tv The value type for the hashmap.
 * @param hash A function of the signature <tt>size_t(const void *)</tt> that hashes \p tk.
 * @param cmp A function of signature <tt>int(const void *, const void *)</tt> that compares two
 * \p tk s for equality. (Similar to the \p == operator)
 *
 * @return A \p SUTLHashmap created according to the parameters given.
 */
#define SUTLHashmapNew(tk, tv, hash, cmp)      SUTL_InternalHashmapNew(sizeof(tk), sizeof(tv), hash, cmp)

/**
 * @brief Frees a \p SUTLHashmap which was created using \p SUTLHashmapNew.
 *
 * @param hm The \p SUTLHashmap to free.
 */
#define SUTLHashmapFree(hm)                    SUTL_InternalHashmapFree(&hm)

/**
 * @brief Gets a value assigned to \p k in \p hm.
 *
 * @param tk The key type of \p hm.
 * @param tv The value type of \p hm.
 * @param hm The hashmap to get from.
 * @param k The key to search for.
 *
 * @return A <tt>tv *</tt> that points to the required value. If \p k doesn't exist in \p hm then
 * it is \p NULL.
 */
#define SUTLHashmapGet(tk, tv, hm, k)          (*(tk *)hm.ParamK = k, (tv *)SUTL_InternalHashmapGet(&hm))

/**
 * @brief Inserts an entry with key \p k and value \p v in \p hm.
 *
 * @param tk The key type of \p hm.
 * @param tv The value type of \p hm.
 * @param hm The hashmap to insert to.
 * @param k The key to insert.
 * @param v The value to insert.
 *
 * @return A <tt>tv *</tt> that points to the inserted value.
 */
#define SUTLHashmapInsert(tk, tv, hm, k, v)    (*(tk *)hm.ParamK = k, *(tv *)hm.ParamV = v, (tv *)SUTL_InternalHashmapInsert(&hm))

/**
 * @brief Erases an entry with key \p k in \p hm.
 *
 * @param tk The key type of \p hm.
 * @param hm The hashmap to erase from.
 * @param k The key of the entry to erase.
 */
#define SUTLHashmapErase(tk, hm, k)            (*(tk *)hm.ParamK = k, SUTL_InternalHashmapErase(&hm))

/**
 * @brief Executes \p expr for every entry in \p hm.
 *
 * @param tk The key type of \p hm.
 * @param tv The value type of \p hm.
 * @param hm The hashmap to iterate.
 * @param name The prefix for current entry. Key will have suffix \p _k and value will have suffix
 * \p _v.
 * @param expr The code block to execute for every entry.
 */
#define SUTLHashmapEach(tk, tv, hm, name, expr) \
    {\
        size_t i, j;\
        for (i = 0; i < SUTL_HASHMAP_BUCKET_COUNT; i++)\
        {\
            for (j = 0; j < SUTLVectorSize(hm.Keys[i]) / hm.KeySize; j++)\
            {\
                tk * name##_k = ((tk *)hm.Keys[i] + j);\
                tv * name##_v = ((tv *)hm.Values[i] + j);\
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
SUTLHashmap SUTL_InternalHashmapNew(size_t keysize, size_t valuesize, size_t( * hash)(const void *), int ( * keycomp)(const void *, const void *));
void SUTL_InternalHashmapFree(SUTLHashmap * hm);
void * SUTL_InternalHashmapInsert(SUTLHashmap * hm);
void SUTL_InternalHashmapErase(SUTLHashmap * hm);
void * SUTL_InternalHashmapGet(SUTLHashmap * hm);
/**
 * @{
 */

#ifdef SUTL_IMPLEMENTATION
    SUTLHashmap SUTL_InternalHashmapNew(size_t keysize, size_t valuesize, size_t( * hash)(const void *), int ( * keycomp)(const void *, const void *))
    {
        SUTLHashmap hm;

        /*
         * Initialize the members of `hm`.
         */
        hm.Size = 0;
        hm.KeySize = keysize;
        hm.ValueSize = valuesize;
        hm.ParamK = SUTLVectorNew(char);
        hm.ParamV = SUTLVectorNew(char);
        hm.Hash = hash;
        hm.KeyComp = keycomp;

        /*
         * Allocate enough memory to store 1 key and 1 value in `ParamK` and `ParamV` respectively.
         */
        SUTLVectorResize(hm.ParamK, hm.KeySize);
        SUTLVectorResize(hm.ParamV, hm.ValueSize);

        size_t i;

        /*
         * Initialize all the buckets.
         */
        for (i = 0; i < SUTL_HASHMAP_BUCKET_COUNT; i++)
        {
            hm.Keys[i] = SUTLVectorNew(char);
            hm.Values[i] = SUTLVectorNew(char);
        }

        return hm;
    }

    void SUTL_InternalHashmapFree(SUTLHashmap * hm)
    {
        size_t i;

        /*
         * Free all the buckets.
         */
        for (i = 0; i < SUTL_HASHMAP_BUCKET_COUNT; i++)
        {
            SUTLVectorFree(hm->Values[i]);
            SUTLVectorFree(hm->Keys[i]);
        }

        /*
         * Free the memory allocated for parameter variables.
         */
        SUTLVectorFree(hm->ParamV);
        SUTLVectorFree(hm->ParamK);
    }

    void * SUTL_InternalHashmapInsert(SUTLHashmap * hm)
    {
        /*
         * Calculate the index of the key stored in `ParamK`.
         */
        size_t index = hm->Hash(hm->ParamK) % SUTL_HASHMAP_BUCKET_COUNT;

        hm->Size++;

        void * value = NULL;

        size_t i;

        /*
         * Check if the entry with key stored in `ParamK` already exists.
         */
        for (i = 0; i < SUTLVectorSize(hm->Keys[index]) / hm->KeySize; i++)
            if (hm->KeyComp(hm->ParamK, hm->Keys[index] + i * hm->KeySize))
                /*
                 * Initialize `value` to the value of the entry that already exists.
                 */
                value = hm->Values[index] + i * hm->KeySize;

        /*
         * If the entry already exists return it, otherwise proceed to add the new entry.
         */
        if (value)
            return value;

        /*
         * Push the new key at the end of the bucket with index `index`.
         */
        SUTLVectorPushN(hm->Keys[index], hm->ParamK, hm->KeySize);

        /*
         * Push the new value at the end of the bucket with index `index` and return only the
         * value.
         */
        return SUTLVectorPushN(hm->Values[index], hm->ParamV, hm->ValueSize);
    }

    void SUTL_InternalHashmapErase(SUTLHashmap * hm)
    {
        /*
         * Calculate the index of the key stored in `ParamK`.
         */
        size_t index = hm->Hash(hm->ParamK) % SUTL_HASHMAP_BUCKET_COUNT;

        size_t i;

        /*
         * Search the bucket with index `index` for the key stored in `ParamK`.
         */
        for (i = 0; i < SUTLVectorSize(hm->Keys[index]); i++)
        {
            if (hm->KeyComp(hm->ParamK, hm->Keys[index] + i * hm->KeySize))
            {
                /*
                 * Erase the key and the value from the bucket.
                 */
                SUTLVectorEraseN(hm->Keys[index], i * hm->KeySize, hm->KeySize);
                SUTLVectorEraseN(hm->Values[index], i * hm->ValueSize, hm->ValueSize);

                hm->Size--;

                break;
            }
        }
    }

    void * SUTL_InternalHashmapGet(SUTLHashmap * hm)
    {
        /*
         * Calculate the index of the key stored in `ParamK`.
         */
        size_t index = hm->Hash(hm->ParamK) % SUTL_HASHMAP_BUCKET_COUNT;

        size_t i;

        /*
         * Search the bucket with index `index` for the key stored in `ParamK`.
         */
        for (i = 0; i < SUTLVectorSize(hm->Keys[index]) / hm->KeySize; i++)
            if (hm->KeyComp(hm->ParamK, hm->Keys[index] + i * hm->KeySize))
                /*
                 * Return the value.
                 */
                return hm->Values[index] + i * hm->ValueSize;

        /*
         * If the search fails then `NULL` is returned.
         */
        return NULL;
    }
#endif

#endif
