#include "globals.h"
#include "definitions.h"

int main(int argc, char* argv[])
{
	int i;
	FILE* fd1, *fd2=NULL;/* fd1-for reading, fd2-for writing.*/

	if (!(checkArgc(argc)))
		exit(0);

	for (i = 1; i<argc; i++)
	{
		strcpy(fileName, argv[i]);
		strcat(fileName, ".as");/* The name of the file is recived without the suffix ".as". */

		if (!(fd1 = fopen(fileName, "r")))
		{
			fprintf(stderr, "There a problem with the file %s\n", fileName);
			continue;/* To pass to the next file*/
		}

		assembler(fd1, fd2, argc, argv, i);/*To do 2 moves and print to 3 files*/

		fclose(fd1);

		/* fd2 closing in assembler.c */
	}
	return 0;
}

