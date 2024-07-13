#ifndef __EXP_FILES__
#define __EXP_FILES__


int ExportEntries(slist_t *entry_list, char *src_fname);

int ExportExternals(slist_t* extern_list, char *src_fname);
int ExportObject(memory_image_t *mem_img, char *src_fname);
int ExportDo(assembler_data_t *assembler_data, char *src_fname);

int ExportEntryAction(void *symbol ,void *param);
int ExportExternalAction(void *data ,void *param);
int ExportObjectAction(void *data, void *param);
#endif
