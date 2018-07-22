/* Cube solver by Yuri Pertsovski                                        */
/* Based on Mark Jeays' simple solution #1 (http://jeays.net/rubiks.htm) */

/* I used the following (internal) notation:
   for a corner or an edge in the middle layer:
     r - the face on which the right side of the cubie is on
     l - ...left...
   for an edge on the top or bottom layer:
     f - the non U or D face of the edge
     r - the face to the right of f
     l - ...left...
     b - opposite to f
   to the right/left - when looking at it (see f array)
   (I didn't use the normal r,l,f&b for solving so there is no conflict)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 20 /* number of cubies */

#define sides(side,offset) strchr(f,side)[offset]
/* the name (the original position) of the cubie currently being placed.
   it is often in a diffrenet character order than the same string in ref. */
#define cube_ic cube[ic]
#define ref_ic ref[ic] /* the current position of that cubie */
#define refresh ic=find(co);
#define ref_ic0 *ref_ic
#define ref_ic1 ref_ic[1]
#define cube_ic0 *cube_ic
#define cube_ic1 cube_ic[1]
#define check_in_place if (!strcmp(ref_ic,cube_ic)) continue;
#define if_on_bottom if (ref_ic0=='D') /*in the solved cube top & bottom */
#define if_not_on_top if (ref_ic0!='U')/*layer cubies always start with U/D */
/* cubie_sides is evaluated a few times */
#define cubie_sides (ref_ic0=='U'||ref_ic0=='D'?(ref_ic+1):ref_ic)
#define l2r_name(cb_sides) (sides(*cb_sides,1)==cb_sides[1])
#define cubie_left cubie_rl(0)
#define cubie_right cubie_rl(1)

#define do_movm(mov) { do_mov(mov); refresh }
#define do_algm(alg_rls) { char *alg=replace_alg_rls(alg_rls); do_alg(alg); free(alg); refresh }

typedef char cube_t[N][4];
typedef int mov_desc[2][N];

/* I use global variables - and I hate it! */

cube_t ref = {"UF","UR","UB","UL","DF","DR","DB","DL","FR","FL","BR","BL",
              "UFR","URB","UBL","ULF","DRF","DFL","DLB","DBR"},
           /* a solved cube - used as reference to the original positions */
       cube; /* the current state of the cube */

mov_desc mov_descs[6] = /* order: UDFBRL */
  {{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},        /* U */
    {3,-1,-1,-1,0,0,0,0,0,0,0,0,3,-1,-1,-1,0,0,0,0}},
   {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},        /* D */
    {0,0,0,0,1,1,1,-3,0,0,0,0,0,0,0,0,3,-1,-1,-1}},
   {{1,0,0,0,1,0,0,0,1,1,0,0,1,0,0,-1,-1,1,0,0},      /* F */
    {8,0,0,0,5,0,0,0,-4,-9,0,0,4,0,0,-3,1,-2,0}},
   {{0,0,1,0,0,0,1,0,0,0,1,1,0,-1,1,0,0,0,-1,1},      /* B */
    {0,0,9,0,0,0,4,0,0,0,-8,-5,0,1,4,0,0,0,1,-6}},
   {{0,0,0,0,0,0,0,0,0,0,0,0,-1,1,0,0,1,0,0,-1},      /* R */
    {0,9,0,0,0,3,0,0,-7,0,-5,0,1,6,0,0,-4,0,0,-3}},
   {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,1,0,-1,1,0},      /* L */
    {0,0,0,6,0,0,0,4,0,-2,0,-8,0,0,1,2,0,1,-4,0}}
  };

char *f="LFRBLFR", /* the faces from left to right (& around) + extra L&F */
     co[4]="D?", /* cubie name & original position - same as cube_ic except
                    for (maybe) the character order */
     solution[500];
int  ic, /* current cubie index.
            it is the variable most of the macros depend on. */
     sol_len; /* length of solution string (not move count) */


