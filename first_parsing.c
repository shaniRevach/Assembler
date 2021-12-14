#include "declarations.h"
#include "definitions.h"

struct opcode opcodes[] =
{
        { "mov",0,2,0,{ { 1,1,0,1 },{ 0,1,0,1 } } },
	{ "cmp",1,2,0,{ { 1,1,0,1 },{ 1,1,0,1 } } },
	{ "add",2,2,1,{ { 1,1,0,1 },{ 0,1,0,1 } } },
	{ "sub",2,2,2,{ { 1,1,0,1 },{ 0,1,0,1 } } },
	{ "lea",4,2,0,{ { 0,1,0,0 },{ 0,1,0,1 } } },
	{ "clr",5,1,1,{ { 0,0,0,0 },{ 0,1,0,1 } } },
	{ "not",5,1,2,{ { 0,0,0,0 },{ 0,1,0,1 } } },	
	{ "inc",5,1,3,{ { 0,0,0,0 },{ 0,1,0,1 } } },
	{ "dec",5,1,4,{ { 0,0,0,0 },{ 0,1,0,1 } } },
	{ "jmp",9,1,1,{ { 0,0,0,0 },{ 0,1,1,1 } } },
	{ "bne",9,1,2,{ { 0,0,0,0 },{ 0,1,1,1 } } },
	{ "jsr",9,1,3,{ { 0,0,0,0 },{ 0,1,1,1 } } },
	{ "red",12,1,0,{ { 0,0,0,0 },{ 0,1,0,1 } } },
	{ "prn",13,1,0,{ { 0,0,0,0 },{ 1,1,0,1 } } },
	{ "rts",14,0,0,{ { 0,0,0,0 },{ 0,0,0,0 } } },
	{ "stop",15,0,0,{ { 0,0,0,0 },{ 0,0,0,0 } } }
};


int firstMove(char* kelet)
{
	char word[31];
	int num;
	char label[31];
	int variableType=FALSE;
	int i = 0, j = 0;
	int legal = TRUE;

	if (kelet[i] == ';')/*It is a comment sentence*/
		return legal;

	i = getTillSpace(kelet, word);/*To get the first word*/

        if (i == NO)/*It is an empty sentence*/
		return legal;

	/*building symbols table*/
	if (ifSymbol(word))
	{
		strcpy(label, word);
		label[strlen(label) - 1] = '\0';/*To delete the character ':' .*/
		if (!(labelCheck(label)))
		{
			fprintf(stdout, "Line %d: ERROR: illegal label %s\n", LINE, label);
			legal = FALSE;
		}

		if (!(symbolExist(label)))
		{
			strcpy(name_symbol[labelsCounter],label);

			/*Reading the next word after a label*/
			for (; kelet[i] && (kelet[i] == ' ' || kelet[i] == '	'); i++);/*To avoiding white spaces*/
			for (; kelet[i] && kelet[i] != ' ' && kelet[i] != '\t' && kelet[i] != '\n' ; i++, j++)
				word[j] = kelet[i];
                        word[j] = '\0';

			if ((variableType = itVariable(word)))
			{
				adress_symbol[labelsCounter] = DC;
				ext_symbol[labelsCounter] = FALSE;
				ifCommand_symbol[labelsCounter] = FALSE;
				legal = addToData(kelet + i, word);
			}
			else/*It is not variable*/ 
			{
                           if ((num = itCmd(word)) != NO)/*It is a command sentence*/
                           {
				adress_symbol[labelsCounter] = IC;
				ext_symbol[labelsCounter] = FALSE;
				ifCommand_symbol[labelsCounter] = TRUE;
				if (num == 9)
					legal = (legal && updateICj(num, kelet + i));/*updateICj will call to updateIC*/
				else
					legal = (legal && updateIC(num, kelet + i));
                           }
			   else /*It is not variable and not a command*/
                           {
                              if (strcmp(word, ".extern") == 0 || strcmp(word, ".entry") == 0)
			      {
				  fprintf(stdout, "Line %d: WARNING: lable before extern definition is meaningless.\n", LINE);
				  adress_symbol[labelsCounter] = FALSE;
				  ext_symbol[labelsCounter] = TRUE;
				  ifCommand_symbol[labelsCounter] = FALSE;
			      }
                           
			      if((strcmp(word, ".string") != 0 && strcmp(word, ".data") != 0 && strcmp(word, ".extern") != 0 && strcmp(word, ".entry") != 0))
			      {
				 fprintf(stdout, "Line %d: ERROR: Your command %s is not exist.\n", LINE, word);
				 legal = FALSE;
			      }
                           }
                         }

                        labelsCounter++;
		}
		else/*if the symbol is allready exist*/
		{
			fprintf(stdout, "Line %d: ERROR: double definition of label %s.\n", LINE, word);
			legal = FALSE;
		}
	}/*end of if is it symbol*/

	else/*The sentence is without a symbol*/
	{
		if (strcmp(word, ".extern") == 0)
		{
			for (; kelet[i] && (kelet[i] == ' ' || kelet[i] == '\t' || kelet[i] == '\n'); i++);/*To avoiding white spaces*/
			for (; kelet[i] && kelet[i] != ' ' && kelet[i] != '\t' && kelet[i] != ',' && kelet[i] != '\n'; i++, j++)
				label[j] = kelet[i];
			label[j] = '\0';

			if (!(labelCheck(label)))
			{
				fprintf(stdout, "Line %d: ERROR: illegal label %s\n", LINE, label);
				legal = FALSE;
			}

			if (symbolExist(label))
			{
				fprintf(stdout, "Line %d: ERROR: double definition of label %s.\n", LINE, word);
				legal = FALSE;
			}
			else/*The symbol is not exists.*/
			{
				strcpy(name_symbol[labelsCounter],label);
				adress_symbol[labelsCounter] = FALSE;/*We don't know the adress of external variable*/
				ext_symbol[labelsCounter] = TRUE;
				ifCommand_symbol[labelsCounter] = FALSE;
	                        labelsCounter++;

			}
		}/*End of it is extern definition*/
	
		else if ((num = itCmd(word)) != NO)/*It is a command sentence*/
		{
			if (num == 9)
				legal = (legal && updateICj(num, kelet + i));/*updateICj will call to updateIC*/
			else
				legal = (legal && updateIC(num, kelet + i));
		}
		
	}/*end of if it is not a symbol*/

	commandSize = IC-100;
	dataSize = DC;

	return legal;
}/*End of function*/



