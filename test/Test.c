#include <stdio.h>

#define SUTL_IMPLEMENTATION
#define SUTL_ERROR_HANDLER_CUSTOM 1
#include "../include/Shroon/Utils/Vector.h"
#include "../include/Shroon/Utils/String.h"
#include "../include/Shroon/Utils/Hashmap.h"
#include "../include/Shroon/Utils/Hashset.h"
#include "../include/Shroon/Utils/HashUtils.h"

#include "Test.h"

int tmp = 45;
int tmparr[] = {13, 33, 47};

char * ExpectedMsg = NULL;
int ExpectationFulfilled = 0;

void TestErrorHandler(const char * msg)
{
    if (ExpectedMsg)
        if (SHRN_STRCMP(msg, ExpectedMsg) == 0)
            ExpectationFulfilled = 1;

    if (!ExpectationFulfilled)
        fprintf(stderr, "Error: %s\n", msg);
}

void( * SUTLErrorHandler)(const char *) = TestErrorHandler;

void RestoreVectorDefault(int ** v)
{
    SUTLVectorResize(*v, 0);
    SUTLVectorReserve(*v, 0);
}

int main()
{
    SHRN_TEST_INIT()

    SHRN_TEST_GROUP(SHROON_UTILS,

        SHRN_TEST_GROUP(VECTOR,

            int * v = SUTLVectorNew(int);

            SHRN_TEST(v != NULL)

            SHRN_TEST_GROUP(RESERVE,

                /* When new capacity > current size */
                SUTLVectorReserve(v, 2);
                SHRN_TEST(SUTLVectorSize(v) == 0 && SUTLVectorCapacity(v) == 2)
                SUTLVectorReserve(v, 0);

                /* When new capacity < current size */
                SUTLVectorPush(v, tmp);
                SUTLVectorPush(v, tmp);
                ExpectedMsg = "Can't reserve memory less than the amount of memory already in use.";
                SUTLVectorReserve(v, 1);
                SHRN_TEST(SUTLVectorSize(v) == 2 && SUTLVectorCapacity(v) >= 2)

                /* Expect an error msg here */
                SHRN_TEST(ExpectationFulfilled == 1)
                ExpectedMsg = NULL;
                ExpectationFulfilled = 0;

                /* When new capacity == current size */
                SUTLVectorReserve(v, 2);
                SHRN_TEST(SUTLVectorSize(v) == 2 && SUTLVectorCapacity(v) >= 2)

                RestoreVectorDefault(&v);
            )


            SHRN_TEST_GROUP(RESIZE,

                /* When new size > current capacity */
                SUTLVectorResize(v, 2);
                SHRN_TEST(SUTLVectorSize(v) == 2 && SUTLVectorCapacity(v) >= 2)
                SUTLVectorResize(v, 0);

                /* When new size < current capacity */
                SUTLVectorReserve(v, 2);
                SUTLVectorResize(v, 1);
                SHRN_TEST(SUTLVectorSize(v) == 1 && SUTLVectorCapacity(v) >= 2)

                /* When new size == current capacity */
                SUTLVectorResize(v, 2);
                SHRN_TEST(SUTLVectorSize(v) == 2 && SUTLVectorCapacity(v) >= 2)

                RestoreVectorDefault(&v);
            )

            SHRN_TEST_GROUP(INSERT,

                /* When capacity < insert size */
                SUTLVectorPush(v, tmp);
                SHRN_TEST(SUTLVectorSize(v) == 1)
                SHRN_TEST(v[0] == tmp)

                RestoreVectorDefault(&v);

                /* When capacity >= insert size */
                SUTLVectorReserve(v, 1);
                SUTLVectorPush(v, tmp);
                SHRN_TEST(SUTLVectorSize(v) == 1)
                SHRN_TEST(v[0] == tmp)

                RestoreVectorDefault(&v);

                /* When capacity < insert size */
                SUTLVectorPushN(v, tmparr, 3);
                SHRN_TEST(SUTLVectorSize(v) == 3)
                SHRN_TEST(v[0] == tmparr[0] && v[1] == tmparr[1] && v[2] == tmparr[2])

                RestoreVectorDefault(&v);

                /* When capacity >= insert size */
                SUTLVectorReserve(v, 3);
                SUTLVectorPushN(v, tmparr, 3);
                SHRN_TEST(SUTLVectorSize(v) == 3)
                SHRN_TEST(v[0] == tmparr[0] && v[1] == tmparr[1] && v[2] == tmparr[2])

                RestoreVectorDefault(&v);

                /* When capacity < insert size */
                SUTLVectorResize(v, 3);
                SUTLVectorInsert(v, 1, tmp);
                SHRN_TEST(SUTLVectorSize(v) == 4)
                SHRN_TEST(v[1] == tmp)

                RestoreVectorDefault(&v);

                /* When capacity >= insert size */
                SUTLVectorReserve(v, 4);
                SUTLVectorResize(v, 3);
                SUTLVectorInsert(v, 1, tmp);
                SHRN_TEST(SUTLVectorSize(v) == 4)
                SHRN_TEST(v[1] == tmp)

                RestoreVectorDefault(&v);

                /* When capacity < insert size */
                SUTLVectorResize(v, 3);
                SUTLVectorInsertN(v, 1, tmparr, 3);
                SHRN_TEST(SUTLVectorSize(v) == 6)
                SHRN_TEST(v[1] == tmparr[0] && v[2] == tmparr[1] && v[3] == tmparr[2])

                RestoreVectorDefault(&v);

                /* When capacity >= insert size */
                SUTLVectorReserve(v, 6);
                SUTLVectorResize(v, 3);
                SUTLVectorInsertN(v, 1, tmparr, 3);
                SHRN_TEST(SUTLVectorSize(v) == 6)
                SHRN_TEST(v[1] == tmparr[0] && v[2] == tmparr[1] && v[3] == tmparr[2])

                RestoreVectorDefault(&v);
            )

            SHRN_TEST_GROUP(ERASE,

                /* When size == 0 */
                SUTLVectorPop(v);
                SHRN_TEST(SUTLVectorSize(v) == 0)

                /* When size > 0 */
                SUTLVectorPush(v, tmp);
                SUTLVectorPop(v);
                SHRN_TEST(SUTLVectorSize(v) == 0)

                RestoreVectorDefault(&v);

                /* When size == 0 */
                SUTLVectorPopN(v, 3);
                SHRN_TEST(SUTLVectorSize(v) == 0)

                /* When size > 0 */
                SUTLVectorPushN(v, tmparr, 3);
                SUTLVectorPopN(v, 3);
                SHRN_TEST(SUTLVectorSize(v) == 0)

                RestoreVectorDefault(&v);

                /* When size == 0 */
                ExpectedMsg = "Elements requested to be erased don't exist.";
                SUTLVectorErase(v, 1);
                SHRN_TEST(SUTLVectorSize(v) == 0)

                /* Expect an error msg here */
                SHRN_TEST(ExpectationFulfilled == 1)
                ExpectedMsg = NULL;
                ExpectationFulfilled = 0;

                /* When size > 0 */
                SUTLVectorPushN(v, tmparr, 3);
                SUTLVectorErase(v, 1);
                SHRN_TEST(SUTLVectorSize(v) == 2)

                RestoreVectorDefault(&v);

                /* When size == 0 */
                ExpectedMsg = "Elements requested to be erased don't exist.";
                SUTLVectorEraseN(v, 1, 3);
                SHRN_TEST(SUTLVectorSize(v) == 0)

                /* Expect an error msg here */
                SHRN_TEST(ExpectationFulfilled == 1)
                ExpectedMsg = NULL;
                ExpectationFulfilled = 0;

                /* When size > 0 */
                SUTLVectorPushN(v, tmparr, 3);
                SUTLVectorEraseN(v, 1, 2);
                SHRN_TEST(SUTLVectorSize(v) == 1)

                RestoreVectorDefault(&v);
            )

            SHRN_TEST_GROUP(EACH,

                SUTLVectorPushN(v, tmparr, 3);
                SUTLVectorEach(int, v, elem,
                    *elem *= *elem;
                );

                SHRN_TEST(v[0] == tmparr[0] * tmparr[0] && v[1] == tmparr[1] * tmparr[1] && v[2] == tmparr[2] * tmparr[2])
            )

            SUTLVectorFree(v);
        )

        SHRN_TEST_GROUP(STRING,

            SUTLString slice;
            SUTLString str = SUTLStringNew();

            SUTLStringAppendP(str, "0123456789");

            /* When at <= string size and slice size == 0 */
            slice = SUTLStringSlice(str, 3, 0);
            SHRN_TEST(strcmp(slice, "3456789") == 0);

            /* When at <= string size and at + slice size <= string size */
            slice = SUTLStringSlice(str, 1, 3);
            SHRN_TEST(strcmp(slice, "123") == 0);

            /* When at <= string size and at + slice size > string size */
            slice = SUTLStringSlice(str, 7, 5);
            SHRN_TEST(strncmp(slice, "789", 3) == 0 && SUTLStringSize(slice) == 5);

            /* When at > string size and at + slice size > string size */
            ExpectedMsg = "Invalid index specified for slicing string.";
            slice = SUTLStringSlice(str, 10, 2);
            SHRN_TEST(slice == NULL);

            /* Expect an error msg here */
            SHRN_TEST(ExpectationFulfilled == 1)
            ExpectedMsg = NULL;
            ExpectationFulfilled = 0;

            SUTLStringFree(str);

        )

        SHRN_TEST_GROUP(HASHMAP,

            SUTLHashmap hm = SUTLHashmapNew(int, int, SUTLHash_int, SUTLCmp_int);

            /* Normal use case */
            SUTLHashmapInsert(int, int, hm, 25, 625);
            SHRN_TEST(hm.Size == 1);
            SHRN_TEST(*SUTLHashmapGet(int, int, hm, 25) == 625);

            /* When element exists */
            SUTLHashmapErase(int, hm, 25);
            SHRN_TEST(hm.Size == 0);
            SHRN_TEST(SUTLHashmapGet(int, int, hm, 25) == NULL);

            /* When element doesn't exist */
            SUTLHashmapErase(int, hm, 25);
            SHRN_TEST(hm.Size == 0 && SUTLHashmapGet(int, int, hm, 25) == NULL);
            SHRN_TEST(SUTLHashmapGet(int, int, hm, 25) == NULL);

            SUTLHashmapFree(hm);

        )

        SHRN_TEST_GROUP(HASHSET,

            SUTLHashset hs = SUTLHashsetNew(int, SUTLHash_int, SUTLCmp_int);

            /* Normal use case */
            SUTLHashsetInsert(int, hs, 25);
            SHRN_TEST(hs.Size == 1);
            SHRN_TEST(*SUTLHashsetGet(int, hs, 25) == 25);

            /* When element exists */
            SUTLHashsetErase(int, hs, 25);
            SHRN_TEST(hs.Size == 0);
            SHRN_TEST(SUTLHashsetGet(int, hs, 25) == NULL);

            /* When element doesn't exist */
            SUTLHashsetErase(int, hs, 25);
            SHRN_TEST(hs.Size == 0 && SUTLHashsetGet(int, hs, 25) == NULL);
            SHRN_TEST(SUTLHashsetGet(int, hs, 25) == NULL);

            SUTLHashsetFree(hs);

        )

    )
}
