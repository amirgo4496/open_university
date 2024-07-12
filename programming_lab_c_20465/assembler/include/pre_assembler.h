#ifndef __PRE_ASSEMBLER__
#define __PRE_ASSEMBLER__

int PreAsmDo(char *src_file_name, assembler_data_t* assembler_data);
int PreAsmParseLine(char *line, char *token, FILE *src_file ,FILE *target_file ,hash_table_t **ht);
int PreAsmExtendMacro(FILE *src_file ,FILE *target_file ,macro_t macro);
int PreAsmIsValidMacroName(const char *name);
int PreAsmCompMacroFunc(const void *data , void *param);
int PreAsmParseSourceFile(FILE *src_file, FILE *target_file, 
				hash_table_t **macro_table);

#endif
