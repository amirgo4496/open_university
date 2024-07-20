#include "../include/glibc_libs.h"
#include "../include/my_libs.h"

#define MACRO_STARTER ("macr")
#define MACRO_STARTER_LEN (4)

enum PRE_ASSEMBLER_STATES{DEFAULT, PARSE_MACRO};


/* -------------------------------------------------------------------------- *
 * Description - Checks if a given macro name is valid.
 * Arguments - char *name of macro.
 * Return - returns 1 if valid, 0 otherwise.
 * -------------------------------------------------------------------------- */
int PreAsmIsValidMacroName(const char *name)
{
	return isalpha(*name) && !GetOperation(name) && !GetInstructionType(name) && strlen(name) < 32;
}

/* -------------------------------------------------------------------------- *
 * Description - writes a given macro from src_file to target_file(extends macro at target file).
 * Arguments - FILE *src_file from which macro defenition is read.
 * 		FILE *target_file to which macro is extended.
 * 		macro_t macro to be extended.
 * Return - None.
 * -------------------------------------------------------------------------- */
void PreAsmExtendMacro(FILE *src_file ,FILE *target_file ,macro_t macro)
{
	int macro_size = 0;
	long saved_position = ftell(src_file);

	
	macro_size = macro.end_offset - macro.start_offset;
	fseek(src_file ,macro.start_offset ,SEEK_SET);
	while(macro_size--)
	{
		fputc(fgetc(src_file) ,target_file);
	}
	fseek(src_file ,saved_position ,SEEK_SET);
}

/* -------------------------------------------------------------------------- *
 * Description - Parses a single line from source file and writes if needed to target file.
 * Arguments - char *line to be parsed
 * 		char *token first word of line
 * 		FILE *src_file
 * 		FILE *target_file
 *		hash_table_t **macro_table.
 * Return - error number or 0 for success.
 * -------------------------------------------------------------------------- */
int PreAsmParseLine(char *line, char *token, FILE *src_file ,FILE *target_file ,hash_table_t **macro_table)
{
	static int state = DEFAULT;
	static macro_t curr_macr;
	user_data_t *found_macr = NULL;
	/***/
	switch(state)
	{
		case DEFAULT:
			if(!strcmp(token ,"macr"))
			{
				token = strtok(NULL, "\t\n\r\f ");
				if(!token || !PreAsmIsValidMacroName(token) 
					|| strtok(NULL,"\t\n\r\f "))
				{
					return INVALID_MACRO_NAME;		
				}
				else if(HashFind(*macro_table ,token))
				{
					return MACRO_ALREADY_DEFINED;
				}
				else
				{
					state = PARSE_MACRO;
					strncpy(curr_macr.name ,token ,MAX_LABEL_LEN);
					curr_macr.start_offset = ftell(src_file);
					curr_macr.end_offset = -1;
				}
			}
			else if((found_macr = (user_data_t *)HashFind(*macro_table ,token)))
			{
				PreAsmExtendMacro(src_file ,target_file ,found_macr->macro);
			}
			else
			{
				fputs(line ,target_file);
			}	
			break;
		case PARSE_MACRO:
			if(!strcmp(token ,"endmacr"))
			{
				state = DEFAULT;
				curr_macr.end_offset = ftell(src_file) - strlen(line);
				HashInsert(macro_table ,(void *)&curr_macr);
				found_macr = HashFind(*macro_table ,curr_macr.name);
			}
			break;
			
	}
	return 0;
}

/* -------------------------------------------------------------------------- *
 * Description - User wrapper to execute the pre assembler stage.
 * Arguments - char *src_file_name
 * 		assembler_data_t assembler data structures and metadata.
 * Return - returns error code or 0 for success.
 * -------------------------------------------------------------------------- */
int PreAsmDo(char *src_file_name, assembler_data_t* assembler_data)
{
	FILE *target_file = NULL, *src_file = NULL; 
	int ret = 0;
	char *target_file_name = NULL;
	char *src_full_fname = NULL;

	src_full_fname = GetFileName(src_file_name, ".as");
	src_file = fopen(src_full_fname, "r+");
	if(!src_file)
	{
		return FOPEN_ERR;
	}
	target_file_name = GetFileName(src_file_name, ".am");
	target_file = fopen(target_file_name, "w+");
	if(!target_file)
	{
		fclose(src_file);
		free(target_file_name);
		return FOPEN_ERR;
	}

	assembler_data->macro_table = HashCreate(128, HashFunc, PreAsmCompMacroFunc);
	assembler_data->src_file = target_file;	
	ret = PreAsmParseSourceFile(src_file, target_file,&(assembler_data->macro_table));

	fseek(target_file, 0, SEEK_SET);
	fclose(src_file);
	free(target_file_name);
	free(src_full_fname);
	return ret;
}


/* -------------------------------------------------------------------------- *
 * Description - Parses the source file line by line and writes the output to target file
 * 		removes comments and extends macros.
 * Arguments - FILE *src_file
 * 		FILE *target_file
 * 		hash_table_t **macro_table.
 * Return - error code or 0 for success.
 * -------------------------------------------------------------------------- */
int PreAsmParseSourceFile(FILE *src_file, FILE *target_file, 
				hash_table_t **macro_table)
{
	char curr_line[MAX_LINE_LEN + 1] = {0} ,line_cpy[MAX_LINE_LEN + 1] = {0};
	int err_code = 0;
	char *token = NULL;
	int line_counter = 0;
	

	while(fgets(curr_line, MAX_LINE_LEN, src_file))
	{
		++line_counter;
		strcpy(line_cpy ,curr_line);
		token = strtok(line_cpy, "\t\n\r\f ");
		if(!token || ';' == token[0])
		{
			continue;
		}
		if(0 > (err_code = PreAsmParseLine(curr_line, token, src_file, target_file ,macro_table)))
		{
			PrintError(err_code ,line_counter);
			return err_code;
		}

	}

	return 0;
}


/* -------------------------------------------------------------------------- *
 * Description - Function to be used for the macro hash table to compare two macros.
 * Arguments - void *data to the macro being compared
 * 		void *compared_name to the name being compared to.
 * Return - returns 1 if equal 0 if not.
 * -------------------------------------------------------------------------- */
int PreAsmCompMacroFunc(const void *data , void *compared_name)
{
        user_data_t *my_data = (user_data_t *)data;    
        return !strcmp((const char *)my_data->macro.name ,(const char *)compared_name);
}


