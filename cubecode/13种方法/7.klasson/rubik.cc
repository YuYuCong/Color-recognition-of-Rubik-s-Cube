/*
Mikael Klasson
mikkl605@student.liu.se
Linköping, Sweden

Front		Middle		Back
ULF UF UFR	UL --- UR	UBL UB URB
FL --- FR	-- --- --	BL --- BR
DFL DF DRF	DL --- DR	DLB DB DBR
0-8		9-17		18-26

012		901		890
345		234		123
678		567		456

Moves:

Corners:
Move	Start	Move steppings	Rotations corner
F1:	0	(2,6,-2,-6)	Left,Right,Left,Right	(ABC in ULF -> BCA in pos UFR, ABC in UFR -> CAB in DRF, ...)
B1:	20	(-2,6,2,-6)	Left,Right,Left,Right	(URB->RBU,...)
U1:	18	(2,-18,-2,18)	None,None,None,None
D1:	6	(2,18,-2-18)	None,None,None,None
L1:	18	(-18,6,18,-6)	Left,Right,Left?,Right?
R1:	2	(18,6,-18,-6)	Left,Right,Left?,Right?

Edges:
Move	Start	Move steppings	Flip faces (e.g. UF->FR)
F1:	1	(4,2,-4,-2)	1,1,1,1
B1:	19	(2,4,-2,-4)	1,1,1,1
U1:	19	(-8,-10,8,10)	0,0,0,0
D1:	7	(10,8,-10,-8)	0,0,0,0
L1:	9	(-6,12,6,-12)	0,0,0,0
R1:	11	(12,-6,-12,6)	0,0,0,0

Face numbers:
0	Front
1	Back
2	Up
3	Down
4	Left
5	Right

TODO: size shaving
a[0]	 -> *a
(a+b)%4  -> a+b&3
while(1) -> loop: ... goto loop;
standardize variable names and put more into #defines
'R' -> 82
#define funcdecl(name) void name() { int j = 0;
use ## and # in defines to cut down on the size of string literals (omitting ")
{ single_statement } -> single_statement

Multimove constants:
Fn	64+n	Bn	68+n	fn	96+n	rn	100+n
Un	72+n	Dn	76+n	bn	104+n	ln	108+n
Ln	80+n	Rn	84+n

//UF UR UB UL  DF DR DB DL  FR FL BR BL   UFR URB UBL ULF DRF DFL DLB DBR
//1, 11,19,9,  7, 17,25,15, 5, 3, 23,21,  2,  20, 18, 0,  8,  6,  24, 26
//int	aindices[] = {1, 11,19,9,  7, 17,25,15, 5, 3, 23,21,  2,  20, 18, 0,  8,  6,  24, 26};
*/

#include <iostream>
#include <algorithm>
using namespace std;

int	basemovecnt,	//moves*2 in moves[]
	j;
char	cubiepos[27],	//all the little cubies' current positions
	poscubie[27],	//which cubie is at a pos
	cubierot[27],	//rotations of cubies (flips and twists)
	moves[999],	//unmerged moves (Fn Fn can be present)
	*all_moves = "A@BFYUAADBWYATYFBUASBDYW@RBIYR@SSQHJ@FBRYI@GJHQSARIFRU@IULFOABRFIU@KLUOF";

//TODO: just store 2 step values. the others are just sign changed
//int ___all_moves[] = {
//	//rotation identifier, start cubie, move steppings	(corner)
//	//flip faces (e.g. UF->FR), start cubie, move steppings	(edge)
//	1,0,	2,6,25,21,	//F1
//	1,1,	4,2,23,25,	//F1
//
//	1,20,	25,6,2,21,	//B1
//	1,19,	2,4,25,23,	//B1
//
//	0,18,	2,9,25,18,	//U1
//	0,19,	19,17,8,10,	//U1
//
//	0,6,	2,18,25,9,	//D1
//	0,7,	10,8,17,19,	//D1
//
//	1,18,	9,6,18,21,	//L1
//	0,9,	21,12,6,15,	//L1
//
//	1,2,	18,6,9,21,	//R1
//	0,11,	12,21,15,6	//R1
//};

//UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR		//solved
void domove( int move, int count ) {
	moves[basemovecnt++] = count;
	moves[basemovecnt++] = move;
	while( count-- )
		for( char * m = all_moves + move*12, i = 3; i > 1; --i )
			for( int j = 0, p, r = *m++ - 64,
			  startp = p = *m++ - 64; 
			  j < 4; ++j ) {
				p = ( p + *m++ - 64 ) % 27;
				swap( poscubie[startp], poscubie[p] ); 
				swap( cubiepos[poscubie[p]], cubiepos[poscubie[startp]] );
				( cubierot[poscubie[p]] += i - r ) %= i;
				r = -r;
			}
}

/*
//move format:
count in bits 0..1
move number in bits 2..4
bit 5 set if move number is relative to given side number.
bits 7..6 = 01 to encode move in characters 64-95 == '@'..'_' for normal moves
					    96-127 for relative moves
multimove( "AJg" ) == moves 65,74,103 = F1,U2,(nextside)3
*/

#define	multimove(movestring,relative_to) multimove_literal(#movestring,relative_to);
void multimove_literal( char * p, int relative_to = 0 ) {
	while( char c = *p++ )
		domove( c & 32 ? "@EAD"[(relative_to + c/4) % 4]&7 : c/4 & 7,	//"@EAD"&7 == {0,5,1,4} == F,R,B,L moves
			c & 3 );
}

//Beware! This doesn't always produce the same output as the old sideface* routines. Still seem to score better overall though... oh well.
//Perhaps it's worse... investigate.
//returns what side face an edge cubie is on (assumes it is in top or bottom layer)
//returns what side face a side cubie is on the _right_ side of
//returns what side face a corner cubie is on the _right_ side of
#define	getside(cubie) "3003*03003*1***3*12212*1221"[cubiepos[cubie]] & 3

// 0 == top layer, 1 == middle layer, 2 = bottom layer
#define layer		cubiepos[cubie] % 9 / 3
#define iscorrect	( cubiepos[cubie] == cubie && !cubierot[cubie] )
#define	forjfrom0to3	for( j = 0; j < 4; ++j ) {

