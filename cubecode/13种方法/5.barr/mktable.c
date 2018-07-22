char movenames[] = "FLBRUDE", *alg_table[4][24] = {
  { // solve DF
    "F2", // UF
    "U1F2", // UR
    "U2F2", // UB
    "U3F2", // UL
    0, // DF
    "R2U1F2", // DR
    "B2U2F2", // DB
    "D1", // DL
    "D1R3D3", // FR
    "D3L1D1", // FL
    "D1R1D3", // BR
    "D3L3D1", // BL
    
    "U1L1F3L3", // UF
    "R3F1R1", // UR
    "U1R3F1R1", // UB
    "L1F3L3", // UL
    "F3D1R3D3", // DF
    "R1F1", // DR
    "B1D1R1D3", // DB
    "L3F3", // DL
    "F1", // FR
    "F3", // FL
    "R2F1R2", // BR
    "L2F3L2", // BL
  }, { // solve DRF
    "R1B1U2B3R3", // UFR*
    "F3U2F1R1U1R3", // URB*
    "F3L1F3L3F2", // UBL*
    "R1U2R3F3U3F1", // ULF*
    0,             // DRF*
    "L3U3L1R1U1R3", // DFL*
    "R2B2R2B2", // DLB*
    "R3U3R1F3U2F1", // DBR*
    
    "R1U1R3", // RUF*
    "F3U1F1", // BUR*
    "F3U2F1", // LUB*
    "U3R1U1R3", // FUL*
    "R1U3R3F3U3F1", // FDR*
    "L3R1U3R3L1", // LDF*
    "B3R1U2R3B1", // BDL*
    "R3U2R2U3R3", // RDB*
    
    "F3U3F1", // FRU*
    "U1F3U3F1", // RBU*
    "R1U2R3", // BLU*
    "R1U3R3", // LFU*
    "F3U1F1R1U1R3", // RFD*
    "F1U2F2U1F1", // FLD*
    "L1F3U2F1L3", // LBD*
    "F3B1U1B3F1", // BRD*
  }, { // solve FR
    "R1U3R3", // UF
    "U1R1U3R3", // UR
    "R1U1R3", // UB
    "R1U2R3", // UL
    0, // DF
    0, // DR
    0, // DB
    0, // DL
    0, // FR
    "F2U2F2U2F2", // FL
    "R2U2R2U2R2", // BR
    "F2D3L2D1F2", // BL
    
    "U3F3U1F1", // UF
    "F3U1F1", // UR
    "F3U2F1", // UB
    "F3U3F1", // UL
    0, // DF
    0, // DR
    0, // DB
    0, // DL
    "F3U2F1R1U1R3", // FR
    "R3D3F3D1R1", // FL
    "F1D1R1D3F3", // BR
    "L1F3U3L1F1L2", // BL
  }, { // solve final FR
    "U2F3U1F1U1R1U3R3", // UF
    "U3F3U1F1U1R1U3R3", // UR
    "F3U1F1U1R1U3R3", // UB
    "U1F3U1F1U1R1U3R3", // UL
    0, // DF
    0, // DR
    0, // DB
    0, // DL
    0, // FR
    0, // FL
    0, // BR
    0, // BL
    
    "U1R1U3R3U3F3U1F1", // UF
    "U2R1U3R3U3F3U1F1", // UR
    "U3R1U3R3U3F3U1F1", // UB
    "R1U3R3U3F3U1F1", // UL
    0, // DF
    0, // DR
    0, // DB
    0, // DL
    "R2U2F1R2F3U2R3U1R3", // FR
    0, // FL
    0, // BR
    //0, // BL
    //"U1D3E3" // y
    //"D1" //D1
    //"U1" //U1
    //"F1U1R1U3R3F3" // LL flip UF and UR
    //"F1R1U1R3U3F3" // LL flip UF and UR
  }
};

//UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR
//00 00 00 00 00 11 11 11 11 11 22 22 222 222 333 333 333 344 444 444
//01 23 45 67 89 01 23 45 67 89 01 23 456 789 012 345 678 901 234 567
//
//0  1  2  3  4  5  6  7  8  9  10 11 12  13  14  15  16  17  18  19

//FLBRUD
static int move_table[32][4] = {
  { 1,16, 9,18},{ 0,17, 8,19},{35,25,38,40},{33,26,36,41},{24,37,39,34}, //F
  { 7,19,15,23},{ 6,18,14,22},{32,34,41,43},{30,35,39,44},{33,40,42,31}, //L
  { 5,22,13,20},{21, 4,23,12},{29,31,44,46},{47,27,32,42},{28,30,43,45}, //B
  { 3,21,11,17},{ 2,20,10,16},{26,28,47,37},{24,29,45,38},{27,46,36,25}, //R
  { 6, 4, 2, 0},{ 3, 1, 7, 5},{33,30,27,24},{26,35,32,29},{28,25,34,31}, //U
  {10,12,14, 8},{11,13,15, 9},{36,45,42,39},{37,46,43,40},{47,44,41,38}, //D
  {17,20,23,18},{21,22,19,16}, //E
};

main() {
  int i,j,k=0,m,x;
  for(i=0;i<4;i++) {
    for(j=0;j<24;j++) {
      if(alg_table[i][j]) {
	if(k) {
	  printf("%c", k-1+'#');
	  k=0;
	}
	for(m=0;alg_table[i][j][m];m+=2) {
/* 	  printf("%c",(strchr(movenames,alg_table[i][j][m])-movenames)*4+ */
/* 		 alg_table[i][j][m+1]-'0'+','); */
	  printf("%c",(strchr(movenames,alg_table[i][j][m])-movenames)*3+
		 alg_table[i][j][m+1]-'1'+',');
	}
      }
      k++;
    }
  }
  if(k>1) {
    printf("%c", k-1+'#');
  }
  printf("\n\n",k);
  for(i=0;i<32;i++) {
    for(j=0;j<4;j++) {
      printf("%c", move_table[i][j]+',');
    }
  }
  printf("\n");
}
