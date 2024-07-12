#include "../include/flags.h"
#include "../include/glibc_libs.h"
#include "../include/utils.h"
#include "../include/my_libs.h"


int main(int argc, char **argv)
{

	FILE *src_file = fopen("test_file", "r+");
	FILE *target_file = fopen("target_file", "w+");

	ParseSourceFile(src_file, target_file);
	
	fclose(src_file);
	fclose(target_file);
	return 0;
}
