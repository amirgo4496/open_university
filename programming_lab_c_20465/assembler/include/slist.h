#ifndef __SLIST__
#define __SLIST__

#include "../include/glibc_libs.h"

typedef struct slist slist_t;
typedef struct node *slist_iter_t;
typedef const struct node *slist_const_iter_t;

typedef int (*is_match_t)(const void *data, void *param);

typedef struct symbol
{
	char name[MAX_LABEL_LEN];
        int value;
	int type;
} symbol_t;

typedef struct macro
{
        char name[MAX_LABEL_LEN];
        int start_offset;
        int end_offset;
} macro_t;


typedef union user_data
{
        macro_t macro;
        symbol_t symbol;
} user_data_t;

typedef int (*action_t)(void *data ,void *param);

/* -------------------------------------------------------------------------- *
 * Description - User Wrapper to free the list.
 * Arguments - the list.
 * Return - nothing.
 * -------------------------------------------------------------------------- */
void SListDestroy(slist_t *list);

/* -------------------------------------------------------------------------- *
 * Description - Creates a new empty linked list or NULL if fails
   NOTE: list will be created with head and a dummy/tail
 * Arguments - Nothing.
 * Return - slist_t pointer.
 * -------------------------------------------------------------------------- */
slist_t *SListCreate(void);

/* -------------------------------------------------------------------------- *
 * Description - Inserts new data before the given iterator.
   SPECIAL NOTE: iterator invalidation is a side effect of this function execution.
 * Arguments - to_insert - iterator to insert before ,data - the data to be inserted.
 * Return - the inserted slist_iter_t.
 * -------------------------------------------------------------------------- */
slist_iter_t SListInsertBefore(slist_iter_t to_insert, const void *data);


/* -------------------------------------------------------------------------- *
 * Description - The function returns the first element of the list. 
 *               if the list is empty we return an iterator to tail
 * Arguments - list - list
 * Return - the first element of the list.
 * -------------------------------------------------------------------------- */
slist_iter_t SListStart(const slist_t *list);


/* -------------------------------------------------------------------------- *
 * Description - The function returns the last element of the list - an invalid iterator.
 * Arguments - list - list
 * Return - the last element of the list.
 * -------------------------------------------------------------------------- */
slist_iter_t SListEnd(const slist_t *list);


/* -------------------------------------------------------------------------- *
 * Description - The function returns data in the given iterator.
 * Arguments - element - the iterator to the requested data.
 * Return - the data in the given iterator.
 * -------------------------------------------------------------------------- */
void *SListGetData(slist_const_iter_t element);


/* -------------------------------------------------------------------------- *
 * Description - The function return the iterator for next element to the given iterator.
    if user gives last iterator will return tail;
 * Arguments - element - the iterator before the requested iterator.
 * Return - slist_iter_t - the next iterator.
 * -------------------------------------------------------------------------- */
slist_iter_t SListNext(slist_const_iter_t element);


/* -------------------------------------------------------------------------- *
 * Description - Deletes the node in place of the given iterator ,if the given iterator points to tail
   undefined behavior.
   SPECIAL NOTE: iterator invalidation is a side effect of this function execution.
 * Arguments - to_remove - iterator to remove
 * Return - the new iterator in the place of the given iterator. if you delete the last node it will return
   iterator to tail.
 * -------------------------------------------------------------------------- */
slist_iter_t SListDelete(slist_iter_t to_remove);


/* -------------------------------------------------------------------------- *
 * Description - The function writes over the data in the given iterator with the given data.
 * Arguments - element - the iterator to the data write operation , data - to be written.
 * Return - nothing.
 * -------------------------------------------------------------------------- */
void SListSetData(slist_iter_t element, const void *data);


/* -------------------------------------------------------------------------- *
 * Description - The function return the iterator for next element to the given iterator.
    if user gives last iterator will return tail;
 * Arguments - element - the iterator before the requested iterator.
 * Return - slist_iter_t - the next iterator.
 * -------------------------------------------------------------------------- */
slist_iter_t SListNext(slist_const_iter_t element);

/* -------------------------------------------------------------------------- *
 * Description - activates the CompareFunc pointer on each node in the iterator range with the given param.
 * Arguments - slist_iter_t from start of range, slist_iter_t to end of range CompareFunc the function to compare
   with ,param - a param to send as argument to the CompareFunc .
   NOTE: from inclusive to exclusive
 * Return - iterator to the matching node or "to iterator" if not found.
 * -------------------------------------------------------------------------- */
slist_iter_t SListFind(slist_const_iter_t from, slist_const_iter_t to,
				 is_match_t CompareFunc, void *param);


/* -------------------------------------------------------------------------- *
 * Description - compares the given iterators.
 * Arguments - elem1 - iterator to be compared ,elem2 - iterator to be compared.
 * Return - returns 1 if equal,0 otherwise.
 * -------------------------------------------------------------------------- */
int SListIterIsEqual(slist_const_iter_t element1, slist_const_iter_t element2);

size_t SListSize(const slist_t *list);
int SListForEach(slist_iter_t from, slist_const_iter_t to, action_t ActionFunc, void *param);

#endif
