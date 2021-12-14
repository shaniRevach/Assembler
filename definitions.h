#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define TRUE 1
#define FALSE 0
#define NO -1
#define KELET_SIZE 81


/*processor components ------------------------------------*/

/* 7 registers*/


/* memory words */

typedef union
{
    struct 
    {
	unsigned int are : 3;
	unsigned int distanceOrImmed : 21;
    }a;
    unsigned int distJumpOrImmed_;
}distJumpOrImmed;

/*----------------------------------------------------------------*/

/* Instructions in that language------------------------------------------------------------------------------------------*/

/* machine command */
typedef union
{
    struct 
   {
	unsigned int are : 3;
	unsigned int funct : 5;
	unsigned int destReg : 3;
	unsigned int destAddress : 2;
	unsigned int sourceReg : 3;
	unsigned int sourceAddress: 2;
	unsigned int opcode : 6;
   }u;
   unsigned int command_;
}command;

/* Types of opcodes - Instructions sentences */

struct opcode
{
	char name[5];/* 3-4 chars +'\0' */
	int code;/* code between 0-15 */
	int oper;/* how many operands for that command (0-2) */
	int funct;/*code beetween 0-4*/
	int permit[2][4];/* table of 2*4 permitable source or destination operand: immidate, direct, jump, register */
	
};



/* 4 mihoon systems - How to use the opcodes*/
typedef enum
{
	immidiate, direct, jump, direct_reg
}SYSTEMS;

/*Type of encoding- absolute(A=100), relocatable(R=010) or external(E=001). */
typedef enum
{
	A=4,R=2,E=1
}ARE;

/* To 10-13 bits, mioon system num.2*/
typedef enum
{
	now_num, label, reg = 3
}miun2;




/*-----------------------------------------------------------------------------------------------------------------------------------*/


void assembler(FILE*, FILE*, int, char**, int);

/* Functions of first_parsing.c */
int firstMove(char*);
int itVariable(char*);
int addToData(char*, char*);
int updateIC(int, char*);
int updateICj(int , char*);
void updateAdress();

/* Function of second_parsing.c */
int secondMove(char*);

/* Functions of helpFunc.c */
int checkArgc(int);
char* getLine(FILE*, char*);
int getTillSpace(char*, char*);
int ifSymbol(char*);
void toFree();
int labelCheck(char*);
char* help(char*);
int symbolExist(char*);
int itCmd(char[]);
int itCmdFunct(char []);
int getAdress(char*);
int getNextWord(char* help, char*kelet, int i);
int isReg(char* regName);
int checkEnd(char*);

/* Functions of writing.c */
void writeObject(FILE *);
void writeExtern(FILE *);
void writeEntry(FILE *);
void funcLine(int, FILE*);
void binaryToHexa(FILE* , int ,int , int , int );