int itVariable(char* word)
{
	int i = 0;
	for (; i<2; i++)/*if the guid is:.data or .stringn.*/
	{
		if (strcmp(word, guiding[i]) == 0)
			return TRUE;
	}
	return FALSE;
}

int addToData(char* toAdd, char* variableType)
{
        char help[30];/*To keep the number or string*/
	int comma = FALSE;/*We don't have a comma now*/
	int j = 0, minus = FALSE, numberToAdd = FALSE;
	char* i = toAdd;
	int legal = TRUE;
        int len= strlen(toAdd);
        if(toAdd[len-1]=='\n' || toAdd[len-1]=='\t' || toAdd[len-1]==' ')
        {
            toAdd[len-1]='\0';
            len= strlen(toAdd);
        }

	if (strcmp(variableType, ".data") == 0)
	{
		while ((*i) && ((*i) == ' ' || (*i) == '	' || (*i) == ',' || (*i) == '\n'))/*to avoiding white characters*/
		{
			if ((*i) == ',')
			{
				fprintf(stdout, "Line %d: ERROR: A comma before the first number at .data is forbbiden.\n", LINE);
				legal = FALSE;
			}
			i++;
		}
		comma = TRUE;/*For the first eteration*/
		while (*i)
		{
			while (isdigit(*i) || (*i) == '-')/*It is a digit or a minus before a digit*/
			{
				if (minus && !(isdigit(*i)))/*There is minus without any digit*/
				{
					fprintf(stdout, "Line %d: ERROR: minus without of number is forbbiden.\n", LINE);
					legal = FALSE;
				}
				if (minus && isdigit(*i))/*A negative number*/
					minus = FALSE;
				if ((*i) == '-' && numberToAdd)/*That mean we have minus after a number*/
				{
					fprintf(stdout, "Line %d: ERROR: minus after a number is forbbiden.\n", LINE);
					legal = FALSE;
				}
				numberToAdd = TRUE;
				if ((*i) == '-' && !minus)/*If minus=TRUE and we have more minus-it an error*/
				{
                                	minus = TRUE;
                                       
                                }
				else if ((*i) == '-' && minus)
				{
					fprintf(stdout, "Line %d: ERROR: Too many minuses.\n", LINE);
					legal = FALSE;
				}
				help[j] = (*i);
				if (isdigit(*i))
					comma = FALSE;/*Now we need a comma before the next number*/
				i++;
				j++;
			}/*End of: while (isdigit(*i) || (*i)== '-')*/

			help[j] = '\0';
			if (numberToAdd)
			{
				dataArr[DC] = atoi(help);
				DC++;
				numberToAdd = FALSE;
				minus = FALSE;
                                j=0;
                                help[0]='\0';
			}
			while ((*i) && ((*i) == ' ' || (*i) == '	'))/*to avoiding white characters*/
				i++;

			if (!comma)/*We need to get a comma or we at the last number*/
			{
				if ((*i) == ',' && checkEnd(i+1))/*That mean we are at the end of line and have unnecessary comma*/
				{
					fprintf(stdout, "Line %d: ERROR: comma after the last number is forbbiden.\n", LINE);
					legal = FALSE;
				}
                                else if ((*i) != ',' && checkEnd(i+1))/*That mean we are at the end of line and not need a comma*/ 
				{
					return legal;
				} 
				else if ((*i) != ',')
				{
					fprintf(stdout, "Line %d: ERROR: You miss a comma.\n", LINE);
					legal = FALSE;
					i++;
				}
				else if ((*i) == ',')
				{
                                 	comma = TRUE;
                                        i++;
                                }
			}
			else if (comma)/*We don't need a comma now.*/
			{
				if ((*i) == ',')
				{
					fprintf(stdout, "Line %d: ERROR: Too many commas sequencly.\n", LINE);
					legal = FALSE;
					i++;
				}
			}
			else if ((*i) == '.')
			{
				fprintf(stdout, "Line %d: ERROR: Decimals numbers are not supported.\n", LINE);
				legal = FALSE;
				i++;
			}
			else/* (*i) is not ',' or '.' */
			{
				fprintf(stdout, "Line %d: ERROR: illegal character.\n", LINE);
				legal = FALSE;
			}
			while ((*i) && ((*i) == ' ' || (*i) == '	'))/*to avoiding white characters*/
				i++;
		}/*End of while(*i)*/

		if (comma)/*There is a comma after the last number*/
		{
			fprintf(stdout, "Line %d: ERROR: A comma after the last number at .data is forbbiden.\n", LINE);
			legal = FALSE;
		}
	}/*End of .data*/


	else/*variableType is .string.*/
	{
		for (;(*i) && ((*i) == ' ' || (*i) == '\t'); i++,len--);/*To avoiding white spaces*/
		
		if ((*i) != '\"')
		{
			fprintf(stdout, "Line %d: ERROR: character %c is forbbiden here.\n", LINE, *i);
			legal = FALSE;
		}
		else  
		{
                	i++;/*to skip on the quotationMark*/
                        len--;
                }
		while (len)
		{
			if ((*i) != '\"')
			{
				dataArr[DC] = (*i);
				DC++;
				i++;
				len--;
			}
			else/* (*i) = " */
			{
				if (len >2)
				{
					fprintf(stdout, "Line %d: ERROR: More characters after the second quotationMark.\n", LINE);
					legal = FALSE;
                                        len--;
					break;
				}
                                else
                                  len--;
			}
		}/*End of While*/
                dataArr[DC] = 0;
                DC++;
	}/*End of .string*/

	return legal;

}/*End of function addToData*/


