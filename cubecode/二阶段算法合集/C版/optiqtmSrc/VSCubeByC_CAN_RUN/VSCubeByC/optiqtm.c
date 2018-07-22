//FILE OPTIQTM.C

//#define _LINUX_
//Comment this line out if you compile with MinGW under Windows.
//This disables the SIGNAL handling.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <assert.h>
#include  <signal.h>
#include "cubedefs.h"

static char manString[256];
int subOptLev;
int symRed;

#ifdef _LINUX_
static sigjmp_buf jump_buf;
#endif

CubieCube cc_a;


#ifdef _LINUX_ 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void  user_break(int  n)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
{
printf("-- skipping cube --\n");
fflush(stdout);
siglongjmp(jump_buf, 1);
return;
}
#endif

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void  pp()
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
{
printf(".");
fflush(stdout);
return;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void sys_init(void)
{
	subOptLev = 2;
	symRed = 1;

	printf("initializing memory.\n");
	visitedA = (char *)calloc(NGOAL / 8 + 1, 1);//initialized to 0 by default
	visitedB = (char *)calloc(NGOAL / 8 + 1, 1);
	for (int l = 0; l<NTWIST; l++)
		movesCloserToTarget[l] = (short *)calloc(NFLIPSLICE * 2, 2);

	printf("initializing tables"); fflush(stdout);
	initSymCubes();
	initMoveCubes();
	initInvSymIdx();
	initSymIdxMultiply();
	initMoveConjugate();
	initMoveBitsConjugate();
	initGESymmetries();
	initTwistConjugate(); pp();
	initRawFLipSliceRep(); pp();
	initTwistMove(); pp();
	initCorn6PosMove(); pp();
	initEdge4PosMove(); pp();
	initEdge6PosMove(); pp();
	initSymFlipSliceClassMove(); pp();
	initMovesCloserToTarget(); pp();
	initNextMove(); pp();
	printf("\r\n");

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main(void)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
{
	sys_init();
	while (1)
	{
		printf("enter cube (x to exit): "); fflush(stdout);
		if (fgets(manString, sizeof(manString), stdin) == NULL) break;
		if (manString[0] == 'x') exit(EXIT_SUCCESS);
		//strcpy(manString, "F2 R2 U' D2 B2 F R B D L R D D R' U' B D");
		int l = strlen(manString);
		if (manString[l - 1] == '\n') manString[l - 1] = 0;//remove LF
		if (l>1 && manString[l - 2] == '\r') manString[l - 2] = 0;//remove CR if present
		if (strlen(manString) == 0) continue;//ignore empty lines
		printf("\nsolving optimal: %s\n", manString); fflush(stdout);
		cc_a = stringToCubieCube(manString);
		solveOptimal(cc_a);
	}
		system("pause");
	//exit(EXIT_SUCCESS);
	return 0;
}
