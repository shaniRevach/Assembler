#include "declarations.h"
#include "definitions.h"
extern struct opcode opcodes[];

int secondMove(char* kelet)
{
	int num1 = NO, num2 = NO, i = 0;/*num1=number of opcode, num2=number of funct*/
	int legal = TRUE, withParameters = TRUE;
	int ICcopy = IC + 1-100;/*cause we put the command word at the array after we put there the encoding of the operands*/
	char help[31];/*The number or the num of reg or the label*/
	int regNum= NO;/*Which register is it - r0, r1........*/
	int j;
	
	command cmd;
	distJumpOrImmed immedNum;
	distJumpOrImmed adress;
	
	if (kelet[i] == ';')/*It is a comment sentence*/
		return legal;

	i += getTillSpace(kelet, help);/*To get firstWord*/

	if (i == NO)/*It is an empty sentence*/
		return legal;

	if (ifSymbol(help))
	{/*We need to get the next word, Symbols were already treated at first move*/
		i += getTillSpace(kelet + i, help);
	}

	help[i] = '\0';

	if (strcmp(help, ".entry") == 0)
	{
		i = getNextWord(help, kelet, i);
		entAdress[entCount] = getAdress(help);
		if (entAdress[entCount] == NO)/*It is not exist at symbolsTable*/
		{
			fprintf(stdout, "Line %d: ERROR: .entry guiding on %s label- is forbbiden.\n", LINE, help);
			legal = FALSE;
		}
		if (entAdress[entCount])/*That mean it is not an external so the adress is not 0*/
		{
			strcpy(entLabels[entCount], help);
			entCount++;
		}
		else/*It is an external label*/
		{
			fprintf(stdout, "Line %d: ERROR: .entry guiding on an external variable- is forbbiden.\n", LINE);
			legal = FALSE;
		}
	}

	else if (((num1 = itCmd(help)) != NO) && ((num2 = itCmdFunct(help)) != NO))/*It is a command sentence*/
	{

		if (num1 == 9)
		{

		cmd.u.are = A;
		cmd.u.funct = num2;   
	        cmd.u.destReg = 0;
	        cmd.u.sourceReg = 0;
		cmd.u.sourceAddress =0;
		cmd.u.opcode = num1;
		
		i = getNextWord(help, kelet, i);/*Now we have at "help" the &label or label*/
		
		if(help[0] == '&')
		{
			
		 	cmd.u.destAddress = jump;

			for(j=0;j<strlen(help);j++)
			  help[j]=help[j+1];

		        adress.a.are = A;		     
			adress.a.distanceOrImmed = getAdress(help) - IC;

			if ((getAdress(help)) == NO)/*That mean there is no such label*/
			{
				fprintf(stdout, "Line %d: ERROR: The label %s is not exist.\n", LINE, help);
				legal = FALSE;
			}
                        if ((getAdress(help)) == 0)/*That mean it is an external label*/
                        {
				extAdress[extCount] = ICcopy+100;/*To here someone will need to insert the really adress of the extern label*/
				strcpy(extLabels[extCount], help);
				extCount++;
				adress.a.are = E;
			}
			instructionArr[ICcopy] = adress.distJumpOrImmed_;
			ICcopy++;

			

			
		}
	        else
		{
		 cmd.u.destAddress = direct;

		 adress.a.are = R;
		 adress.a.distanceOrImmed = getAdress(help);
		   if ((getAdress(help)) == NO)/*That mean there is no such label*/
	           {
			fprintf(stdout, "Line %d: ERROR: The label %s is not exist.\n", LINE, help);
			legal = FALSE;
		   }
                   if ((getAdress(help)) == 0)/*That mean it is an external label*/
                   {
			extAdress[extCount] = ICcopy+100;/*To here someone will need to insert the really adress of the extern label*/
			strcpy(extLabels[extCount], help);
			extCount++;
			adress.a.are = E;
		    }
		instructionArr[ICcopy] = adress.distJumpOrImmed_;
		ICcopy++;

		}

		instructionArr[IC-100] = cmd.command_;
		IC = ICcopy+100;
		}

		if (num1 == 14 || num1 == 15 || (!(withParameters)))/*commands without parameters*/
		{
		      cmd.u.are = A;
		      cmd.u.funct = num2;
		      cmd.u.destReg = 0;
		      if(!(withParameters))
		         cmd.u.destAddress = direct;			 
		      else
                         cmd.u.destAddress = 0;
		      
		      cmd.u.sourceReg = 0; 
                      cmd.u.sourceAddress = 0;
		      cmd.u.opcode = num1;
		      
                      
		      instructionArr[IC-100] = cmd.command_;
		      IC = ICcopy+100;/*if it only 1 memory word- IC+=1, if it mioon 2 IC+=2*/
		      
		      return legal;
		}

		else if (num1 == 0 || num1 == 1 || num1 == 2 || num1 == 4)/*We need to get 2 operands*/
		
		{
			/*Now we wanna update the first operand ---------------------------------------------------------------------*/
              
			i = getNextWord(help, kelet, i);/*Now we have at "help" the first operand*/
                        help[i]='\0';
                       

			if (help[0] == '#')
			{
			       immedNum.a.are = A;
			       immedNum.a.distanceOrImmed = atoi(help + 1);/*To skip on "#" */

				instructionArr[ICcopy] = immedNum.distJumpOrImmed_;
				ICcopy++;

				cmd.u.sourceAddress = immidiate;
				cmd.u.sourceReg = 0;
			}
			else if ((regNum = isReg(help)) != NO)
			{
			        cmd.u.sourceReg = regNum;
			        cmd.u.are = A;
			        cmd.u.sourceAddress = direct_reg;
			}
			else /*probably it symbol. we need to check if that symbol is really exist.*/
			{
			        adress.a.are = R;
			        adress.a.distanceOrImmed = getAdress(help);
				if ((adress.a.distanceOrImmed) == NO)/*That mean there is no such label*/
				{
					fprintf(stdout, "Line %d: ERROR: The label %s is not exist.\n", LINE, help);
					legal = FALSE;
				}
				if ((adress.a.distanceOrImmed) == 0)/*That mean it is an external label*/
				{
					extAdress[extCount] = ICcopy+100;/*To here someone will need to insert the really adress of the extern label*/
					strcpy(extLabels[extCount], help);
					extCount++;
				adress.a.are = E;
				}
				instructionArr[ICcopy] = adress.distJumpOrImmed_;
				ICcopy++;

				cmd.u.sourceAddress = direct;
			}

			/*Now we wanna update the second operand ---------------------------------------------------------------------*/

			i = getNextWord(help, kelet, i);/*Now we have at "help" the second operand*/
                        help[i] = '\0';

			if ((regNum = isReg(help)) != NO)
			{
			        cmd.u.destReg = regNum;
			        cmd.u.are = A;
				cmd.u.destAddress = direct_reg;
			}

			if (help[0] == '#')/*That mean immediatly - mioon 0.*/
			{ 
			        immedNum.a.are = A;
			        immedNum.a.distanceOrImmed = atoi(help + 1);/*To skip on "#" */

				instructionArr[ICcopy] = immedNum.distJumpOrImmed_;
				ICcopy++;

				cmd.u.destAddress = immidiate;
			}

			else if(regNum ==NO) /*probably it symbol, we need to check if that symbol is really exist.*/
			{
			        adress.a.are = R;
			        adress.a.distanceOrImmed = getAdress(help);
				if ((adress.a.distanceOrImmed) == NO)/*That mean there is no such label*/
				{
					fprintf(stdout, "Line %d: ERROR: The label %s is not exist.\n", LINE, help);
					legal = FALSE;
				}
				if ((adress.a.distanceOrImmed) == 0)/*That mean it is an external label*/
				{
					extAdress[extCount] = ICcopy+100;/*To here someone will need to insert the really adress of the extern label*/
					strcpy(extLabels[extCount], help);
					extCount++;
				        adress.a.are = E;
				}
				instructionArr[ICcopy] = adress.distJumpOrImmed_;
				ICcopy++;
				cmd.u.destAddress = direct;
				cmd.u.destReg=0;
			}
			cmd.u.funct = num2;
			cmd.u.opcode = num1;
			instructionArr[IC-100] = cmd.command_;
			IC = ICcopy+100;

		}/*End of 2 operands case*/

		else /*It is command with 1 operand - */
		{
		        cmd.u.sourceAddress = 0;/*There is no source operand*/
			cmd.u.sourceReg = 0;
		        /*We need to update only the second operand---------------------------------------------------------------------*/

			i = getNextWord(help, kelet, i);/*Now we have at "help" the second operand*/

			if ((regNum = isReg(help)) != NO)
			{
				cmd.u.are = A;
			        cmd.u.destAddress = direct_reg;
				cmd.u.destReg = regNum;
			}

			if (help[0] == '#')/*That mean immediatly - mioon 0.*/
			{
			        immedNum.a.are = A;
			        immedNum.a.distanceOrImmed = atoi(help + 1);/*To skip on "#" */
				instructionArr[ICcopy] = immedNum.distJumpOrImmed_;
				ICcopy++;
			        cmd.u.destAddress = immidiate;
			}

			else /*probably it symbol,we need to check if that symbol is really exist.*/
			{
			        adress.a.are = R;
			        adress.a.distanceOrImmed = getAdress(help);
				if ((adress.a.distanceOrImmed) == NO)/*That mean there is no such symbol*/
				{
					fprintf(stdout, "Line %d: ERROR: The label %s is not exist.\n", LINE, help);
					legal = FALSE;
				}
				if ((adress.a.distanceOrImmed) == 0)/*That mean it is an external symbol*/
				{
					extAdress[extCount] = ICcopy+100;/*To here someone will need to insert the really adress of the extern label*/
					strcpy(extLabels[extCount], help);
					extCount++;
				        adress.a.are = E;
				}
				instructionArr[ICcopy] = adress.distJumpOrImmed_;
				ICcopy++;
			        cmd.u.destAddress = direct;
			}
			cmd.u.funct = num2;
			cmd.u.opcode = num1;
			instructionArr[IC-100] = cmd.command_;
			IC = ICcopy+100;

		}/*End of 1 operand case*/
	}/*End of : if((num=itCmd(word))!= NO) that mean: It is a command sentence*/

	 /*If it is not command and not .entry, It is .data or .string or .extern
	 - that we don't have what to do with it at that move- or if it illegal word-
	 and we already say it to user at first move*/

	return legal;

}/*End of function secondMove*/

