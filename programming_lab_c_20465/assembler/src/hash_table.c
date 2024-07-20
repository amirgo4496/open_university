#include "../include/glibc_libs.h"
#include "../include/my_libs.h"

struct hash_table
{
    	size_t index_amount;
	size_t occupied_indexes;
    	hash_func_t HashFunc;
    	cmp_func_t CompareFunc;
    	slist_t **bucket_list;
};

static int RehashAction(void *data ,void *new_hash_table);
static double GetLoadFactor(hash_table_t *hash_table);



/*
 * Description - HashCreate creates a hash table.
 * Arguments - index_capacity - maximum amount of indexes in the table
 *             HashFunc - hash function for the hash table 
 *             and hashcmp - compre function for data.
 * Return - A pointer to a new hash table, return NULL on fail.
 */
hash_table_t *HashCreate(size_t index_amount, 
			hash_func_t HashFunc, cmp_func_t CompareFunc)
{
	size_t i = 0;
        hash_table_t *hash_table = (hash_table_t *)malloc(sizeof(hash_table_t) + (sizeof(slist_t *) * index_amount));
        if(!hash_table)
        {
                return NULL;
        }
        hash_table->index_amount = index_amount;
        hash_table->HashFunc = HashFunc;
	hash_table->occupied_indexes = 0;
        hash_table->CompareFunc = CompareFunc;
        hash_table->bucket_list = (slist_t **)((char *)hash_table + sizeof(hash_table_t));
        while(i < index_amount)
        {
                *((hash_table->bucket_list) + i) = SListCreate();
                if(!(*((hash_table->bucket_list) + i)))
                {
                        size_t j = 0;
                        while(j < i)
                        {
                                SListDestroy(*(hash_table->bucket_list + j));
                                ++j;
                        }
                        free(hash_table);
                        return NULL;
                }
                ++i;
        }
        return hash_table;
}

/*
 * Description - HashDestroy destroys a given hash table
 * Arguments - table - pointer to hash table
 * Return - void.
 */
void HashDestroy(hash_table_t *hash_table)
{
        size_t i = 0;
        while(i < hash_table->index_amount)
        {
                SListDestroy(*(hash_table->bucket_list + i));
                ++i;
        }
        free(hash_table);
}

/*
 * Description - HashRemove removes the element correlated with the given key.
 * Arguments - table - pointer to hash table
 *             Data - to remove (will use as parameter in 
 *              hash function)
 * Return - 0 upon success, 1 upon failiur.
 */
int HashRemove(hash_table_t *hash_table, const void *data)
{
        size_t index = 0;
        slist_iter_t found = NULL;
        index = hash_table->HashFunc(data) % hash_table->index_amount;
        found = SListFind(SListStart(*(hash_table->bucket_list + index)), SListEnd(*(hash_table->bucket_list + index)), hash_table->CompareFunc, (void *)data);
        if(!SListIterIsEqual(found ,SListEnd(*(hash_table->bucket_list + index))))
        {
                SListDelete(found);
		hash_table->occupied_indexes -= 1;
                return 0;
        }
        return 1;
}

/*
 * Description - HashInsert inserts a new value to hash table
 * Arguments - table - pointer to hash table, data - to insert.
 * Return - 0 upon success, 1 upon failier.
 */
int HashInsert(hash_table_t **hash_table, const void *data)
{
        size_t index = 0;
        slist_iter_t new_data = NULL;
        index = (*hash_table)->HashFunc(data) % (*hash_table)->index_amount;
        new_data = SListInsertBefore(SListStart(*((*hash_table)->bucket_list + index)), data);
	if(new_data)
	{
		(*hash_table)->occupied_indexes += 1;
		if(GetLoadFactor(*hash_table) > 0.6)
		{
			*hash_table = ResizeRehash(*hash_table ,
					((*hash_table)->index_amount * 2));
		}
		return 0;
	}
	return 1;
}

/*
 * Description - HashFind find the element correlated with the given data.
 * Arguments - table - pointer to hash table
 *             data - key value to find.
 * Return - void pointer to the value correlated with the given key or NULL if not found.
 */
void *HashFind(const hash_table_t *hash_table, const void *data)
{
        size_t index = 0;
        slist_iter_t found = NULL;
        void *data_found = NULL;
        index = hash_table->HashFunc(data) % hash_table->index_amount;
        found = SListFind(SListStart(*(hash_table->bucket_list + index)), SListEnd(*(hash_table->bucket_list + index)),hash_table->CompareFunc, (void *)data);
        if(!SListIterIsEqual(found ,SListEnd(*(hash_table->bucket_list + index))))
        {
                data_found = SListGetData(found);
		return data_found;
        }
	return NULL;
}


/*
 * Description - ForEach applies ActionFunc with param on each element in the table.
 * Arguments - table - pointer to hash table
 *             ActionFunc - pointer to action function
 *             param - pointer to action func parameter
 * Return - 0 upon success, 1 upon failure.
 */     
int HashForEach(hash_table_t *hash_table, action_t ActionFunc, void *param)
{
        size_t i = 0;
        int result = 0;
        while(i < hash_table->index_amount)
        {
                result = SListForEach(SListStart(*(hash_table->bucket_list + i)),
				SListEnd(*(hash_table->bucket_list + i)),
				ActionFunc, param);
                ++i;
        }
        return result;
}

static double GetLoadFactor(hash_table_t *hash_table)
{
        return (double)hash_table->occupied_indexes / hash_table->index_amount;
}

static int RehashAction(void *data ,void *new_hash_table)
{
        return HashInsert((hash_table_t **)&new_hash_table, data);
}


/*create a new hash table and rehashes the keys to the new hash table if fails at any point it returns NULL else it returns a pointer to the new hash table address*/
hash_table_t *ResizeRehash(hash_table_t *hash_table ,size_t index_amount)
{
        hash_table_t *rehashed_hash_table = HashCreate(index_amount, 
					hash_table->HashFunc, hash_table->CompareFunc);
        int err_code = 0;
        if(!rehashed_hash_table)
        {
                return NULL;
        }
        err_code = HashForEach(hash_table, RehashAction, (void *)rehashed_hash_table);
        if(err_code)
        {
                HashDestroy(rehashed_hash_table);
                return NULL;
        }
	rehashed_hash_table->occupied_indexes = hash_table->occupied_indexes;
        HashDestroy(hash_table);
        return rehashed_hash_table;
}


/*
 * Description - HashSize computes the number of elements in the hash table.
 * Arguments - table - pointer to hash table
 * Return - the number of elements in the table.
 * Time Complexity - O(n)
 * Space Complexity - O(1)
 */
size_t HashOccupiedSize(const hash_table_t *hash_table)
{
        return hash_table->occupied_indexes;
}

size_t HashTotalSize(const hash_table_t *hash_table)
{
	return hash_table->index_amount;
}

