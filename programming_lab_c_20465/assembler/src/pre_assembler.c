#include "../include/glibc_libs.h"
#include "../include/my_libs.h"

#define MACRO_STARTER ("macr")
#define MACRO_STARTER_LEN (4)

enum PRE_ASSEMBLER_STATES{DEFAULT, PARSE_MACRO};


/*
TODO: ADD COMMENTS!!!
*/

int PreAsmIsValidMacroName(const char *name)
{
	return isalpha(*name) && !GetOperation(name) && !GetInstructionType(name) && strlen(name) < 32;
}

int PreAsmExtendMacro(FILE *src_file ,FILE *target_file ,macro_t macro)
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
	return 0;	
}

int PreAsmParseLine(char *line, char *token, FILE *src_file ,FILE *target_file ,hash_table_t **ht)
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
				else if(HashFind(*ht ,token))
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
			else if((found_macr = (user_data_t *)HashFind(*ht ,token)))
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
				HashInsert(ht ,(void *)&curr_macr);
				found_macr = HashFind(*ht ,curr_macr.name);
			}
			break;
			
	}


	return 0;
}

int PreAsmDo(char *src_file_name, assembler_data_t* assembler_data)
{
	FILE *target_file = NULL, *src_file = NULL;; 
	char *target_file_name = NULL;
	int ret = 0;
	
	target_file = fopen(target_file_name, "r+");
	if(!target_file)
	{
		return FOPEN_ERR;
	}
	target_file_name = GetPreAsmTargetFName(src_file_name);
	target_file = fopen(target_file_name, "w+");
	if(!target_file)
	{
		fclose(src_file);
		free(target_file_name);
		return FOPEN_ERR;
	}

	*(assembler_data->macro_table) = HashCreate(128, HashFunc, PreAsmCompMacroFunc);
	
	ret = PreAsmParseSourceFile(src_file, target_file,assembler_data->macro_table);
	

	fclose(src_file);
	free(target_file_name);
	return ret;
}


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


int PreAsmCompMacroFunc(const void *data , void *param)
{
        user_data_t *my_data = (user_data_t *)data;    
        return !strcmp((const char *)my_data->macro.name ,(const char *)param);
}


