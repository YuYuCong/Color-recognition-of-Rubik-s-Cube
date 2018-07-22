// Author: Grant Tregay

#include <stdio.h>		// Standard input/output
#include <memory.h>		// memset
#include <string.h>		// strcpy

enum Moves { U = 0, U2, U3, D, D2, D3, F, F2, F3, B, B2, B3, R, R2, R3, L, L2, L3, X, X2, X3, Y, Y2, Y3, Z, Z2, Z3};

char	g_acSolvedState[49] = "UFURUBULDFDRDBDLFRFLBRBLUFRURBUBLULFDRFDFLDLBDBR"; // State of a solved cube
char	g_acState[49];			// Current cube state
char	g_acTestState[49];		// Current test cube state - Moves applied to a test cube, not yet applied to solution
Moves	g_aMoveSolution[2000];	// Current solution
int		g_iSolutionLen(0);		// Current solution length
int		g_iTestSolutionLen(0);	// Current test state length

// See if the facelet matches the correct value
bool Match(int iStateFacelet, int iSolvedFacelet)
{
	return g_acTestState[iStateFacelet] == g_acSolvedState[iSolvedFacelet];
}

// See if a cubelet is place (not worrying about orientation)
bool Placed(int iFacelet1, int iFacelet2, int iFacelet3=-1)
{
	if (iFacelet3 == -1) iFacelet3 = iFacelet1;
	return ((Match(iFacelet1, iFacelet1) || Match(iFacelet1, iFacelet2) || Match(iFacelet1, iFacelet3)) &&
			(Match(iFacelet2, iFacelet1) || Match(iFacelet2, iFacelet2) || Match(iFacelet2, iFacelet3)) &&
			(Match(iFacelet3, iFacelet1) || Match(iFacelet3, iFacelet2) || Match(iFacelet3, iFacelet3)));
}

// See if the cubelet is solved (placed and oriented)
bool Solved(int iFacelet1, int iFacelet2, int iFacelet3=-1)
{
	if (iFacelet3 == -1) iFacelet3 = iFacelet1;
	return (Match(iFacelet1, iFacelet1) && Match(iFacelet2, iFacelet2) && Match(iFacelet3, iFacelet3));
}

// Cycle a series of four facelets to turn a face or change cube orientation
void CycleFacelets(int iSrc, int iDest1, int iDest2, int iDest3)
{
	char cTemp = g_acTestState[iDest3];
	g_acTestState[iDest3] = g_acTestState[iDest2];
	g_acTestState[iDest2] = g_acTestState[iDest1];
	g_acTestState[iDest1] = g_acTestState[iSrc];
	g_acTestState[iSrc] = cTemp;
}

// Used in cube rotations
void CycleFaces(char cSrc, char cDest1, char cDest2, char cDest3)
{
	char * pcTemp = g_acTestState-1;
	while (*++pcTemp)
	{
		if (cSrc == *pcTemp) *pcTemp = cDest1;
		else if (cDest1 == *pcTemp) *pcTemp = cDest2;
		else if (cDest2 == *pcTemp) *pcTemp = cDest3;
		else if (cDest3 == *pcTemp) *pcTemp = cSrc;
	}
}

