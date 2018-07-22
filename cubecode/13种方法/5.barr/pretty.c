#include <stdio.h>

#define R(x) P["-<5>,=4?OERTMFPUDQSN3?;C2>:BLNUWJOSXM\
TVK1B9@A0C8IKXZ[GLVHJWY/A7=.@6<FH[QDIYRGZPE20.,/-31MJ\
GDFOLIHENK68:479;5PYVSQZWT[XUR=@C>AB?<"[a*20+c*4+x]-A]
char T[]="-#8-#9-#:-$68-#39-#;#;7=#=/;#;5=#=1;#8/.1#7,5#87,5#/.1#.;7=#5,#2;5=#\
1.#,#.#6,6#0.0#52947#.9,587#./.1-#597.:,$1:/587#6363#7:5.9,#587#.8,#.9,#:587#5\
:7.:,#15:7/#45972#796:7#.:,#8.:,#597#5:7#.8,587#,9-8,#/.9,1#.284,#5:7#85:7#587\
#597(-9-9-#69696#-=0;-#:.8,#.8,#.9,#.:,'.9,587#7=.;5#,;5=.#/.:/,0#9.8,85:7#:.8\
,85:7#.8,85:7#8.8,85:7+85:7:.8,#95:7:.8,#:5:7:.8,#5:7:.8,'69,6.9787&",N[]="FLB\
RUD",S[5][4]={"DF","DRF","FR","FR"},A=44,P[49],*s,**V,W[999];int I[96],
a,b,c,d,e,g,i,k,m,p,q,r,Y;

u(char*v,int r){
  for(;*v>=A;v++){
    a=(m=*v-A)/3;
    b=m%3+1;
    if(r) {
      //printf("%c%d\n", N[a],b);
      sprintf(W+(Y++)*2,"%c%d",N[a],b);
    }
    while(b--)
      for(c=0;c<5-a/6*3;c++){
	k=R(3);
	for(m=3;m;)R(m)=R(--m);
	R(0)=k;
      }
  }
}

o(int n){
  s=S[n];
  g=s[2]?1:0;
  for(e=0;s[e];e++){
    for(d=0;d<20;d++)
      if(g==d/12&&!strncmp(P+d*(2+g)-g*12,s,2+g)) {
	//printf("\n%s found at location %d, orientation %d\n", s, d, e);
	u(T+I[4*(n*6+e*(3-g)-g*3)+d],1);
      }
    for(m=s[d=0];s[d++];)
      s[d-1]=s[d];s[d-2]=m;
  }
}

y(int t) {
  while(t--) {
    for(k=N[m=3];m--;)
      N[m+1]=N[m];
    N[0]=k;
    u("8=@",0);
    for(k=0;P[k];k++)
      if((m=strchr(N,P[k])-N)<4)P[k]=N[(m+1)%4];
  }
}

main(int C,char**U){
  char t[5];
  setbuf(stdout, 0);
  for(I[i=k=0]=0;m=T[i++];)
    if(m<A){
      while(m-->35)
	I[++k]=i-1;
      I[++k]=i;
    }
  for(p=0;p<104976;p++) {
    W[Y=0]=0;
    r=p;
    //printf("r = %d N = %s", r, N);
    P[0]=0;
    for(V=U;*++V;)
      strcat(P,*V);
    for(i=0;i<4;i++) {
      t[3-i]=r%18+',';
      r /= 18;
    }
    t[4]=0;
    u(t,1);
    for(i=0;i<3;i++){
      //printf("\nstep %d\n", i);
      for(q=0;q<4;q++) {
	if(i==2)
	  u(";",1); // turn D1
	if(!i||q!=3)
	  o(i);
	y(1);
      }
    }
    y(3);
    o(1);
    o(3);
    y(1);

    //flip LL edges:
    for(i=0;i<4;i++){
      if(P[0]!='U')
	if(P[2]!='U')
	  u(",85:7.",1);
	else if(P[4]!='U')
	  u(",587:.",1);
      y(1);
    }

    for(i=0;i<4;i++) {
      //printf("P = %s\n", P);
      if(!strncmp(P,"UFURUB",6) &&
	 !strncmp(P+25,"FRURBU",6)) {
	puts(W);
	exit(0);
      }
      u("8",1);
    }
    //printf("\n");
  }
}