int main( int cubie, char ** argv ) {
	for( ; j < 20; ++j ) {
		char * p = argv[j+1], len = 2 + j / 12;
		for( int i = 0; i < len; ++i, rotate( p, p + 1, p + len ) )
			for( int k = 0; k < 27; ++k )
				if( !strncmp( p, "ULFUF UFRFL*   FR DFLDF DRFUL ***UR*   ***   DL*   DR UBLUB URBBL*   BR DLBDB DBR" + k*3, 3 - ( j < 12 && k % 2 ) ) ) {
					cubiepos[k] = "AKSIGQYOECWUBTR@HFXZ"[j]-64;
					cubierot[k] = i;
					poscubie[cubiepos[k]] = k;
				}
	}

	//step 1
	//is any edge down face already facing down?
	//int	aedges[] = {7,17,25,15};//,
	//	turn = 9;
	//for( int j = 0; j < 4; ++j )
	//	//if( *cube[aedges[j]] == 'D' ) {
	//	//if( !cubierot[cubiepos[aedges[j]]] ) {
	//	if( layer( aedges[j] ) > 1 && !cubierot[aedges[j]] ) {
	//		turn = aedges[j];
	//		//is it on the correct side as well?
	//		if( cube[aedges[j]][1] == solvedcube[aedges[j]][1] ) {
	//			turn = 9;
	//			break;
	//		}
	//	}
	////turn down layer to put downfacing cubie on correct side
	////if( turn < 9 )
	//if( turn != 9 )
	//	//for( int cnt = -1; !iscorrect( aedges[turn+cnt & 3] ); ++cnt )
	//	//for( ; !iscorrect( aedges[turn & 3] ); ++turn )
	//	for( ; !iscorrect( turn ); )
	//		multimove( M, 0 )	//D1

	//put all down edge cubies in correct place
	//for( j = 0; j < 4; ++j ) {
	forjfrom0to3
		cubie = "GQYO"[j] - 64;	// {7,17,25,15} == DF,DR,DB,DL
		//is cubie correct?
		if( iscorrect )
			continue;
		int r = getside( cubie );
		//is cubie in bottom layer?
		if( layer > 1 )
			multimove( a, r )	//f1	(put it in middle layer)
		//is it in the middle layer?
		if( layer & 1 ) {
			multimove( a, r )	//f1
			if( !iscorrect ) {
				multimove( b, r )	//f2
				if( !iscorrect )
					if( layer > 1 )
						multimove( bIc, r )	//f2 U1 f3
					else
						multimove( Ia, r )	//U1 f1
			}
		}

		//top layer now (otherwise it's correct)
		if( iscorrect )
			continue;
		//turn top layer till cubie is above its correct pos
		while( cubiepos[cubie] + 6 - cubie )
			multimove( I, 0 )	//U1
		//is bottom face up?
		if( cubierot[cubie] )
			multimove( Kgae, j )	//U3 r3 f1 r1
		else
			multimove( b, j )	//f2
	}
	//cout << "***** Stage 1 completed *****" << endl; print();



	//step 2
	//for( j = 1; j < 4; ++j ) {
	forjfrom0to3
		!j && ++j;
		cubie = "HZXF"[j] - 64;	// {8,26,24,6} == DRF,DBR,DLB,DFL
		// is it in the bottom layer?
		if( iscorrect )
			continue;
		if( layer > 1 )
			multimove( cKa, getside( cubie ) )	//f3 U3 f1
			//TODO: optimize these moves instead of moving cubie to top layer:
			//if( cube[wc][0] == 'R' ) {
			//	domoves( "F2 L1 F1 L3 U2 F1" );
			//} else {
			//	domoves( "F1 R3 F3 R2 U1 R3" );
			//}
		//find_state( findbitmappos, findbitmaprot );
		//continue;
		//turn top layer till cubie is above its correct pos
		while( cubiepos[cubie] + 6 - cubie )
			multimove( I, 0 )	//U1
		if( !cubierot[cubie] )
			multimove( coJma, j )	//f3 l3 U2 l1 f1
		else if( cubierot[cubie] < 2 )
			multimove( cKa, j )	//f3 U3 f1
		else
			multimove( eIg, j )	//r1 U1 r3
	}
	//cout << "***** Stage 2 completed *****" << endl; print();
	//working corner is DRF



	//step 3
	//for( j = 1; j < 5; ++j ) {
	forjfrom0to3
		int jplus1 = j+1;
		multimove( M, 0 )	//D1
		cubie = "WUCW"[j] - 64;	// {23,21,3} == BR,BL,FL  ,BR (to do extra D1)
		//find_state( findbitmappos, findbitmaprot );
		if( iscorrect )
			continue;
		//in middle layer? then move it to top layer
		//TODO: optimize this for shorter moves
		if( layer & 1 ) {
			int r = getside( cubie );
			domove( 3, r - jplus1 & 3 );
			multimove( KcIa, r )	//U3 f3 U1 f1
			domove( 3, jplus1 - r & 3 );
		}