// Cycles four sets of four facelets to turn a face, or 2 sets of 4, plus two face rotations for a cube rotation
void DoMove(Moves MoveCur)
{
	switch (MoveCur)
	{
		default : break;
		case U :
			CycleFacelets( 0,  6,  4,  2);
			CycleFacelets( 1,  7,  5,  3);
			CycleFacelets(24, 33, 30, 27);
			CycleFacelets(25, 34, 31, 28);
			CycleFacelets(26, 35, 32, 29);
			break;
		case D :
			CycleFacelets( 8, 10, 12, 14);
			CycleFacelets( 9, 11, 13, 15);
			CycleFacelets(39, 36, 45, 42);
			CycleFacelets(40, 37, 46, 43);
			CycleFacelets(41, 38, 47, 44);
			break;
		case F :
			CycleFacelets( 1, 16,  9, 18);
			CycleFacelets( 0, 17,  8, 19);
			CycleFacelets(35, 25, 38, 40);
			CycleFacelets(33, 26, 36, 41);
			CycleFacelets(34, 24, 37, 39);
			break;
		case B :
			CycleFacelets( 5, 22, 13, 20);
			CycleFacelets( 4, 23, 12, 21);
			CycleFacelets(29, 31, 44, 46);
			CycleFacelets(27, 32, 42, 47);
			CycleFacelets(28, 30, 43, 45);
			break;
		case R :
			CycleFacelets( 3, 21, 11, 17);
			CycleFacelets( 2, 20, 10, 16);
			CycleFacelets(26, 28, 47, 37);
			CycleFacelets(24, 29, 45, 38);
			CycleFacelets(25, 27, 46, 36);
			break;
		case L :
			CycleFacelets( 7, 19, 15, 23);
			CycleFacelets( 6, 18, 14, 22);
			CycleFacelets(32, 34, 41, 43);
			CycleFacelets(30, 35, 39, 44);
			CycleFacelets(31, 33, 40, 42);
			break;
		case X :
			CycleFaces('F', 'U', 'B', 'D');
			DoMove(R);
			DoMove(L3);
			CycleFacelets( 1,  4, 13,  8);
			CycleFacelets( 0,  5, 12,  9);
			break;
		case Y :
			CycleFaces('F', 'L', 'B', 'R');
			DoMove(U);
			DoMove(D3);
			CycleFacelets(17, 18, 23, 20);
			CycleFacelets(16, 19, 22, 21);
			break;
		case Z :
			CycleFaces('U', 'R', 'D', 'L');
			DoMove(F);
			DoMove(B3);
			CycleFacelets( 6,  3, 10, 15);
			CycleFacelets( 7,  2, 11, 14);
			break;
		// For a triple turn, turn once, then do a double
		case U3 :
		case D3 :
		case F3 :
		case B3 :
		case R3 :
		case L3 :
		case X3 :
		case Y3 :
		case Z3 :
			DoMove(Moves((MoveCur / 3) * 3));
		// For a double turn, do two single turns
		case U2 :
		case D2 :
		case F2 :
		case B2 :
		case R2 :
		case L2 :
		case X2 :
		case Y2 :
		case Z2 :
			DoMove(Moves((MoveCur / 3) * 3));
			DoMove(Moves((MoveCur / 3) * 3));
			break;
	}
}

// Applies a move to the test state
void TestMove(Moves MoveCur)
{
	DoMove(MoveCur);
	g_aMoveSolution[g_iTestSolutionLen++] = MoveCur;
}

// Applies a series of moves to the test state
void TestSequence(Moves *aMoveSequence, int iLen)
{
	for (int i = -1; ++i < iLen;)
		TestMove(aMoveSequence[i]);
}

// Applies the current test cube's state and solution to the actual solution cube
void ApplyTest()
{
	strcpy(g_acState, g_acTestState);
	g_iSolutionLen = g_iTestSolutionLen;
}

// Resets the test cube to the actual solution cube's state and current solution
void ScrapTest()
{
	strcpy(g_acTestState, g_acState);
	g_iTestSolutionLen = g_iSolutionLen;
}

// Applies a move to the solution cube
void ApplyMove(Moves MoveCur)
{
	TestMove(MoveCur);
	ApplyTest();
}

// Applies a series of moves to the solution cube
void ApplySequence(Moves *aMoveSequence, int iLen)
{
	TestSequence(aMoveSequence, iLen);
	ApplyTest();
}

// Converts a move to it's character representation
char FaceOrAxis(Moves moveCur)
{
	switch (moveCur/3)
	{
		default:
		case 0: return 'U';
		case 1: return 'D';
		case 2: return 'F';
		case 3: return 'B';
		case 4: return 'R';
		case 5: return 'L';
	}
}

