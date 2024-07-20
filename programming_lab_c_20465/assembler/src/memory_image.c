#include "../include/my_libs.h"
#include "../include/glibc_libs.h"

/* -------------------------------------------------------------------------- *
 * Description - Creates a new memory image.
 * Arguments - capacity initial capacity for the iamge.
 * Return - memory image created or NULL if error occured.
 * -------------------------------------------------------------------------- */
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
/* -------------------------------------------------------------------------- *
 * Description - Free's resources used by memory image.
 * Arguments - image to be destroyed.
 * Return - None.
 * -------------------------------------------------------------------------- */
void MemImageDestroy(memory_image_t *image)
{
	free(image->memory_cells);
	free(image);
}

/* -------------------------------------------------------------------------- *
 * Description - Inserts a new cell into the memory image, if size is insufficient
 * 		it reallocates new space.
 * Arguments - mem_image memory image to be updated
 * 		to_insert cell to be inserted.
 * Return - 0 for success or error code for failure.
 * -------------------------------------------------------------------------- */
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
/* -------------------------------------------------------------------------- *
 * Description - Get a cell of memory at address from image.
 * Arguments - mem_image memory image
 * 		address - address of memory image.
 * Return - memory cell found or NULL if not found.
 * -------------------------------------------------------------------------- */
memory_cell_t *MemImageGetCell(memory_image_t *mem_image ,size_t address)
{
	if(address > mem_image->size)
	{
		return NULL;
	}
	return &mem_image->memory_cells[address];
}
/* -------------------------------------------------------------------------- *
 * Description - Concatantes two memory images.
 * Arguments - code image to be concatenated to
 * 		data second image to be concatanted.
 * 		IC value after first stage(maximum value).
 * Return - returns new memory image or NULL if something failed.
 * -------------------------------------------------------------------------- */
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
/* -------------------------------------------------------------------------- *
 * Description - Execute ActionFunc for each memory cell in memory image.
 * Arguments - img the memory image
 * 		ActionFunc the function to be executed.
 * 		param a user supplied parameter for ActionFunc
 * Return - return error code (0 for success).
 * -------------------------------------------------------------------------- */
int MemImageForEach(memory_image_t *img, action_t ActionFunc, void *param)
{
	int i = 0;
	int ret = 0;
	for(; i < img->size; ++i)
	{
		if((ret = ActionFunc(&(img->memory_cells[i]), param)))
		{
			break;
		}
	}
	return ret;
}
/* -------------------------------------------------------------------------- *
 * Description - Helper function for debugging for printing a memory image.
 * Arguments - img the memory image.
 * Return - None.
 * -------------------------------------------------------------------------- */
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

