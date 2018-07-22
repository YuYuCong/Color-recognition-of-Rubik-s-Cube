/*==============================================================================
	COMMON PART
==============================================================================*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define FOR(a,b) for(a=0;a<b;a++)
#define TAB [256]

char seq TAB TAB, output TAB = "", *s, t TAB;
int  nbr, c, C, i, j, k, count=0, name=0;

typedef struct {
	char color TAB;
	int g, h;
} Cube;

Cube cube;


/*==============================================================================
	ROTATIONAL ENGINE
==============================================================================*/

int rotColor TAB;

#define LI(a)     cube->color[rotColor[20*k+4*j+a]]
#define MV(a,b)   LI(a)=LI(b),

void apply(Cube *cube, char *sequence) {
	int i, j, k, len = strlen(sequence);
	FOR(i,len) {
		k = toupper(sequence[i])-64;
		k = k<9?k/2-1:(k+9)/6;
		if (k>=0)
			FOR(j,5)
				if (sequence[i]<91)
					c = LI(0), MV(0,1) MV(1,2) MV(2,3) LI(3) = c;
				else
					c = LI(3), MV(3,2) MV(2,1) MV(1,0) LI(0) = c;
	}
}


/*==============================================================================
	SEARCH ENGINE
==============================================================================*/

int (*dfHeur)(Cube *), pathLen, path[99], zLim, zMax;

int DepthFirst(Cube *cube) {
	int i, f;
	Cube cube2;

	f = cube->g + cube->h;
	if (f > zLim)
		zMax = f<zMax?f:zMax;
	else if (!cube->h)
		return 1;
	else
		FOR(i,nbr) {
			path[pathLen++] = i;
			apply( memcpy(&cube2, cube, sizeof(Cube)), seq[i] );
			cube2.g++;
			cube2.h = dfHeur(&cube2);
			if (DepthFirst(&cube2))
				return 1;
			pathLen--;
		}
	return 0;
}

void IDAstar(int max) {
	cube.h = zLim = dfHeur(&cube);
	while (1) {
		zMax = 99;
		pathLen = 0;
		if (DepthFirst(&cube)) {
			FOR(i,pathLen)
				strcat(output, s=seq[path[i]]),
				apply(&cube,s);
			return;
		}
		if (zLim == zMax || zMax > max) return;
		zLim = zMax;
	}
}


/*==============================================================================
	ALGORITHMS
==============================================================================*/

void replicate(char *map, int n) {
	n=nbr / n;
	FOR(i,n) {
		for(j=0;c=seq[i][j];j++)
			FOR(k,6)
				if (toupper(c)=="UDLRFB"[k])
					t[j] = c<91 ? map[k] : tolower(map[k]);
		t[j]=0;
		strcpy(seq[nbr++], t);
	}
}

int comp(const void *a, const void *b) {
	return strlen((char *)a) - strlen((char *)b);
}

#define APPEND(a)      s = strtok( strcpy(t,a), " " ); while (s) strcpy( seq[nbr++], s ), s = strtok(0," ");
#define INITSET(a,b)   memset(&seq,nbr=0,sizeof(seq)); APPEND(a) replicate("UDRLBF",1); replicate("UDFBRL",1); APPEND(b) SORT
#define MULTIPLY(a,b)  INITSET(a,b) replicate("FBLRDU",1); replicate("RLUDFB",2); replicate("DUFBLR",1); SORT
#define SORT           qsort(seq,nbr,256,comp);
#define HEURISTIC(a,b) int a(Cube *cube) { int i, sum=0; b return sum; }
#define CHECK(a)       (cube->color[i+a]-"UFURUBULDFDRDBDLFRFLBRBLUFRURBUBLULFDRFDFLDLBDBR"[i+a])
#define CHECK01        CHECK(0)||CHECK(1)
#define EDGE(a,b)      for(i=a;  i<b;  i+=2) if (CHECK01) sum++;
#define CORNER         for(i=24; i<48; i+=3) if (CHECK01||CHECK(2)) sum++;
#define TEST(a)        if (sum) return a;

HEURISTIC(hAll,       EDGE(0,24) CORNER TEST(sum-16?sum-13?sum-8?3:1:2:2) )
HEURISTIC(hBackCross, EDGE(0,8) )
HEURISTIC(hLayer23,   EDGE(8,24) TEST(sum>5?5:sum>2?sum-2:1) )
HEURISTIC(hCorners,   CORNER TEST(sum>6?5:sum-1) )


/*==============================================================================
	MAIN
==============================================================================*/

int main(int argc, char **argv) {
	FOR(i,120) rotColor[i]="^om`FUNW]nl_VMXEpka\\hkneIOMKofilNLJPmpgjdigZBSJQchfYATIRbje[aljcHXPTkib`OSGWhd_m[fp]RLVD\\ZeoCQKUYgn^ad[^HBDFcZ]`_bY\\EGAC"[i]-65;

	memset(&cube,0,sizeof(Cube));
	while (--argc) strcat(cube.color, *++argv);

	dfHeur = hAll;
	MULTIPLY("", "d D DD")
	IDAstar(5);

	dfHeur = hBackCross;
	IDAstar(9);

	dfHeur = hLayer23;
	INITSET("rdR rDR rDDR FDDf FDf Fdf RfrF fRFr", "d D DD")
	IDAstar(9);

	dfHeur = hCorners;
	MULTIPLY("BLbRBlbr rULuRUlu RBLbrBlb ULurUluR RRDrUURdrUUr BBRRblBRRbLb", "")
	IDAstar(9);

	FOR(i,strlen(output)+1) {
		C = toupper(c = output[i]);
		if (C - name) {
			if (count%=4)
				printf("%c%c", name, count==1?43:count-2?45:50);
			name = C;
			count = 0;
		}
		count += c>90?3:1;
	}
}
