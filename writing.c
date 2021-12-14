#include "declarations.h"
#include "definitions.h"

/* The function prints the machine code in a "Hex-Decimal"*/
void writeObject(FILE* fd2)
{
	int line = 100;
	int i,j;
	int mask = 1;
	int ch1,ch2,ch3,ch4;	
	fprintf(fd2, "\t %d %d\n", commandSize, dataSize);
	for (i = 0; i<(IC-100) ; i++)
	{
		mask = 1;
                mask<<=23;
		funcLine(line,  fd2);/*Print the line number*/
		fputc('\t', fd2);
		for (j = 0; j < 6; j++)/*Passing the 24 places*/
		{
			ch1 = (mask&(instructionArr[i])); 
			mask >>= 1;
			ch2 = (mask&(instructionArr[i])); 
			mask >>= 1;
			ch3 = (mask&(instructionArr[i])); 
			mask >>= 1;
			ch4 = (mask&(instructionArr[i])); 
			mask >>= 1;
			binaryToHexa(fd2,ch1,ch2,ch3,ch4); /*replace 4 characters from binary to hexa and prints to fd2*/
		}
		fputc('\n', fd2);
		line++;
	}
        
        for (i = 0; i<DC ; i++)
	{
		mask = 1;
                mask<<=23;
		funcLine(line,  fd2);/*Print the line number*/
		fputc('\t', fd2);
		for (j = 0; j < 6; j++)/*Passing the 24 places*/
		{
			ch1 = (mask&(dataArr[i])); 
			mask >>= 1;
			ch2 = (mask&(dataArr[i])); 
			mask >>= 1;
			ch3 = (mask&(dataArr[i])); 
			mask >>= 1;
			ch4 = (mask&(dataArr[i])); 
			mask >>= 1;
			binaryToHexa(fd2,ch1,ch2,ch3,ch4); /*replace 4 characters from binary to hexa and prints to fd2*/
		}
		fputc('\n', fd2);
		line++;
	}
}
void binaryToHexa(FILE* fd2, int ch1,int ch2, int ch3, int ch4)
{	
	int i = 0;
	char hexa = 'a';
	int dcm = 0;
	if(ch1 != 0)
		dcm+=8;
	if(ch2 != 0)
		dcm+=4;
	if(ch3 != 0)
		dcm+=2;
	if(ch4 != 0)
		dcm+=1;
	if(dcm < 10)
		fprintf(fd2 ,"%d",dcm);
	else
		for(i=10;i<16;i++)
		{
			if(i == dcm)
			    fprintf(fd2, "%c", hexa);
			hexa++;
		}
}

void funcLine(int line, FILE* fd2)
{
	  if (line < 1000) /*there is maximum 1000 instructions*/
	     fputc('0000', fd2);
	  else
		 fputc('000', fd2);
	   
         fprintf(fd2, "%d", line); 
}

void writeExtern(FILE * fd2)
{
	int i;
	for (i = 0; extAdress[i]; i++)
	{
		fputs(extLabels[i], fd2);
		fputs("   ", fd2);
		fprintf(fd2, "%d", extAdress[i]);
		fputs("\n", fd2);
	}
}


void writeEntry(FILE * fd2)
{
	int i;
	for (i = 0; entAdress[i]; i++)
	{
		fputs(entLabels[i], fd2);
		fputs("   ", fd2);
		fprintf(fd2, "%d", entAdress[i]);
		fputs("\n", fd2);
	}

}/*End of function writeEntry*/
