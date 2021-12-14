#include "definitions.h"
#include "declarations.h"

void assembler(FILE* fd1, FILE* fd2, int argc, char** argv, int i)
{
       char kelet[KELET_SIZE];
	int legal = TRUE, legalCopy = legal;
	char* p = NULL;

	if(fgetc(fd1) == EOF)
	{
			fprintf(stdout, "The file is empty.\n");
			exit(0);
	}
	rewind(fd1);
	while ((p = getLine(fd1, kelet)))/*Read 1 line from file and doing the first move*/
	{
		   legalCopy = firstMove(p);/*In order to don't delete the value of legal*/
		   legal = (legalCopy && legal);/*If legal or legalCopy is FALSE - legal will be FALSE*/
	}
    
	rewind(fd1);
	LINE = 0;
        updateAdress();/*To put the right adresses of the data after all the insructions*/
	IC = 100;
        

	
	while ((p = getLine(fd1, kelet)))/*Read 1 line secondary from file and doing the second move*/
	{
		legalCopy = secondMove(p);
		legal = (legalCopy && legal);
	}
	

	if(legal)/*There were no errors at the 2 moves, so we can create 3 files.*/
	{
             if(IC)/*Just if the object file won't be empty, we create it*/
             {
	        strcpy(fileName, argv[i]);
		strcat(fileName, ".ob");/* To create the object file.*/
		if (!(fd2 = fopen(fileName, "w")))
			fprintf(stdout, "can't open the file %s\n", fileName);
		else
			writeObject(fd2);

             }

             if(extCount)/*Just if the extern file won't be empty, we create it*/
             {
		strcpy(fileName, argv[i]);
		strcat(fileName, ".ext");/* To create the extern file.*/
		if (!(fd2 = fopen(fileName, "w")))
			fprintf(stdout, "can't open the file %s\n", fileName);
		else
			writeExtern(fd2);
             }
             if(entCount)/*Just if the entry file won't be empty, we create it*/
             {
		strcpy(fileName, argv[i]);
		strcat(fileName, ".ent");/* To create the entry file.*/
		if (!(fd2 = fopen(fileName, "w")))
			fprintf(stdout, "can't open the file %s\n", fileName);
		else
			writeEntry(fd2);
             }
	}
        if(legal)
            fclose(fd2);
}


