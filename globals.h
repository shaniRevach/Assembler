#define MAX_MEMORY 256

int LINE = 0;/*How many lines we allready read from file.*/
char fileName[30];

/*To storage the code to the object file*/
int instructionArr[1000];
int IC = 100;
int indexInstructionArr = 0;
int dataArr[500];
int DC = 0;

int commandSize = 0;
int dataSize = 0;

/* For symbol table*/
char name_symbol[MAX_MEMORY][31];
int adress_symbol[MAX_MEMORY]={0};
int ext_symbol[MAX_MEMORY]={0};
int ifCommand_symbol[MAX_MEMORY]={0};
int labelsCounter=0;

/*To keep the entries and externals*/
int extAdress[500];
char extLabels[500][31];
int entAdress[500];
char entLabels[500][31];
int extCount = 0, entCount = 0;/*Indexes at the arrays of the externals and entries*/


/* Types of guiding sentences */
char* guiding[4] =
{ ".data",".string",".entry",".extern" };

char* cpu_regs[] =
{ "r0","r1","r2","r3","r4","r5","r6","r7" };