void do_mov_desc(int i) {
  cube_t tmp;

  int j, k, len=2;

  for (j=0; j<20; ++j) {
    int shift=mov_descs[i][0][j];
    if (j==12) len=3;
    if (shift<0) shift+=len;
    for (k=0; k<len; ++k)
      tmp[j][(k+shift)%len]=cube[j][k];
    tmp[j][len]=0;
  }


  for (j=0; j<20; ++j)
    strcpy(cube[j+mov_descs[i][1][j]],tmp[j]);
}

void do_mov(char *mov) {
/* performs and outputs a move */
  /*static*/ char dsc_f[]="UDFBRL", cnt_c[]="?+2-";
  #define desc_i(chr) (strchr(dsc_f,chr)-dsc_f)
  #define get_cnt(chr) (strchr(cnt_c,chr)-cnt_c)
  int i, cnt;
  i=desc_i(*mov);
  for (cnt=0; cnt<get_cnt(mov[1]); ++cnt) do_mov_desc(i);

  if (!sol_len||solution[sol_len-2]!=*mov) {
    strcat(solution,mov);
    sol_len+=2;
  } else {
    cnt=(cnt+get_cnt(solution[sol_len-1]))%4;
    if (cnt) solution[sol_len-1]=cnt_c[cnt];
    else {
      solution[sol_len-2]=0;
      sol_len-=2;
    }
  }
}

void do_alg(char *alg) {
/* assumes no spaces */
  char *p=alg;
  while (*p) {
    char mov[3];
    strncpy(mov,p,2);
    mov[2]=0;
    do_mov(mov);
    p+=2;
  }
}

int find(char *cubie) {
/* find the current position of the cubie that is supposed to be in the
   position specified by 'cubie' */
/* assumes that cubie is valid (that it exists in cube) */
  int i, j, len=strlen(cubie), low, high;
  if (len==2) { low=0; high=12; } else { low=12; high=20; }
  for (i=low; i<high; ++i) {
    int match=1;
    for (j=0; j<len; ++j) {
      match=match && (strchr(cube[i],cubie[j])!=NULL);
    }
    if (match) return i;
  }
}

char cubie_rl(char rl) { /* cubie right/left. 1 is right, 0 is left */
  char *cb_sides=cubie_sides;
  if (cb_sides[1]) { /* len(cb_sides)==2 */
    int tmp=rl|l2r_name(cb_sides);  if (tmp);
    return rl^l2r_name(cb_sides)?*cb_sides:cb_sides[1];}
  else
    return sides(*cb_sides,rl?1:-1);
}

char *replace_alg_rls(char *alg) {
  char *new_alg=(char*)malloc(strlen(alg)+1), *p=new_alg;
  strcpy(new_alg,alg);
  while (*p) {
    switch (*p) {
      case 'r': *p=cubie_right; break;
      case 'l': *p=cubie_left; break;
      case 'f': *p=*cubie_sides; break;
      case 'b': *p=sides(*cubie_sides,2);
    }
    p+=2;
  }
  return new_alg;
}

void mov_with_U_to(char side) {
 /* move current cubie cy rotating U so its right side for a corner or front
    for an edge would be on 'side' */
  char from=strlen(ref_ic)>2?cubie_right:*cubie_sides, cnt[]="?-2+",
       Umov[3]="U?";
  int dif=strchr(f,side)-strchr(f,from);
  if (dif) {
    if (dif<0) dif+=4;
    Umov[1]=cnt[dif];
    do_movm(Umov);
  }
}

