#include "declarations.h"
#include "definitions.h"
extern struct opcode opcodes[];

int checkArgc(int argc)
{
	if (argc<2)
	{
		fprintf(stderr, "No arguments inserted in command line.\n");
		return FALSE;
	}
	return TRUE;
}

char* getLine(FILE* fd, char* kelet)
{
        LINE++;
	if(fgets(kelet, KELET_SIZE, fd)==NULL)
          return NULL;/*to get line from file till '\n' or 81 characters.*/
        return kelet;
}

int getTillSpace(char* kelet, char* firstWord)
{
	int i = 0, j=0;
	for (; kelet[i] && (kelet[i] == ' ' || kelet[i] == '\t'); i++);/*To avoiding white characters*/
	for (; kelet[i] && kelet[i] != ' ' && kelet[i] != '\t' && kelet[i] != '\n'; i++,j++)
		firstWord[j] = kelet[i];
	firstWord[j] = '\0';
        if(j==0)
           return NO;
	return i + 1;/*without the white space*/
}

int ifSymbol(char* firstWord)
{
	int len = strlen(firstWord);
	if (firstWord[len - 1] == ':')
		return TRUE;
	return FALSE;/*It is not a symbol*/
}

int labelCheck(char* label)
{
	int flag = TRUE;
	int i = 0;
	if (isalpha(label[i]))/*Checks whether the first place is a letter*/
	{
		for (i = 1; label[i] && i < 31 && flag; i++)/*Checks whether all the parts are only numbers and letters*/
		{
			if (!(isalpha(label[i])) && !(isdigit(label[i])))
			{
				if (label[i] != ':')/*Checks whether the last character in the label is a colon*/
					flag = FALSE;
			}
		}
		if (symbolExist(label))/*Checks whether the label is already present*/
			flag = FALSE;
	}
	else
		flag = FALSE;

	if (strcmp(help("r0"), label) ==FALSE || strcmp(help("r1"), label) == FALSE || strcmp(help("r2"), label) == FALSE || strcmp(help("r3"), label)    ==     FALSE || strcmp(help("r4"), label) == FALSE || strcmp(help("r5"), label) == FALSE || strcmp(help("r6"), label) == FALSE || strcmp(help("r7"), label) == FALSE ||
		strcmp(help("mov"), label) == FALSE || strcmp(help("cmp"), label) == FALSE || strcmp(help("add"), label) == FALSE || strcmp(help("sub"), label) == FALSE || strcmp(help("not"), label) == FALSE || strcmp(help("clr"), label) == FALSE || strcmp(help("lea"), label) == FALSE || strcmp(help("inc"), label) == FALSE || strcmp(help("dec"), label) == FALSE || strcmp(help("jmp"), label) == FALSE || strcmp(help("bne"), label) == FALSE || strcmp(help("red"), label) == FALSE || strcmp(help("prn"), label) == FALSE || strcmp(help("jsr"), label) == FALSE || strcmp(help("rts"), label) == FALSE || strcmp(help("stop"), label) == FALSE ||
		strcmp(help("data"), label) == FALSE || strcmp(help("string"), label) == FALSE || strcmp(help("entry"), label) == FALSE || strcmp(help("extern"), label) == FALSE)

		flag = FALSE;
	if (flag)
		return TRUE;
	else
		return FALSE;
}

char* help(char* string)
{
	static char ch[31];
	strcpy(ch, string);
	strcat(ch, ":");
	return ch;
}

int symbolExist(char* label)
{
	int i;
	for (i = 0; i<labelsCounter; i++)
        {
		if (strcmp(name_symbol[i], label) == FALSE)
			return TRUE;
        }
	return FALSE;
}


int getNextWord(char* help, char* kelet, int i)
{
	int j = 0;
        help[0]= '\0';
	while (kelet[i] && (kelet[i] == ' ' || kelet[i] == '\t' || kelet[i] == '(' || kelet[i] == ',' ))
		i++;
	while (kelet[i] && kelet[i] != ' ' && kelet[i] != '\t' && kelet[i] != ','  && kelet[i] != '('&& kelet[i] != ')'  && kelet[i] != '\n')
	{
		help[j] = kelet[i];
		i++;
		j++;
	}
	help[j] = '\0';
	return i;
}

int getAdress(char* help)
{
	int i ;
	for(i=0 ; i<labelsCounter; i++)
        {
             if(strcmp(name_symbol[i],help)==0)
                 return adress_symbol[i];
        }
        return NO;
}

int checkEnd(char* i)
{
	while ((*i) && (*i) != '\n')
	{
		if ((*i) && (*i) != ' ' && (*i) != '\t' && (*i) != '\n')
			return FALSE;
		i++;
	}
	return TRUE;
}

int isReg(char* regName)
{ /*if it a name of register -return the number of an register.else return NO*/
	int i = 0;
	for (; i<8; i++)
        {
		if (strcmp(regName, cpu_regs[i]) == 0)
			return i;
        }
	return NO;/*if it not a regName*/
}

int itCmd(char word[])
{
	int i = 0;

	for (;i<16; i++)
		if (strcmp(opcodes[i].name, word) == 0)
		{    
                    return opcodes[i].code;
                }
	return NO;
}
int itCmdFunct(char word[])
{
		int i = 0;

	for (;i<16; i++)
		if (strcmp(opcodes[i].name, word) == 0)
		{    
                    return opcodes[i].funct;
                }
	return NO;
}

