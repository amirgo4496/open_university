#include "../include/my_libs.h"
#include "../include/glibc_libs.h"

int main()
{
	char a[] = "aaa" , *b = "mov" , c[] = "bne" , d[] = "AAA";

	printf("%d\n" ,IsOperationName(a));
	printf("%d\n" ,IsOperationName(b));
	printf("%d\n" ,IsOperationName(c));
	printf("%d\n" ,IsOperationName(d));
	return 0;
}
