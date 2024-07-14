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
/* -------------------------------------------------------------------------- *
 * Description - Creates a new memory image.
 * Arguments - capacity initial capacity for the iamge.
 * Return - memory image created or NULL if error occured.
 * -------------------------------------------------------------------------- */
memory_image_t *MemImageCreate(size_t capacity);
/* -------------------------------------------------------------------------- *
 * Description - Free's resources used by memory image.
 * Arguments - image to be destroyed.
 * Return - None.
 * -------------------------------------------------------------------------- */
void MemImageDestroy(memory_image_t *image);
/* -------------------------------------------------------------------------- *
 * Description - Inserts a new cell into the memory image, if size is insufficient
 * 		it reallocates new space.
 * Arguments - mem_image memory image to be updated
 * 		to_insert cell to be inserted.
 * Return - 0 for success or error code for failure.
 * -------------------------------------------------------------------------- */
int MemImageInsert(memory_image_t *mem_image, memory_cell_t to_insert);
/* -------------------------------------------------------------------------- *
 * Description - Get a cell of memory at address from image.
 * Arguments - mem_image memory image
 * 		address - address of memory image.
 * Return - memory cell found or NULL if not found.
 * -------------------------------------------------------------------------- */
memory_cell_t *MemImageGetCell(memory_image_t *mem_image ,size_t address);
/* -------------------------------------------------------------------------- *
 * Description - Helper function for debugging for printing a memory image.
 * Arguments - img the memory image.
 * Return - None.
 * -------------------------------------------------------------------------- */
void MemImagePrint(memory_image_t *img);

/* -------------------------------------------------------------------------- *
 * Description - Concatantes two memory images.
 * Arguments - code image to be concatenated to
 * 		data second image to be concatanted.
 * 		IC value after first stage(maximum value).
 * Return - returns new memory image or NULL if something failed.
 * -------------------------------------------------------------------------- */
memory_image_t *MemImageUnite(memory_image_t *code, memory_image_t *data, int IC);

/* -------------------------------------------------------------------------- *
 * Description - Execute ActionFunc for each memory cell in memory image.
 * Arguments - img the memory image
 * 		ActionFunc the function to be executed.
 * 		param a user supplied parameter for ActionFunc
 * Return - return error code (0 for success).
 * -------------------------------------------------------------------------- */
int MemImageForEach(memory_image_t *img, action_t ActionFunc, void *param);
#endif
