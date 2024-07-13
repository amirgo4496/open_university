#ifndef __MEMORY_IMAGE__
#define __MEMORY_IMAGE__

#define IC_OFFSET (100)

typedef struct memory_cell
{
	size_t address;
	short machine_code;
} memory_cell_t;

typedef struct memory_image
{
	size_t size;
	size_t capacity;
	size_t code_len;
	size_t data_len;
	memory_cell_t *memory_cells;
} memory_image_t;

memory_image_t *MemImageCreate(size_t capacity);
void MemImageDestroy(memory_image_t *image);
int MemImageInsert(memory_image_t *mem_image, memory_cell_t to_insert);
memory_cell_t *MemImageGetCell(memory_image_t *mem_image ,size_t address);
void MemImagePrint(memory_image_t *img);
memory_image_t *MemImageUnite(memory_image_t *code, memory_image_t *data, int IC);
void MemImageForEach(memory_image_t *img, action_t ActionFunc, void *param);
#endif
