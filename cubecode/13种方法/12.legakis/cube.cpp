
// Short Cube Program Contest
// Justin Legakis
// legakis@alum.mit.edu

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int FaceCharToIndex(char ch) {
	for (int i=0; i<6; i++) if (ch == "UDFBLR"[i]) return i;
}

void SetArray(int *a, char *v) { while (strlen(v)) *a++ = *v++ - 'A'; }


int remap_X[40], remap_nY[40], remap_Z[40], remap_null[40],
	remaps[9][40], orients[24][6];

void Compose(int *rA, int *rB, int times=1) {

	int i, a, tmp[40];
	while (times--) {
		for (i=0; i<40; i++)
			a = rB[i]*2 + (i>23)*24,
			tmp[i++] = rA[a],
			tmp[i] = (rB[i] + rA[a+1]) % (2+(i>23));
		for (i=0; i<40; i++) rA[i] = tmp[i];
	}
}

vector<string> SS_moves[288];
int SS_remap[288][40], SS_invert[288][24];


void ComposeOrientations(int A, int C, int B=1) {

	for (int i=0; i<6; i++)
		orients[A][i] = orients[C][orients[B][i]];
}

void ThreeZ(int A) {

	ComposeOrientations(A+1, A); // 5: R B (-y+z)
	ComposeOrientations(A+2, A+1); // 6: R D (-y+z+z)
	ComposeOrientations(A+3, A+2); // 7: R F (-y+z+z+z)
}


char* corner_strs[8] =
	{ "UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR" };

char* edge_strs[12] =
	{ "UF", "UR", "UB", "UL", "DR", "DB", "DF", "DL", "FR", "FL", "BR", "BL" };


int StringToKey(int n, char *str) {

	int ret=0, i=0, j, r, m, ok;

	switch (n) {
		case 0:
			for (; i<8; i++)
				for (j=0; j<8; j++)
					for (r=0; r<3; r++) {
						ok = 1;
						for (m=0; m<3; m++)
							if (str[i*4+m+36] != corner_strs[j][(m+r)%3]) ok=0;
						if (ok) ret = ret | (j << (i*3));
					}
			break;
		case 1:
			for (; i<8; i++)
				for (r=0; r<3; r++)
					if (str[i*4+36] == corner_strs[i][r])
						ret = ret | (r << (i*2));
			break;
		case 2:
			for (; i<12; i++)
				for (j=0; j<4; j++)
					for (r=0; r<2; r++) {
						ok = 1;
						for (m=0; m<2; m++)
							if (str[i*3+m] != edge_strs[j][(m+r)%2]) ok = 0;
						if (ok) ret = ret | (i << (j*4));
					}
			break;
		case 3:
			for (; i<12; i++)
				for (j=4; j<12; j++)
					for (r=0; r<2; r++) {
						ok = 1;
						for (m=0; m<2; m++)
							if (str[i*3+m] != edge_strs[j][(m+r)%2]) ok = 0;
						if (ok) ret = ret | ((i-4) << ((j-4)*3));
					}
			break;
		case 4:
			for (; i<12; i++)
				ret = ret | ((str[i*3] != edge_strs[i][0]) << i);
			break;
	}

	return ret;
}


int MoveEncoded(int n, int src, int moveint) {

	int dst=0,
		*remap = SS_remap[moveint],
		*invert_e = SS_invert[moveint],
		i=0;

	switch (n) {
		case 0:
			while (i<8)
				dst |= (((src >> (remap[(i+12)*2]*3)) & 7) << (3*i++));
			break;
		case 1:
			while (i<8)
				dst |= (((remap[(i+12)*2+1] + ((src>>(2*i))&3))%3)<<(2*i++));
			break;
		case 2:
			while (i<4)
				dst |= (invert_e[((src >> (i*4)) & 15)*2] << (4*i++));
			break;
		case 3:
			for (; i<8; i++)
				dst |= ((invert_e[(((src >> (3*i))&7)+4)*2] - 4) << (3*i));
			break;
		case 4:
			for (; i<12; i++)
				dst |= ((((src>>i)&1) != remap[i*2+1]) << i);
	}

	return dst;
}