int main(int argc, char * argv[])
{
	// DISCLAIMER  :-)
	// This solver implements a solution very similar to Mark Jeay's simple
	// solution. It's a great solution - largely intuitive, easy to teach and
	// learn, and not terribly inefficient. Check out his solution page at
	// http://www.jeays.net/rubiks.htm for more information. The main
	// inefficiency of this method (and more so of my implementation of it)
	// is the final step of orienting corners.  This can be done much more
	// efficiently.

	// Clear state and solution buffers
	memset(g_acState, 0, 49);
	memset(g_aMoveSolution, U, 2000);
	int i = -1, j, k, l;

	// Load the cube state we're trying to solve
	for (i = -1; ++i < 24;)
	{
		g_acState[i] = argv[i/2+1][i%2];
		g_acState[i+24] = argv[i/3+13][i%3];
	}
	strcpy(g_acTestState, g_acState);

	// Algs needed to position F2L pieces (All U edges, 3 U corners, and 3 second layer edges)
	Moves aMoveAllMoves[] = { U, U2, U3, D, D2, D3, F, F2, F3, B, B2, B3, R, R2, R3, L, L2, L3, X, X2, X3, Y, Y2, Y3, Z, Z2, Z3};
	Moves aMoveF2LAlgs[3][23][8] =
	{
		{
			// Algs to solve UF (rotate Y and repeat until cross is solved)
			{ U,  L, U3, F},
			{U3},
			{ L, F},
			{ L, U2, L3},
			{ U, R3, U3, F3},
			{R3, F3},
			{R2, D3, F2},
			{ F},
			{ U, L3, U3},
			{L2,  F, L2},
			{ U,  L, U3},
			{R2, F3, R2},
			{U3, R3, U},
			{F3},
			{U3,  R, U},
			{F2},
			{ L, D3, L3, F},
			{ D, F2},
			{L3,  F,  L},
			{D2, F2},
			{ D, L3,  F,  L},
			{D3, F2},
			{ R, F3, R3}
		},
		{
			// Algs to solve UFR (rotate Y and repeat until U is solved, except one corner)
			{ F, D3,  R, F2, R3, F3},
			{ F,  D, B3, D2,  B, F3},
			{ F,  D, F3},
			{R3, D3,  R},
			{ F,  R, F2, R3, F3},
			{R3,  D,  R},
			{ R, U2, L3, U2, R3},
			{D2,  F, D3, F3},
			{ F, D2, F3},
			{R3, D2,  R},
			{D3, R2, F3, R2, F},
			{R2, F3, R2,  F},
			{ F, D3, F3},
			{D3,  F,  D, F3},
			{B3,  D,  B,  F, D3, F3},
			{ R, D2, R2,  D,  R},
			{B3,  F, D3,  B, F3},
			{R2, B2, R2, B2},
			{ F, L3, D2,  L, F3},
			{ B, R3, D2, B3,  R},
			{ L,  D, L3, R3, D3,  R},
			{F3, D2, F2, D3, F3},
			{ L, R3,  D, L3,  R}
		},
		{
			// Algs to solve FR (rotate Y and repeat until F2L is solved, except one C/E pair)
			{R3, D2,  R,  F,  D, F3},
			{ D,  F, D3, F3},
			{ F, D3, F3},
			{ F, D2, F3},
			{ F,  D, F3},
			{ D, R3, D3,  R},
			{R3, D3,  R},
			{R3, D2,  R},
			{R3,  D,  R},
			{F3, U3, R3,  U,  F},
			{R3, F3, R2,  F,  R},
			{L3,  F,  D, L3, F3, L2},
			{U3, B2, D3,  U, R2},
			{ R,  U,  F, U3, R3},
			{R2, D3,  U, F2, U3}
		}
	};
	int aiF2LAlgLengths[3][23] =
	{
		{4, 1, 2, 3, 4, 2, 3, 1, 3, 3, 3, 3, 3, 1, 3, 1, 4, 2, 3, 2, 4, 2, 3},	// UE placement alg lengths
		{6, 6, 3, 3, 5, 3, 5, 4, 3, 3, 5, 4, 3, 4, 6, 5, 5, 4, 5, 5, 6, 5, 5},	// UC placement alg lengths
		{6, 4, 3, 3, 3, 4, 3, 3, 3, 5, 5, 6, 5, 5, 5}							// Second layer edge placement alg lengths
	};
	int aiF2LNumTests[3] = {23, 23, 15};
	int aiF2LCheckFacelets[3][3] =
	{
		{0, 1, 1},		// Check UF for the U edges
		{24, 25, 26},	// Check UFR for the U corners
		{16, 17, 17}	// Check FR for the second layer edges
	};

	// Solve F2L (except one corner/edge pair)
	for (i = -1; ++i < 3;)
		for (j = -1; ++j < 4;) // Four pieces of each type
		{
			if (j || (i-1)) // Skip one second layer edge and first layer corner
			{
				// While the designated pieces remains unsolved...
				if (!Solved(aiF2LCheckFacelets[i][0], aiF2LCheckFacelets[i][1], aiF2LCheckFacelets[i][2]))
				{
					// Test each of the possible algs for solving the piece
					for (k = -1; ++k < aiF2LNumTests[i];)
					{
						TestSequence(aMoveF2LAlgs[i][k], aiF2LAlgLengths[i][k]);
						// If that does it, jump out of the test loop
						if (Solved(aiF2LCheckFacelets[i][0], aiF2LCheckFacelets[i][1], aiF2LCheckFacelets[i][2]))
							break;
						// If not, scrap that test
						ScrapTest();
					}

					// Make the test sequence permanent
					ApplyTest();
				}
			}
			// Rotate the cube
			ApplyMove(Y);
			// Preserve U corners if working on the second layer
			if (i > 1)
				ApplyMove(U3);
		}

	// Orient LL edges
	Moves aMoveOEAlg[2][6] =
	{
		{R3, D3, B3,  D,  B,  R},
		{R3, B3, D3,  B,  D,  R}
	};
	// Find out out many edges are flipped
	for (i = -1, j = 0; ++i < 4;)
		if (g_acTestState[8+2*i] != 'D')
			j ++;
	// If four, flip 2
	if (j > 2)
		ApplySequence(aMoveOEAlg[0], 6);
	// If any (therefore two at this point), flip them
	if (j)
	{
		// Rotate the cube to put a solved edge at DF and a non-solved one at DR
		for (i = 1; g_acTestState[8] != 'D' || g_acTestState[10] == 'D'; i++)
			ApplyMove(Y);
		// Flip DR and DB or DL
		ApplySequence(aMoveOEAlg[g_acTestState[12] != 'D' ? 0 : 1], 6);
		// Reverse the cube rotations
		while (--i)
			ApplyMove(Y3);
	}

	// Rotate D until DF comes into position
	while (!Solved(8, 9))
		ApplyMove(D);

	// Rotate LL edges until DR comes into position
	Moves aMovePELLAlg[] = {R3, D2, R, D, R3, D, R};
	while (!Solved(10, 11))
		ApplySequence(aMovePELLAlg, 7);

	// Swap DL and DB if needed
	if (!Solved(14, 15))
	{
		ApplyMove(D2);
		ApplySequence(aMovePELLAlg, 7);
		ApplyMove(D3);
	}

	// Position remaining corners
	Moves aMovePCAlg[] = {D, L, U2, L3, D3}; // Repeated twice with different D rotations rotates 3 corners (2 LL + 1 FL)
	// Repeat while at least one LL corner isn't placed right
	while (!(Placed(36, 37, 38) && Placed(39, 40, 41) && Placed(42, 43, 44) && Placed(45, 46, 47)))
	{
		// if the last F2L corner is placed
		if (Placed(24, 25, 26))
		{
			// Find a LL corner that isn't, and switch them
			for (i = 33; (i += 3) < 47 && Placed(i, i+1, i+2););
			i = ((i - 36) / 3 + 2) % 4;
			aMovePCAlg[0] = Moves(D + i - 1);
			aMovePCAlg[4] = Moves(D3 - i + 1);
			ApplySequence(&aMovePCAlg[i ? 0 : 1], i ? 5 : 3);
		}
		else // Otherwise, find the LL position where the coner in UFR belongs through trial and error
			for (i = -1; ++i < 4;)
			{
				// Adjust pre/post turn of D (before/after L U2 L3)
				aMovePCAlg[0] = Moves(D + i - 1);
				aMovePCAlg[4] = Moves(D3 - i + 1);
				// Try it out
				TestSequence(&aMovePCAlg[i ? 0 : 1], i ? 5 : 3);
				j = (i + 2) % 4 * 3 + 36;
				// Solidify or scrap test sequence
				if (Placed(j, j+1, j+2)) ApplyTest();
				else ScrapTest();
			}
	}

	// Move potentially misoriented corners to four on U and one in DFL
	ApplyMove(Z2);

	// Orient corners - Orient UFR and UBR, turn UFL as needed, apply Z cube rotation, repeat (four times)
	Moves aMoveOCAlgs[8][13] =
	{															// UFR/UBR
		{F2, U3, R3,  U, R2, D3, F2,  D, F2, R3,  U, F2, U3},	// C/0		0
		{ X, Y2, Y2, X3},										// CC/0		1 Followed by 0
		{ L, B3, D2,  B, L3, U2,  L, B3, D2,  B, L3, U2},		// 0/C		2
		{ F, U3,  B, U2, F2, D3, L2,  D,  U, B3, U3,  F, U3},	// C/C		3
		{ Y, Y3},												// CC/C		4 Followed by 0
		{Y2, Y2},												// 0/CC		5 Followed by 2
		{Z3, Y3,  Y,  Z},										// C/CC		6 Followed by 0
		{R3,  U,  L, D3, U3, F2,  D, R2, U2, L3,  U, R3,  U}	// CC/CC	7
	};
	// Length of previous algs - For 1, 4, 5, & 6 it's half of total before the actual alg, and half to reverse it
	int aiOCAlgLengths[] = { 13, 2, 12, 13, 1, 1, 2, 13};
	for (i = -1; ++i<2; )
	{
		// Calculate twist index based on UFR and UBR
		int iTwist = 0;
		for (j = -1; ++j < 2; )
			for (k = 0; ++k < 3; )
				if (g_acTestState[24+j*3+k] == 'U')
					iTwist += (j * 2 + 1) * k;
		// Fix them, if needed
		if (iTwist)
		{
			// Adjust index to 0 base
			iTwist--;
			// Figure out if it's an alg itself or just a setup to one - store length of both
			int iLenSetup = aiOCAlgLengths[iTwist];
			bool bExtra = iLenSetup < 12;
			int iAlg = bExtra ? (iTwist == 5 ? 2 : 0) : iTwist;
			int iLenAlg = aiOCAlgLengths[iAlg];
			// Do setup (if needed), alg proper, and reverse of setup
			if (bExtra) ApplySequence(aMoveOCAlgs[iTwist], iLenSetup);
			ApplySequence(aMoveOCAlgs[iAlg], iLenAlg);
			if (bExtra) ApplySequence(&aMoveOCAlgs[iTwist][iLenSetup], iLenSetup);
		}
		// Rotate final corners to be fixed onto U
		ApplyMove(Z);
	}

	// Simplify the solution sequence:
	// Remove all cube rotations
	Moves aMoveCubeRotCycles[3][2][4] =
	{
		{// X rotation makes F -> U -> B -> D and Y -> Z' -> Y' -> Z
			{F, U, B, D},
			{Z, Y, Z3, Y3},
		},
		{// Y rotation makes R -> F -> L -> B and X -> Z -> X' -> Z'
			{R, F, L, B},
			{X, Z, X3, Z3},
		},
		{// Z rotation makes L -> U -> R -> D and Y -> X -> Y' -> X'
			{L, U, R, D},
			{Y, X, Y3, X3},
		}
	};

	Moves	tmp_Move;
	int iAxis;
	// Go through the solution converting all moves to original faces, based on cube rotations
	for (i = -1; ++i < g_iSolutionLen;)
	{
		// Store original move value
		tmp_Move = g_aMoveSolution[i];
		if (tmp_Move < X) continue;// Go on if not a cube rotation

		// Store move's axis (x, y, z) and amount of turn (1, 2, 3)
		iAxis = (tmp_Move - X) / 3;
		int iAmt = tmp_Move % 3 + 1;

		// Adjust all following moves the number of times indicated by turn amount
		for (j = iAmt; j--;)
		{
			// Go from the current move to the end
			for (k = i-1; ++k < g_iSolutionLen;)
			{
				// Grab move and determine if it's a face turn or cube rotation
				tmp_Move = g_aMoveSolution[k];
				iAmt = tmp_Move % 3;
				int iSubset = tmp_Move < X ? 0 : 1;

				// Find this move amongst the rotation cycles to see what it becomes
				for (l = -1; ++l < 4;)
				{
					// Match the face (any turn amount), the axis (half turn), or the exact axis turn amount
					if (((tmp_Move / 3 == aMoveCubeRotCycles[iAxis][iSubset][l] / 3) && (iAmt == 1 || !iSubset)) ||
						(tmp_Move == aMoveCubeRotCycles[iAxis][iSubset][l]))
					{
						// Set new face/axis that is turned (amount of turn included for axis quarter turns)
						tmp_Move = aMoveCubeRotCycles[iAxis][iSubset][(l+3)%4];
						// Adjust turn amount for axis half turns or face turns
						if (!iSubset || iAmt == 1)
							tmp_Move = Moves(tmp_Move / 3 * 3 + iAmt);
						break;
					}
				}
				// Set the adjusted face/axis turn value
				g_aMoveSolution[k] = tmp_Move;
			}
		}
	}

	// Simplification, part 2/print solution - remove sequential turns of the same or opposite faces, then print
	j = 0;
	while (j < g_iSolutionLen)
	{
		// Make sure it's a face turn - really only necessary at the beginning
		if (X <= g_aMoveSolution[j])
		{
			j++;
			continue;
		}
		// Snag this move - store axis and turn amount information
		tmp_Move = g_aMoveSolution[j];
		iAxis = tmp_Move / 6;
		int iTurn[2] = {0, 0};
		// Figure out how much turn of this face and the other parallel face
		while (iAxis == tmp_Move / 6)
		{
			// Set turn amount for tmp_Move
			iTurn[tmp_Move/3%2] += tmp_Move % 3 + 1;
			// Go to the next move - skip over any cube rotations
			while (++j < g_iSolutionLen && X <= g_aMoveSolution[j]);
			// Store the next move in tmp_Move
			tmp_Move = g_iSolutionLen <= j ? X : g_aMoveSolution[j];
		}
		// Print out parallel face turns
		for (k = -1; ++k < 2; )
		{
			i = iTurn[k] % 4;
			if (i)
				printf("%c%c", FaceOrAxis(Moves(iAxis*6+3*k)), '1' + i - 1);
		}
	}

	return 0;
}
