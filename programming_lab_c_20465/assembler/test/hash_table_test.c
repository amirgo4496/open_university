#include "../include/glibc_libs.h"
#include "../include/my_libs.h"

size_t HashFunc(const void *data);
int CompFunc(const void *data , void *param);

void GenerateRandomMacros(macro_t *macros);
char GetRandomChar(void);
char *GenerateRandomName(char *str);

int main(void)
{
	hash_table_t *hash_table = HashCreate(128 ,HashFunc, CompFunc);
	macro_t macros[1000];
	int i = 0, j = 0;
	GenerateRandomMacros(macros);
	user_data_t *u_data = NULL;
	printf("Table Size: %lu --- Occupied: %lu\n" ,HashTotalSize(hash_table),HashOccupiedSize(hash_table));
	for(i = 0; i < 32; ++i)
	{
		HashInsert(&hash_table, (void *)&macros[i]);
	}	
	printf("Table Size: %lu --- Occupied: %lu\n" ,HashTotalSize(hash_table),HashOccupiedSize(hash_table));

	for(i = 0; i < 100; ++i)
	{
		if(u_data = HashFind(hash_table, (void *)macros[i].name))
		{
			printf("%s\n" ,u_data->macro.name);
			++j;
		}
	}
	for(i = 32; i < 100; ++i)
	{
		HashInsert(&hash_table, (void *)&macros[i]);
	}
	printf("Table Size: %lu --- Occupied: %lu\n" ,HashTotalSize(hash_table),HashOccupiedSize(hash_table));
	printf("Found %d Items!\n" ,j);
	HashDestroy(hash_table);	
	return 0;
}

int CompFunc(const void *data , void *param)
{
	user_data_t *my_data = (user_data_t *)data;	
        return !strcmp((const char *)my_data->macro.name ,(const char *)param);
}

size_t HashFunc(const void *data)
{
        size_t i = 0;
        char *str = (char *)data;
        size_t size = 0;
        const int p = 53; /* closest prime to */
        const size_t m = (pow(10, 9) + 9); /* very big prime */
        long hash_value = 0;
        long power = 1;
        size = strlen(str);
        for (i = 0; i < (size - 1); ++i)
        {
                hash_value = (hash_value + (str[i] - 'a' + 1) * power) % m;
                power = (power * p) % m;
        }
        return hash_value;
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

char *GenerateRandomName(char *str)
{
	int i = 0;
	static int j = 0;
	for(i = 0; i < MAX_LABEL_LEN - 1; ++i)
	{
		str[i] = rand() % 26 + 65;	
		//str[i] = j % 26 + 65;
	}	
	str[MAX_LABEL_LEN - 1] = 0;
	++j;
	return str;
}