int updateIC(int opcode, char* kelet)
{  /*Look for errors and updates IC*/
	int legal = TRUE;
	int count = 1, comma = FALSE;/*count- how many memory words to that command(1: for the opcode)*/
	char* i = kelet;
	int j = 0;/*i go on kelet, j - go on help*/
	char help[31];/*The number or the num of reg or the label*/
	int regNameInUse = FALSE;/*We didnt use a memory word for registers name yet*/

	if (opcode == 9 )/*opcode jmp, bne or jsr.*/
		count++;/*for the adress of the label which we jump to it*/

	if (opcode == 14 || opcode == 15)/*commands without parameters*/
	{
		legal = checkEnd(i);/*count=1 just for the command*/
		IC += count;
		return legal;
	}

	if (opcode== 0 || opcode== 1 || opcode== 2 || opcode== 4 )/*We need to get 2 parameters*/
	{
		while ((*i) && ((*i) == ' ' || (*i) == '\t'))/*To avoiding white spaces*/
			i++;
		/*Now we are in the oparands names.*/

		/*To get the source operand*/
		for (; (*i) && (*i) != ',' && (*i) != ' ' && (*i) != '\t' && (*i) !='\n'; i++, j++)
			help[j] = (*i);
		help[j] = '\0';

		if (help[0] == '#')/*That mean immediatly - mioon 0*/
		{
			count++;
			if (opcode!=9 &&  !(opcodes[opcode].permit[0][0]))
			{
				fprintf(stdout, "line %d: mioon system not legal for opcode %s\n", LINE, opcodes[opcode].name);
				legal = FALSE;
			}
		}
		else if (isReg(help) != NO)
		{
			count++;
			regNameInUse = TRUE;
			if (opcode!=9 && !(opcodes[opcode].permit[0][3]))
			{
				fprintf(stdout, "line %d: mioon system not legal for opcode %s\n", LINE, opcodes[opcode].name);
				legal = FALSE;
			}
		}
		else /*probably it symbol,At second move we will check if that symbol is really exist.*/
		{
			count++;
			if (opcode!=9 && !(opcodes[opcode].permit[0][1]))
			{
				fprintf(stdout, "line %d: mioon system not legal for opcode %s\n", LINE, opcodes[opcode].name);
				legal = FALSE;
			}
                        
		}

		for (; (*i) && ((*i) == ',' || (*i) == ' ' || (*i) == '\t'); i++)/*For avoiding white characters*/
		{
			if ((*i) == ',')
			{
				if (comma)/*There is too many commas-it an error*/
				{
					fprintf(stdout, "line %d: too many commas\n", LINE);
					legal = FALSE;
				}
				else
					comma = TRUE;
			}
		}
	}/*End of: if(opcode==0 || opcode==1 || opcode==2 || opcode==3 || opcode==6 || opcode==9 || opcode==10 || opcode==13)*/

	 /*To get the destination operand- for commands with 2 operands either commands with 1 operand*/
	j = 0;
	help[j] = '\0';

	for (; (*i) && ((*i) == ',' || (*i) == ' ' || (*i) == '\t'); i++);/*For avoiding white characters*/
 
        for (; (*i) && (*i) != ' ' && (*i) != '\t' && (*i) != '\n'; i++, j++)
		help[j] = (*i);
	help[j] = '\0';

	if (help[0] == '#')/*That mean immediatly - mioon 0*/
	{
		count++;
		if (opcode!=9 && !(opcodes[opcode].permit[1][0]))
		{
			fprintf(stdout, "line %d: mioon system not legal for opcode %s\n", LINE, opcodes[opcode].name);
			legal = FALSE;
		}
		j = 0;
	}
	else if ((isReg(help)) != NO)
	{
		if (!regNameInUse)
		{
                        count++;
                        regNameInUse= TRUE;
                }
		if (opcode!=9 && !(opcodes[opcode].permit[1][3]))
		{
			fprintf(stdout, "line %d: mioon system not legal for opcode %s\n", LINE, opcodes[opcode].name);
			legal = FALSE;
		}
	}
	else /*probably it symbol*/
	{
		count++;
		if (opcode!=9 && !(opcodes[opcode].permit[1][1]))
		{
			fprintf(stdout, "line %d: mioon system not legal for opcode %s\n", LINE, opcodes[opcode].name);
			legal = FALSE;
		}
               
	}

	IC += count;
	legal = (checkEnd(i) && legal);

	return legal;
}/*End of function updateIC*/

int updateICj(int opcode, char* toAdd)
{ 
	
	char* i = toAdd;/*i-going on toAdd*/
	char* help = i;/*To check the legal of the operand*/
	int legal = TRUE; 

	for (; (*i) && ((*i) == ' ' || (*i) == '	'); i++);/*To avoiding white spaces*/
	
	for (; (*i) && (*i) != ' ' && (*i) != '\t' && (*i) != '\n' ; i++)
	
	if((*i)=='&')
	   i++;

	for (; (*i) && ((*i) == ' ' || (*i) == '	'); i++);/*To avoiding white spaces*/
	

	if (checkEnd(i))/*We arrive to the end of that input line - this is operand without parameters*/
	{
		IC += 2;/*For the code of the command and the label or distance*/
		return legal;
	}

	return FALSE;

}/*end of function updateICj*/


void updateAdress()
{
	int i;

	for (i = 0; i<labelsCounter ; i++)
		if (!(ifCommand_symbol[i]) && !(ext_symbol[i]))/*it is a data symbol*/
		{
			adress_symbol[i] += IC;
			
		}
}/*End of function updateAdress*/
