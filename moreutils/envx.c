#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char const *argv[])
{
	char str[80];
	char* pid = argv[1];
	if( argc<2 )  return 1;
	
	strcpy(str, "/proc/");
	strcat(str, pid);
	strcat(str, "/environ");
	
// read from file
	char ch;
	FILE *fp;
	fp = fopen(str,"r");
	printf("Text from file:%s\n", str);
	
	while( ( ch = fgetc(fp) ) != EOF )
	printf("%c",ch);
 
	fclose(fp);
	

	printf("\n");
	return 0;
}
