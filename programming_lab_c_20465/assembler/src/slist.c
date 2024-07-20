#include "../include/glibc_libs.h"
#include "../include/my_libs.h"


typedef union node_data
{
        slist_t *list;
	user_data_t user_data;
} node_data_t;

typedef struct node
{
	node_data_t data;
	struct node* next;
} slist_node_t;

struct slist
{
        slist_node_t *head;
        slist_node_t *tail;
};

static void SListRecursiveFree(slist_node_t *head);
static int CountAction(void *data ,void *counter);

/* -------------------------------------------------------------------------- *
 * Description - User Wrapper to free the list.
 * Arguments - the list.
 * Return - nothing.
 * -------------------------------------------------------------------------- */
void SListDestroy(slist_t *list)
{
	SListRecursiveFree(list->head);
}

/* -------------------------------------------------------------------------- *
 * Description - Free the list recursively.
 * Arguments - the list head.
 * Return - nothing.
 * -------------------------------------------------------------------------- */
static void SListRecursiveFree(slist_node_t *head)
{
  	if(NULL == head)
  	{
  	  	return;
  	}
  	else
  	{
    		SListRecursiveFree(head->next);
    		free(head);
  	}
}


/* -------------------------------------------------------------------------- *
 * Description - Creates a new empty linked list or NULL if fails
   NOTE: list will be created with head and a dummy/tail
 * Arguments - Nothing.
 * Return - slist_t pointer.
 * -------------------------------------------------------------------------- */
slist_t *SListCreate(void)
{
  	slist_t *new_list = (slist_t *)malloc(sizeof(slist_t));
  	if(!new_list)
  	{
    		return NULL;
  	}
  	new_list->head = (slist_node_t *)malloc(sizeof(slist_node_t));
  	if(!new_list->head)
  	{
    		free(new_list);
		return NULL;
  	}
	(new_list->head->data).list = new_list;
  	new_list->head->next = NULL;
  	new_list->tail = new_list->head;
  	return new_list;
}

/* -------------------------------------------------------------------------- *
 * Description - Inserts new data before the given iterator.
   SPECIAL NOTE: iterator invalidation is a side effect of this function execution.
 * Arguments - to_insert - iterator to insert before ,data - the data to be inserted.
 * Return - the inserted slist_iter_t.
 * -------------------------------------------------------------------------- */
slist_iter_t SListInsertBefore(slist_iter_t to_insert,const void *data)
{
  	slist_node_t *new_node = NULL;
  	node_data_t prev_elem_data = to_insert->data;
  	slist_node_t *prev_elem_next = to_insert->next;
	new_node = (slist_node_t *)malloc(sizeof(slist_node_t));
  	if(!new_node)
  	{
    	return NULL;
  	}
  	if(NULL == to_insert->next)
  	{
    		(to_insert->data.list)->tail = new_node;
  	}
	memcpy(&(to_insert->data) ,data ,sizeof(node_data_t));
  	to_insert->next = new_node;
  	new_node->data = prev_elem_data;
  	new_node->next = prev_elem_next;
  	return to_insert;
}



/* -------------------------------------------------------------------------- *
 * Description - The function returns the first element of the list. 
 *               if the list is empty we return an iterator to tail
 * Arguments - list - list
 * Return - the first element of the list.
 * -------------------------------------------------------------------------- */
slist_iter_t SListStart(const slist_t *list)
{
 	return list->head;
}

/* -------------------------------------------------------------------------- *
 * Description - The function returns the last element of the list - an invalid iterator.
 * Arguments - list - list
 * Return - the last element of the list.
 * -------------------------------------------------------------------------- */
slist_iter_t SListEnd(const slist_t *list)
{
  	return list->tail;
}

/* -------------------------------------------------------------------------- *
 * Description - The function returns data in the given iterator.
 * Arguments - element - the iterator to the requested data.
 * Return - the data in the given iterator.
 * -------------------------------------------------------------------------- */
void *SListGetData(slist_const_iter_t element)
{
  	return (void *)&(element->data);
}


/* -------------------------------------------------------------------------- *
 * Description - The function return the iterator for next element to the given iterator.
    if user gives last iterator will return tail;
 * Arguments - element - the iterator before the requested iterator.
 * Return - slist_iter_t - the next iterator.
 * -------------------------------------------------------------------------- */
slist_iter_t SListNext(slist_const_iter_t element)
{
  	return element->next;
}


/* -------------------------------------------------------------------------- *
 * Description - Deletes the node in place of the given iterator ,if the given iterator points to tail
   undefined behavior.
   SPECIAL NOTE: iterator invalidation is a side effect of this function execution.
 * Arguments - to_remove - iterator to remove
 * Return - the new iterator in the place of the given iterator. if you delete the last node it will return
   iterator to tail.
 * -------------------------------------------------------------------------- */