int main() {
  int i, /* general purpose counter */
      found=0;
  char *fc;
  f++;
  for (i=0; i<N; ++i) scanf("%s",&cube[i]); /* read input */

/*=solution=================================================================*/

/*---------------the-cross---------------*/
  for (i=0; i<4; ++i) {
    co[1]=f[i];
    refresh
    check_in_place /* and continue to next edge if so */
    if_on_bottom do_algm("f2")
    else if_not_on_top do_algm("r+U-r-")
    /* the edge is now on U */
    if (cube_ic0=='D') { /* if D color on U */
      mov_with_U_to(f[i]);
      do_algm("f2")
    } else {
      mov_with_U_to(f[i+1]);
      do_algm("f-l+f+")
    }
  }
/*---------------FL-corners--------------*/
  for (i=0; i<4; ++i) {
    strcpy(co,ref[16+i]); /* ref[16] is DRF */
    refresh
    check_in_place
    if_on_bottom
      if (cube_ic[2]=='D') /* if D color on l */ do_algm("l-U-l+")
      else /* D color on r or D */ do_algm("r+U+r-")
    mov_with_U_to(co[1]);
    if (cube_ic0=='D') /* if D color on U */ do_algm("r+U-r-l-U2l+")
    else if (cube_ic[2]=='D') /* D color on r */ do_algm("r+U+r-")
    else /* D color on l */ do_algm("l-U-l+")
  }
/*---------------middle-edges------------*/
  co[2]=0;
  for (i=0; i<4; ++i) {
    strncpy(co,f+i,2); /* ref[8] is FR */
    refresh
    check_in_place
    if_not_on_top do_algm("l-U+l+U+r+U-r-")
    if (cube_ic0==co[1]) { /* if r color on U */
      mov_with_U_to(sides(co[0],-1));
      do_algm("b+U+b-U-r-U-r+")
    } else { /* l color on U */
      mov_with_U_to(sides(co[1],1));
      do_algm("b-U+b+U+l+U-l-")
    }
  }
/*---------------orient-LL-edges---------*/
  for (ic=0; ic<4 && !found; ++ic)
    found=cube_ic0=='U';
  if (!found) do_alg("F+R+U+R-U-F-U2F+U+R+U-R-F-");
  else if (cube_ic0=='U')
         if (*cube[ic+1]!='U') do_algm("r+U+b+U-b-r-")
         else /* edges already oriented */;
  else if (*cube[ic+1]=='U') do_algm("f+r+U+r-U-f-")
  else if (*cube[ic+2]=='U') do_algm("f+U+r+U-r-f-")
/*---------------permute-LL-edges--------*/
  #define do_cycle(ein_place,dir) { strcpy(co,cube[ein_place]); refresh mov_with_U_to(co[1]); do_algm("b2U" #dir "r+l-b2r-l+U" #dir "b2"); }
  strcpy(co,cube[0]);
  refresh
  fc=strchr(f,cube_ic1);
  if (fc[2]==cube[2][1]/*UB*/)
    if (fc[1]!=cube[1][1]/*UR*/) {
      mov_with_U_to(cube_ic1);
      do_algm("f+r-f-r+l+f-l+r-f-r+f+l2")
    } else /* edges alreay in corrent relative places */ mov_with_U_to(cube_ic1);
  else
    if (fc[1]==cube[1][1]/*UR*/) do_cycle(3,-)
    else if (fc[-1]==cube[3][1]) do_cycle(1,+)
    else if (fc[1]==cube[2][1])  do_cycle(0,+)
    else do_cycle(0,-)
/*---------------permute-LL-corners------*/
  #define c_place(name,place) (strchr(name,place[1])!=NULL && strchr(name,place[2])!=NULL)
  found=0;
  for (ic=12; ic<16 && !found; ++ic)
    found=c_place(cube_ic,ref_ic);
  if (found)
    if (ic==16 || !c_place(cube_ic,ref_ic)) {
      ic-=13;
      if (c_place(cube[12+(ic+1)%4],ref[12+(ic+2)%4]))
        do_algm("l+f-l+b2l-f+l+b2l2") /* cycle CCW */
      else do_algm("b-r+b-l2b+r-b-l2b2")
    } else /* corners already in place */;
  else if (c_place(cube[12],ref[14])) do_alg("R-L-U2R+L+F+B+U2F-B-U2");
  else if (c_place(cube[12],ref[13])) do_alg("L+F+U+F-U-F+U+F-U-F+U+F-U-L-");
  else do_alg("B+L+U+L-U-L+U+L-U-L+U+L-U-B-");
/*---------------orient-LL-corners-------*/
  for (i=0; i<4; ++i) {
    ic=12;
    if (cube_ic1=='U') do_alg("F+D+F-D-F+D+F-D-"); /* CW */
    else if (cube_ic[2]=='U') do_alg("D+F+D-F-D+F+D-F-");
    /* else - alreay rotated correctly */
    do_mov("U+");
  }
/*=EOF=solution=============================================================*/
  printf("%s",solution);
}
