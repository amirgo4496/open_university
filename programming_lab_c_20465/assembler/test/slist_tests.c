#include "../include/my_libs.h"
#include "../include/glibc_libs.h"
#include <time.h>


char GetRandomChar(void);
char *GenerateRandomName(char *str);
void GenerateRandomMacros(macro_t *macros);
int PrintList(void *data, void *param);

int main(void)
{

	srand(time(NULL));

	slist_t *list = SListCreate();
	char str[32];
	macro_t macros[1000];
	int i = 0;

	
	printf("%lu\n" ,sizeof(user_data_t));
	GenerateRandomMacros(macros);
	for(i = 0; i < 1000; ++i)
	{
		SListInsertBefore(SListStart(list), (void *)&macros[i]);
	}
	printf("List Size: %lu\n" ,SListSize(list));

	slist_iter_t node = SListStart(list);
	
	
	if(!SListForEach(SListStart(list), SListEnd(list), PrintList, (void *)macros))
	{
		printf("Success!\n");
	}
	else
	{
		printf("Failure!\n");
	}

	for(i = 0; i < 1000; i += 2)
	{
		SListDelete(SListStart(list));	
	}
	printf("List Size: %lu\n" ,SListSize(list));
	SListDestroy(list);
	return 0;
}

int PrintList(void *data, void *param)
{
	static int i = 0;
	macro_t *macros = (macro_t *)param;
	user_data_t *my_data = (user_data_t *)data;	
	printf("%s -- %s\n" ,my_data->macro.name ,macros[999 - i].name);
	int ret = strcmp(macros[999 - i].name ,my_data->macro.name);
	++i;
	return ret;
}
char *GenerateRandomName(char *str)
{
	int i = 0;
	static int j = 0;
	for(i = 0; i < MAX_LABEL_LEN - 1; ++i)
	{
		//str[i] = rand() % 26 + 65;	
		str[i] = j % 26 + 65;
	}	
	str[MAX_LABEL_LEN - 1] = 0;
	++j;
	return str;
}


char GetRandomChar(void)
{
	return (char)(rand() % 26 + 65);
}

void GenerateRandomMacros(macro_t *macros)
{
	int i = 0;
	for(i = 0; i < 1000; ++i)
	{
		GenerateRandomName(macros[i].name);
		macros[i].start_offset = 100;
		macros[i].end_offset = 200;
		//printf("%s\n" ,macros[i].name);
	}
}
