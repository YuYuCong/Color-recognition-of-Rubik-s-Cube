// Charles Tsai

#include <stdio.h>
#include <string.h>


int doedge(char *);
int docorner(char *);
void alg(long,int,int);
void turn(int,int,int);
void rotate(void);


int edges[12], corners[8],  		// store positions of edges and corners

	edgemoves[6][8] = {				// how the edges move
		4,8,16,15, 0,12,20,11,
		5,9,17,12, 1,13,21,8,
		6,10,18,13, 2,14,22,9,
		7,11,19,14, 3,15,23,10,
		0,3,2,1, 4,7,6,5,
		20,21,22,23, 16,17,18,19},

	cornermoves[6][12] = {			// how the corners move
		4,12,19,11, 0,16,23,7, 8,20,15,3,
		8,5,13,16, 4,1,17,20, 0,9,21,12,
		9,6,14,17, 5,2,18,21, 1,10,22,13,
		7,15,18,10, 3,19,22,6, 11,23,14,2,
		0,3,2,1, 8,11,10,9, 4,7,6,5,
		20,21,22,23, 16,17,18,19, 12,13,14,15},

	alg_d_edge[] = {				// algs for doing DF edge
		2,13,14,15,15,1469,15,4399,
		6628,6325,6022,3,1,6590,6287,5984,
		3,4,7,12,0,5,8,16},
	
	alg_d_corner[] = {				// algs for doing DRF corner
		15,630,649,2436,649,13,2436,2455,
		2417,611,630,15,630,1735,2821,3907,
		2436,3503,4589,1331,0,3503,4608,3907};
			
long
	alg_ml_edge[] = {			// algs for doing FR edge
		14,15,315075673,13,13,14,15,84683631,
		0,718005323,747438,621183776,317565452,749707,80383209,357658,
		0,0,0,0,0,0,0,0,0,6,0,1,46659,0,3011,0},
	
	alg_u_corner[] = {			// algs for doing URF corner
		0,520787317,355292103,15,
		174863803,287156624,714877863,9429036,
		765014867,681408008,563752557,688849988,
		0,1162,157,0,6727259,92362,7,0,1891436,97357,5537,0},
	
	alg_u_edge[] = {			// algs for doing UF edge
		0,159184285,159184323,574760661,
		869770682,541341584,72694099,253334306,
		0,108,110,53,5652484,131,4393,207};
	
char letters[] = "FRBLUD";

// ------------------------------------

int doedge(char *cubie) {
	char *edgelist[] = {
		"UF","UR","UB","UL","FR","RB","BL","LF","DF","DR","DB","DL",
		"FU","RU","BU","LU","RF","BR","LB","FL","FD","RD","BD","LD"};
	int i = 0;
	
	while(strcmp(cubie,edgelist[i]))
		i++;
	return i;
}

// ------------------------------------

int docorner(char *cubie) {
	char *cornerslist[] = {
		"UFR","URB","UBL","ULF","DRF","DBR","DLB","DFL",
		"RUF","BUR","LUB","FUL","FDR","RDB","BDL","LDF",
		"FRU","RBU","BLU","LFU","RFD","BRD","LBD","FLD"};
	int i = 0;
	
	while(strcmp(cubie,cornerslist[i]))
		i++;
	return i;
}

// ------------------------------------

void turn(int move, int rotatecount, int print) {
	int z, k, num = (move-1)%3+1, face = (move-1)/3;
	
	for(z=0; z<12; z++) {
		k = 0;
		while(k<8 && edgemoves[face][k] != edges[z])
			k++;
		if(k<8)
			edges[z] = edgemoves[face][(k+num)%4+k/4*4];
	}	
	for(z=0; z<8; z++) {
		k = 0;
		while(k<12 && cornermoves[face][k] != corners[z])
			k++;
		if(k<12)
			corners[z] = cornermoves[face][(k+num)%4+k/4*4];
	}
				
	if(face<4)
		face = (face + rotatecount)%4;

	if(print)
		printf("%c%d ",letters[face],num);

}

// ------------------------------------

void alg(long moves, int rotatecount, int print) {
	while(moves) {
		turn(moves%19, rotatecount, print);
		moves /= 19;
	}
}

// ------------------------------------

void rotate(void) {
	int i;
	
	alg(355,0,0);			// U D'
	
	for(i=0; i<12; i++)		// ... and the middle layer edges
		if(edges[i]>7 && edges[i]<16)
			edges[i] = (edges[i]+3)%4 + edges[i]/4*4;
}

// ------------------------------------


int main(int argc, char *argv[]) {
	int i, temp,
		edgenums[2][12] = {
			0,1,2,3,20,21,22,23,8,15,13,10,
			4,5,6,7,16,17,18,19,12,11,9,14},
		cornernums[3][8] = {
			0,1,2,3,20,23,22,21,
			4,5,6,7,16,19,18,17,
			8,9,10,11,12,15,14,13};
	
	
	// set up arrays: edges & corners
	for(i=0;i<12;i++) {
		temp = doedge(argv[i+1]);
		edges[temp%12] = edgenums[temp/12][i];
	}
	for(i=0;i<8;i++) {
		temp = docorner(argv[i+13]);
		corners[temp%8] = cornernums[temp/8][i];
	}
	
	
	// do D edges
	for(i=0; i<4; i++) {
		while(edges[8+i] != 20)
			alg(alg_d_edge[edges[8+i]], i, 1);
		rotate();
	}
//	putchar('\n');
		
	// do D corners
	for(i=0; i<4; i++) {
		while(corners[4+i] != 20)
			alg(alg_d_corner[corners[4+i]], i, 1);			
		rotate();
	}
//	putchar('\n');

	// do middle layer edges
	for(i=0; i<4; i++) {
		while((temp=edges[4+i]) != 8) {
			alg(alg_ml_edge[temp], i, 1);
			alg(alg_ml_edge[temp+16],i,1);
		}
		rotate();
//		putchar('\n');
	}
	
	// do U corners
	for(i=0; i<4; i++) {
		temp = corners[i];
		alg(alg_u_corner[temp], i, 1);
		alg(alg_u_corner[temp+12], i, 1);
		rotate();
//		putchar('\n');
	}	

	// do U edges
	for(i=0; i<4; i++) {
		temp = edges[i];
		alg(alg_u_edge[temp], i, 1);
		alg(alg_u_edge[temp+8], i, 1);
		rotate();
//		putchar('\n');
	}
	
}