loop:		//in top layer now
		if( cubierot[cubie] - j % 2 && cubiepos[cubie] == "SIA"[j] - 64 )	// == astarts[j*2] )	//{11,19,9,1}
			multimove( IeKg, jplus1 )	//U1 r1 U3 r3
		else if( cubierot[cubie] - jplus1 % 2 && cubiepos[cubie] == "KSI"[j] - 64 )	//== astarts[j*2+1] )	//{1,11,19,9}
			multimove( KcIa, jplus1 )	//U3 f3 U1 f1
		else {
			multimove( I, 0 )	//U1
			goto loop;
		}
	}
	//multimove( M, 0 )	//D1 (turn bottom layer, aligning working corner with next side edge)
	//cout << "***** Stage 3 completed *****" << endl; print();



	//step 4
	//first fix working corner (DRF == pos 8)
	cubie = 8;
	if( !iscorrect )
		//in bottom layer?
		if( layer > 1 )
			//multimove( CKA, 0 )	//F3 U3 F1	(moves to top layer)
			if( cubierot[8] > 1 )
				multimove( BQASJA, 0 )	//F2 L1 F1 L3 U2 F1
			else
				multimove( AWCVIW, 0 )	//F1 R3 F3 R2 U1 R3
		else {
			//move to correct corner
			while( cubiepos[8] - 2 )	// != wc-6
				multimove( I, 0 )	//U1
			if( !cubierot[8] )
				multimove( CSJQA, 0 )	//F3 L3 U2 L1 F1
			else if( cubierot[8] < 2 )
				multimove( CKA, 0 )	//F3 U3 F1
			else
				multimove( UIW, 0 )	//R1 U1 R3
		}
	//now take care of working edge (pos 5)
	cubie = 5;
	if( !iscorrect )
		//in correct pos, but flipped?
		if( layer & 1 )	//middle layer
			multimove( BJWBUJAKA, 0 )	//F2 U2 R3 F2 R1 U2 F1 U3 F1
		else 
			while(1) {
				if( cubiepos[5] < 2 && cubierot[5] ) {	//cubiepos[5] == 1
					multimove( AJQASJC, 0 )	//F1 U2 L1 F1 L3 U2 F3
					break;
				} else if( cubiepos[5] == 11 && !cubierot[5] ) {
					multimove( AJQCSJC, 0 )	//F1 U2 L1 F3 L3 U2 F3
					break;
				}
				multimove( I, 0 )	//U1
			}
	//cout << "***** Stage 4 completed *****" << endl; print();



	//step 5
	int	aedges[] = {1,11,19,9,1},	//UF,UR,UB,UL,UF
		wrong[4],
		wrongcnt = 0;

	//for( j = 0; j < 4; ++j )
	forjfrom0to3
		if( cubierot[poscubie[aedges[j]]] )
			wrong[wrongcnt++] = j;
	}

	if( wrongcnt > 3 )
		multimove( QCSAJAKUKWC, 0 )	//L1 F3 L3 F1 U2 F1 U3 R1 U3 R3 F3
	else if( wrongcnt )
		if( *wrong + wrong[1] + 1 & 1 )
			multimove( aeIgKc, *wrong )	//f1 r1 U1 r3 U3 f3
		else
			multimove( aIeKgc, wrong[*wrong == ( wrong[1] + 1 ) % 4] )	//f1 U1 r1 U3 r3 f3

	while( cubiepos[19] - 19 )	//make UB correct
		multimove( I, 0 )	//U1

	//swap edges
	//for( j = 0; j < 4; ++j )// {
	forjfrom0to3
		////findbitmappos |= findbitmaprot |= 1 << aedges[j];
		////find_state( findbitmappos, findbitmaprot );
		////continue;
		//swap opposing?
		cubie = aedges[j];
		int	nextcubie = aedges[j+1],
			cond = poscubie[nextcubie] == cubie;
		if( poscubie[cubie] == nextcubie )		//if( poscubie[aedges[j]] == aedges[j+1 & 3] )
			if( cond )				//if( poscubie[aedges[j+1 & 3]] == aedges[j] )
				multimove( IoJmIoIm, j )	//U1 l3 U2 l1 U1 l3 U1 l1
                        else					// if( poscubie[aedges[j]] == aedges[j+1 & 3] )	// && poscubie[aedges[j+3 & 3]] == aedges[j]  ?
				multimove( gJeIgIe, j )		//r3 U2 r1 U1 r3 U1 r1
		else if( cond )					//if( poscubie[aedges[j+1 & 3]] == aedges[j] )	// && poscubie[aedges[j]] != aedges[j+1 & 3]
			multimove( gKeKgJe, j )			//r3 U3 r1 U3 r3 U2 r1
		else if( poscubie[cubie] == aedges[j+2&3] )	//if( poscubie[aedges[j]] == aedges[j+2 & 3] )
			multimove( gJeIgIeKgJeIgIe, j )		//r3 U2 r1 U1 r3 U1 r1 U3 r3 U2 r1 U1 r3 U1 r1
	}
	//cout << "***** Stage 5 completed *****" << endl; print();



	//step 6
	//poscubie[0],pc[2],pc[20],pc[18],move1,move2(0==empty)   repeat for 11 patterns
	char *	combs = "BT@R10@RBT13R@TB14T@BR20RB@T23@TRB24B@RT30RTB@34TBR@31TR@B40BRT@42",
	        *combmoves[] = { "", "ImKgIoKe", "gImKeIoK", "aeIgKeIgKeIgKc", "fnMfnJfnMfnJ" };
	//<empty>,  ULU'R'UL'U'R,  R'ULU'RUL'U',  FRUR'U'RU R'U'RUR'U'F',  R''L''DR''L''U'' R''L''DR''L''U''
	//for( j = 0; j < 4; ++j )
	forjfrom0to3
		//findbitmappos |= findbitmaprot |= 1 << pos[j];
		//find_state( findbitmappos, findbitmaprot );
		for( int k = 0; k < 66; k+=6 ) {
			int	i = 0,
				doit = k - 54 || j - 2;	//don't do cross when we've just turned U twice...
			for( ; i < 4; ++i )
				doit &= cubiepos[combs[k+(i+j)%4]-64] == "@BTR"[i] - 64;
			if( doit ) {	
				for( i = 4; i < 6; ++i )
					multimove_literal( combmoves[combs[k+i]&7], j );
				break;
			}
		}
	}
	//cout << "***** Stage 6 completed *****" << endl; print();



	//step 7
	//for( j = 0; j < 4; ++j ) {
	forjfrom0to3
		int	cubie = "BTR@"[j] - 64,	// {2,20,18,0} == UFR,URB,UBL,ULF
			dir = cubierot[cubie]-1,
			lastwrong = j;
		//findbitmappos |= findbitmaprot |= 1 << cubie;
		//find_state( findbitmappos, findbitmaprot );
		//continue;
		if( iscorrect )
			continue;
		if( cubierot[cubie] < 2 )
			multimove( aNcgNe, j )	//f1 D2 f3 r3 D2 r1
		else
			multimove( gNeaNc, j )	//r3 D2 r1 f1 D2 f3
		for( int k = 1; k < 4; ++k ) {
			multimove( I, 0 )	//U1
			if( cubierot[poscubie[cubie]] )
				lastwrong = k;
			if( k > 2 || 2 - dir == cubierot[poscubie[cubie]] ) {
				domove( 2, 4 - k + lastwrong );
				if( dir )
					multimove( aNcgNe, j )	//f1 D2 f3 r3 D2 r1
				else
					multimove( gNeaNc, j )	//r3 D2 r1 f1 D2 f3
				domove( 2, 4 - lastwrong );
				break;
			}
		}
	}
	//cout << "***** Stage 7 completed *****" << endl; print();


	//merge moves and output real ones
	int movecnt = 0;
	for( j = 0; j < basemovecnt; j+=2 ) {
		if( j + 2 < basemovecnt && moves[j+1] == moves[j+3] )
			moves[j+2] += moves[j];
		else if( moves[j] % 4 ) {
			if( movecnt++ )
				cout << " ";
			cout << "FBUDLR"[moves[j+1]] << moves[j] % 4;
			//if( movecnt % 26 == 0 ) cout << endl;	//TEST*************
		}
	}
}
