#include "../include/my_libs.h"
#include "../include/glibc_libs.h"


memory_image_t *MemImageCreate(size_t capacity)
{
	memory_image_t *memory_image = malloc(sizeof(memory_image_t));
	if(!memory_image)
	{
		return NULL;
	}

	memory_image->memory_cells = malloc(capacity * sizeof(memory_cell_t));
	if(!memory_image->memory_cells)
	{
		free(memory_image);
		return NULL;
	}
	memory_image->capacity = capacity;
	memory_image->size = 0; 

	return memory_image;
}

void MemImageDestroy(memory_image_t *image)
{
	free(image->memory_cells);
	free(image);
}

int MemImageInsert(memory_image_t *mem_image, memory_cell_t to_insert)
{	
	memory_cell_t *new = NULL;
	if(mem_image->size >= mem_image->capacity)
	{
		new = realloc(mem_image->memory_cells, mem_image->capacity * 2);
		if(!new)
		{
			return 1;
		}
		mem_image->memory_cells = new;
		mem_image->capacity *= 2;
	}

	mem_image->memory_cells[to_insert.address] = to_insert;
	mem_image->size += 1;

	return 0;
}

memory_cell_t *MemImageGetCell(memory_image_t *mem_image ,size_t address)
{
	return &mem_image->memory_cells[address];
}

memory_image_t *MemImageUnite(memory_image_t *code, memory_image_t *data, int IC)
{
	size_t capacity = code->size + data->size;
	size_t i = 0;
	memory_cell_t *new = NULL;
	new = realloc(code->memory_cells, capacity * sizeof(memory_cell_t));

	if(!new)
	{
		return NULL;
	}
	code->memory_cells = new;
	code->capacity = capacity;
	code->code_len = code->size;
	code->data_len = data->size;

	for(; i < data->size; ++i)
	{
		data->memory_cells[i].address = IC++;
		code->memory_cells[code->size++] = data->memory_cells[i];
	}
	MemImageDestroy(data);
	return code;
}

void MemImagePrint(memory_image_t *img)
{
	int i = 0;
	printf("ADDRESS --- CODE\n");
	for(; i < img->size; ++i)
	{
		printf("%lu --- ", img->memory_cells[i].address);
		PrintWordBinary(img->memory_cells[i].machine_code);
		printf("\n");
	}
}