slist_iter_t SListDelete(slist_iter_t to_remove)
{
  	slist_node_t *ptr_free = to_remove->next;
  	to_remove->data = to_remove->next->data;
  	if(NULL == to_remove->next->next)
  	{
  	  	(to_remove->data.list)->tail = to_remove;
  	}
  	to_remove->next = to_remove->next->next;
  	free(ptr_free);
  	return to_remove;
}


/* -------------------------------------------------------------------------- *
 * Description - The function writes over the data in the given iterator with the given data.
 * Arguments - element - the iterator to the data write operation , data - to be written.
 * Return - nothing.
 * -------------------------------------------------------------------------- */
void SListSetData(slist_iter_t element, const void *data)
{
	memcpy(&(element->data) ,data ,sizeof(node_data_t));
}



/* -------------------------------------------------------------------------- *
 * Description - activates the CompareFunc pointer on each node in the iterator range with the given param.
 * Arguments - slist_iter_t from start of range, slist_iter_t to end of range CompareFunc the function to compare
   with ,param - a param to send as argument to the CompareFunc .
   NOTE: from inclusive to exclusive
 * Return - iterator to the matching node or "to iterator" if not found.
 * -------------------------------------------------------------------------- */
slist_iter_t SListFind(slist_const_iter_t from,slist_const_iter_t to,
			is_match_t CompareFunc, void *param)
{
  	while(NULL != from->next && (size_t)from != (size_t)to && !CompareFunc((void *)&(from->data) ,param))
  {
		from = SListNext(from);
  }
  	return (slist_iter_t)from;
}


/* -------------------------------------------------------------------------- *
 * Description - counts the number of elements currently in the list.
 * Arguments - slist_t - the list pointer.
 * Return - size_t - the size of the list.
 * -------------------------------------------------------------------------- */
size_t SListSize(const slist_t *list)
{
  size_t counter = 0;
  SListForEach(list->head ,list->tail ,CountAction ,(void *)&counter);
  return counter;
}


/* -------------------------------------------------------------------------- *
 * Description - Side function for counting the list elements to send to foreach.
 * Arguments - counter address to keep that current count.
   NOTE: data not used only here to fit the ActionFunc typedef
 * Return - list current size.
 * -------------------------------------------------------------------------- */
static int CountAction(void *data ,void *counter)
{ 
  (void)data;
  *(int *)counter += 1;
  return 0;
}


/* -------------------------------------------------------------------------- *
 * Description - activates the ActionFunc pointer on each node in the iterator range with the given param or
   ActionFunc fails which ever comes first.
   NOTE: from inclusive to exclusive
 * Arguments - slist_iter_t from start of range, slist_iter_t to end of range ActionFunc the function to activate
   ,param - a param to send as argument to the ActionFunc .
 * Return - 0 for success or non-zero for corresponding failure from ActionFunc.
 * -------------------------------------------------------------------------- */
int SListForEach(slist_iter_t from, slist_const_iter_t to, action_t ActionFunc, void *param)
{
  int err_code = 0;
  while(NULL != from->next && (size_t)from != (size_t)to && !err_code)
  {
    err_code = ActionFunc((void *)&(from->data) ,param);
    from = SListNext(from);
  }
  return err_code;
}

/* -------------------------------------------------------------------------- *
 * Description - compares the given iterators.
 * Arguments - elem1 - iterator to be compared ,elem2 - iterator to be compared.
 * Return - returns 1 if equal,0 otherwise.
 * -------------------------------------------------------------------------- */
int SListIterIsEqual(slist_const_iter_t element1, slist_const_iter_t element2)
{
  return (size_t)element1 == (size_t)element2;
}

/* -------------------------------------------------------------------------- *
 * Description - Sort a given list of symbols.
 * Arguments - list - The list to be sorted.
 * Return - None.
 * -------------------------------------------------------------------------- */

void SListSortSymbols(slist_t *list)
{
	slist_iter_t current = SListStart(list) ,index = NULL;
	symbol_t tmp;
	symbol_t *curr_sym = NULL, *idx_sym = NULL;	
	if(!(SListStart(list)->next))
	{
		return;
	}
	while(current->next)
	{
		index = current->next;
		while(index->next)
		{
			curr_sym = &(current->data.user_data.symbol);
			idx_sym = &(index->data.user_data.symbol);
			if(curr_sym->value > idx_sym->value)
			{
				memcpy(&tmp, curr_sym, sizeof(symbol_t));
				memcpy(curr_sym, idx_sym, sizeof(symbol_t));
				memcpy(idx_sym, &tmp, sizeof(symbol_t));
			}
			index = index->next;
		}
		current = current->next;
	}
}

