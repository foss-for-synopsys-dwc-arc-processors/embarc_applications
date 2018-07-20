#ifndef __CMD_H__
#define __CMD_H__

#define varnum 23

/* Mail */
typedef struct{
	int * value;
	char * name;
}VarAndName;

//const int varnum = 16 + 4 + 2 + 4 + 4;
extern VarAndName varname[varnum];
void paramCorr(void);
	
#endif