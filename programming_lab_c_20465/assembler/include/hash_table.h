#ifndef __HASH_TABLE__
#define __HASH_TABLE__

#include "../include/glibc_libs.h"

typedef int (*cmp_func_t)(const void *data, void *param);
typedef size_t (*hash_func_t)(const void *data);
typedef struct hash_table hash_table_t;



/*
 * Description - HashSize computes the number of elements in the hash table.
 * Arguments - table - pointer to hash table
 * Return - the number of elements in the table.
 * Time Complexity - O(n)
 * Space Complexity - O(1)
 */
size_t HashOccupiedSize(const hash_table_t *hash_table);
size_t HashTotalSize(const hash_table_t *hash_table);

/*
 * Description - HashCreate creates a hash table.
 * Arguments - index_capacity - maximum amount of indexes in the table
 *             HashFunc - hash function for the hash table 
 *             and hashcmp - compre function for data.
 * Return - A pointer to a new hash table, return NULL on fail.
 */
hash_table_t *HashCreate(size_t index_amount, 
			hash_func_t HashFunc, cmp_func_t CompareFunc);

/*
 * Description - HashDestroy destroys a given hash table
 * Arguments - table - pointer to hash table
 * Return - void.
 */
void HashDestroy(hash_table_t *hash_table);

/*
 * Description - HashRemove removes the element correlated with the given key.
 * Arguments - table - pointer to hash table
 *             Data - to remove (will use as parameter in 
 *              hash function)
 * Return - 0 upon success, 1 upon failiur.
 */
int HashRemove(hash_table_t *hash_table, const void *data);

/*
 * Description - HashInsert inserts a new value to hash table
 * Arguments - table - pointer to hash table, data - to insert.
 * Return - 0 upon success, 1 upon failier.
 */
int HashInsert(hash_table_t **hash_table, const void *data);

/*
 * Description - HashFind find the element correlated with the given data.
 * Arguments - table - pointer to hash table
 *             data - key value to find.
 * Return - void pointer to the value correlated with the given key.
 */
void *HashFind(const hash_table_t *hash_table, const void *data);

/*
 * Description - HForEach applies ActionFunc with param on each element in the table.
 * Arguments - table - pointer to hash table
 *             ActionFunc - pointer to action function
 *             param - pointer to action func parameter
 * Return - 0 upon success, 1 upon failure.
 */     
int HashForEach(hash_table_t *hash_table, action_t ActionFunc, void *param);

/*create a new hash table and rehashes the keys to the new hash table if fails at any point it returns NULL else it returns a pointer to the new hash table address*/
hash_table_t *ResizeRehash(hash_table_t *hash_table ,size_t index_amount);


#endif
