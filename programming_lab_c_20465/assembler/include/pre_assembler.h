#ifndef __PRE_ASSEMBLER__
#define __PRE_ASSEMBLER__

/* -------------------------------------------------------------------------- *
 * Description - User wrapper to execute the pre assembler stage.
 * Arguments - char *src_file_name
 * 		assembler_data_t assembler data structures and metadata.
 * Return - returns error code or 0 for success.
 * -------------------------------------------------------------------------- */
int PreAsmDo(char *src_file_name, assembler_data_t* assembler_data);
/* -------------------------------------------------------------------------- *
 * Description - Parses a single line from source file and writes if needed to target file.
 * Arguments - char *line to be parsed
 * 		char *token first word of line
 * 		FILE *src_file
 * 		FILE *target_file
 *		hash_table_t **macro_table.
 * Return - error number or 0 for success.
 * -------------------------------------------------------------------------- */
int PreAsmParseLine(char *line, char *token, FILE *src_file ,FILE *target_file ,hash_table_t **macro_table);
/* -------------------------------------------------------------------------- *
 * Description - writes a given macro from src_file to target_file(extends macro at target file).
 * Arguments - FILE *src_file from which macro defenition is read.
 * 		FILE *target_file to which macro is extended.
 * 		macro_t macro to be extended.
 * Return - None.
 * -------------------------------------------------------------------------- */
void PreAsmExtendMacro(FILE *src_file ,FILE *target_file ,macro_t macro);
/* -------------------------------------------------------------------------- *
 * Description - Checks if a given macro name is valid.
 * Arguments - char *name of macro.
 * Return - returns 1 if valid, 0 otherwise.
 * -------------------------------------------------------------------------- */
int PreAsmIsValidMacroName(const char *name);


/* -------------------------------------------------------------------------- *
 * Description - Function to be used for the macro hash table to compare two macros.
 * Arguments - void *data to the macro being compared
 * 		void *compared_name to the name being compared to.
 * Return - returns 1 if equal 0 if not.
 * -------------------------------------------------------------------------- */
int PreAsmCompMacroFunc(const void *data , void *param);

/* -------------------------------------------------------------------------- *
 * Description - Parses the source file line by line and writes the output to target file
 * 		removes comments and extends macros.
 * Arguments - FILE *src_file
 * 		FILE *target_file
 * 		hash_table_t **macro_table.
 * Return - error code or 0 for success.
 * -------------------------------------------------------------------------- */
int PreAsmParseSourceFile(FILE *src_file, FILE *target_file, 
				hash_table_t **macro_table);

#endif