int solve_array[1024*1024*16];
int work_array[1024*1024*16];
int work_list[2][1024*1024];


			
int main(int argc, char *argv[]) {

	int i, j, p, *remap,
		ormap0[24],
		ormap1[8] = { 0, 4, 8, 12, 16, 17, 18, 19 },
		ormap2[6] = { 0, 1, 2, 3, 16, 20 },
		moves[5][9],
		moveslen[5] = { 6, 4, 8, 4, 6 },
		*ormaps[5] = { ormap2, ormap0, ormap0, ormap1, ormap0 },
		orns[5] = { 6, 24, 24, 8, 24 },
		S;


	for (i=0; i<1024*1024*16; i++)
		work_array[i] = 0;

	SetArray(remaps[0], "BACADAAAEAFAGAHAIAJAKALABACADAAAEAFAGAHA");
	SetArray(remap_X, "CBKAGBLAABIAEBJABADAFAHABCHBGCCBABDCFBEC");
	SetArray(remap_null, "AABACADAEAFAGAHAIAJAKALAAABACADAEAFAGAHA");
	SetArray(remap_nY, "BACADAAAFAGAHAEAKBIBLBJBBACADAAAHAEAFAGA");
	SetArray(moves[0], "ABBAJJ");
	SetArray(moves[1], "CDDC");
	SetArray(moves[2], "EFFEGHHG");
	SetArray(moves[3], "EFFE");
	SetArray(moves[4], "IIKKLL");

	for (i=0; i<40; i++)
		for (int j=1; j<6; j++)
			remap_Z[i] = remaps[j][i] = remap_null[i];

	// remap_Z:
	Compose(remap_Z, remap_X);
	Compose(remap_Z, remap_nY);
	Compose(remap_Z, remap_X, 3);

	// remap_D:
	Compose(remaps[1], remap_X, 2);
	Compose(remaps[1], remaps[0]);
	Compose(remaps[1], remap_X, 2);

	// remap_R:
	Compose(remaps[5], remap_Z);
	Compose(remaps[5], remaps[0]);
	Compose(remaps[5], remap_Z, 3);

	// remap_L:
	Compose(remaps[4], remap_Z, 3);
	Compose(remaps[4], remaps[0]);
	Compose(remaps[4], remap_Z);

	// remap_F:
	Compose(remaps[2], remap_X, 3);
	Compose(remaps[2], remaps[0]);
	Compose(remaps[2], remap_X);

	// remap_B:
	Compose(remaps[3], remap_X, 3);
	Compose(remaps[3], remaps[1]);
	Compose(remaps[3], remap_X);


	SetArray(orients[0], "ABCDEF"); // 0: F U
	SetArray(orients[1], "FECDAB"); // 1: F R (+z)
	ComposeOrientations(2, 1); // 2: F D (+z+z)
	ComposeOrientations(3, 2); // 3: F L (+z+z+z)

	SetArray(orients[4], "ABFECD"); // 4: R U (-y)
	ThreeZ(4);

	ComposeOrientations(8, 4, 4);   // 8:  B U (-y-y)
	ThreeZ(8);

	ComposeOrientations(12, 8, 4);  // 12: L U (-y-y-y)
	ThreeZ(12);

	SetArray(orients[16], "DCABEF"); // 16: U B (+x)
	ThreeZ(16);

	ComposeOrientations(20, 13, 4); // 20: D F (-y-y-y+z-y)
	ThreeZ(20);


	char* moveseq[] = {
		"U+",
		"U-",
		"R-D-R+D-R-D2R+D2", // rot3 fw
		"D2R-D2R+D+R-D+R+", // rot3 bw
		"R+L-F+R-L+D2R+L-F+R-L+", // cycle3 fw
		"R+L-F-R-L+D2R+L-F-R-L+", // cycle3 bw
		"F+U+D-L-U-D+", // top edges fw
		"D-U+L+D+U-F-", // top edges bw
		"R+L-F+R-L+D-R+L-F-R-L+D-R+L-F2R-L+F-B+R+F+B-D2F-B+R+F+B-", //flip2
		"U2",
		"R+F+R-L+D-R+L-F-R-L+D-R+L-F2R-L+F-B+R+F+B-D2F-B+R+F+B-L-", //flip2b
		"F+R+F+R-L+D-R+L-F-R-L+D-R+L-F2R-L+F-B+R+F+B-D2F-B+R+F+B-L-F-", //flip2c
	}, *src, dst[99];


	for (p=0; p<288; p++) {
		string m = moveseq[p/24];
		remap = SS_remap[p];
		vector<string> &mo = SS_moves[p];
		for (i=0; i<m.size(); i+=2) {
			string m2 = m.substr(i, 2);
			m2[0] = "UDFBLR"[orients[p%24][FaceCharToIndex(m2[0])]];
			mo.push_back(m2);
		}
		for (i=0; i<40; i++) remap[i] = remap_null[i];
		for (j=0; j<mo.size(); j++)
			for (i=0;
				 i<(mo[j][1]=='+' ? 1 : (mo[j][1]=='-')+2);
				 i++)
				Compose(remap, remaps[FaceCharToIndex(mo[j][0])]);
		for (i=0; i<24; i+=2)
			SS_invert[p][remap[i]*2] = i/2,
			SS_invert[p][remap[i]*2+1] = remap[i+1];
	}


	for (i=0; i<24; i++) ormap0[i] = i;


	for (int n=0; n<5; n++) {

		int work_list_pos[2] = { 1, 0 }, wl = 0, idst,
			count_work, count_work_total = 0,
			key = StringToKey(n,
		"UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR");

		solve_array[work_list[0][0] = key] = -1;
		work_array[key] = (n+1);

		do {
			for (count_work=p=0; p<work_list_pos[wl]; p++)
				for (count_work++, j=0; j<moveslen[n]; j+=2)
					for (i=0; i<orns[n]; i++)
						if (work_array[
									idst = MoveEncoded(
											n, work_list[wl][p],
											24*moves[n][j+1]+ormaps[n][i])
								] != (n+1)) {
							solve_array[idst] = 24*(moves[n][j])+ormaps[n][i],
							work_array[idst] = n+1;
							work_list[!wl][work_list_pos[!wl]++] = idst;
						}

			work_list_pos[wl] = 0;
			wl = !wl;

			count_work_total += count_work;

		} while (count_work);


		while (solve_array[S = StringToKey(n, src=argv[1])] > -1) {
			remap=SS_remap[solve_array[S]];
			for (i=0; i<12; i++)
				p = remap[i*2]*3,
				j = remap[i*2+1],
				dst[i*3] = src[p+j],
				dst[i*3+1] = src[p+!j];
			for (i=0; i<8; i++)
				for (j=0; j<3; j++)
					dst[i*4+36+j] =
						src[remap[(i+12)*2]*4+36+((remap[(i+12)*2+1]+j)%3)];
			for (i=0; i<strlen(src); i++)
				src[i] = (src[i] == ' ') ? ' ' : dst[i];
			for (i=0; i<SS_moves[solve_array[S]].size(); i++)
				cout << SS_moves[solve_array[S]][i];
		}
	}
}
