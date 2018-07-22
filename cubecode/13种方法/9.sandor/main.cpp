#include <iostream>
#include <vector>

#define fr2s(i, x, y, s) for(int i=x; i<y; i+=s)
#define fr2(i, x, y) fr2s(i, x, y, 1)
#define fr(i, x) fr2(i, 0, x)
#define frs(i, x, s) fr2s(i, 0, x, s)

using namespace std;
//                       1         2         3         4
//             012345678901234567890123456789012345678901234567

//             xx  xx  xx  xx  xx  xx  xxx   xxx   xxx   xxx
//             *+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXY
string solved="UFURUBULDFDRDBDLFRFLBRBLUFRURBUBLULFDRFDFLDLBDBR",
 faces="ULFRDB", opfaces="DRBLUF",
 moves="KHEB*0.,KRTI0<8@KDNS*;2=BGWP,>4:QNWT2468IVXG/@7>", s,
 orient=solved.substr(24)+"FDRFRUFULFLDRDBRBURUFRFDBDLBLUBURBRDLDFLFULUBLBD",

 e1[]={"U2F1R1L3U2L1R3F1U2", "R1F1R3D3L3U3F3U1L1D1", "U3R3U2B2D2L3D2B2U3",
  "R1U1F3U3R3D3L3F1L1D1", "R3L3F2R1L1U1D1F2U3D3", "R3F3D3F2D1R1F1U1F2U3",
  "R3F1L1F3R1F1L3"},
 c1[]={"U2R2U3L3U1R2U3L1U3", "R2B3R1F2R3B1R1F2R1", "U1L1D3L3U3L1D1L3",
  "U3R3D3R1U1R3D1R1", "R3F2R3B3R1F2R3B1R2", "R1F3L3F1R3F3L1F1"};

bool out=true;
#define shift(x, y) moves[f+x]/y*y+(moves[f+x]+j)%y-'*'

char af=32, z='0';
int ar=0;

void apply(string m)
{
	if(out)
		if(m[0]!=af)
		{
			if(ar)
				cout<<af<<ar;
			af=m[0];
			ar=m[1]-z;
		}
		else
			ar=(ar+m[1]-z)%4;
	int f=faces.find(m[0])*8;
	int n=m[1]-z;
	fr(i, n)
		fr(l, 2)
		{
			int q=3-l, p=4*l;
			fr(j, q)
			{
				char t=s[shift(3+p, q)];
				for(int k=3+p; k>p; --k)s[shift(k, q)]=s[shift(k-1, q)];
				s[shift(p, q)]=t;
			}
		}
}

char op(char x)
{
	return opfaces[faces.find(x)];
}

string transl(string f, string fa)
{
	fr(i, 3)fa+=op(fa[i^(i<2)]);
	frs(i, f.length(), 2)
		f[i]=fa[faces.find(f[i])];
	return f;
}

string inv(string f)
{
	string g=f;
	for(int i=f.length()-2, j=0; i>=0; i-=2, j+=2)
	{
		g[j]=f[i];
		g[j+1]=100-f[i+1];
	}
	return g;
}

void applyf(string f, string fa="ULF")
{
	f=transl(f, fa);
	frs(i, f.length(), 2)
		apply(f.substr(i, 2));
}

vector<string>mvs;
int pw[20], x1, x2;

#define APP(i) fr(k, i){applyf(mvs[t%sz]);t/=sz;}

bool transf(int(*test)(), int lim)
{
	int best=test(), besti=0, bestj, sz=mvs.size(), t;
	if(!best)return false;
	pw[0]=1;
	fr(i, lim)pw[i+1]=sz*pw[i];
	string bestf, s1=s;
	out=false;
	fr2(i, 1, lim)if(best)
		fr(j, pw[i])if(best)
		{
			s=s1;
			t=j;
			APP(i)
			int sc=test();
			if(sc<best)
			{
				best=sc;
				besti=i;
				bestj=j;
			}
		}
	s=s1;
	if(!besti)return false;
	t=bestj;
	out=true;
	APP(besti)
	return true;
}

int tst(int x)
{
	return s[x]!=solved[x];
}

int tst2(int x)
{
	return tst(x)+tst(x+1);
}

#define TSTBEGIN(n) int n(){int t=0;
#define TSTEND return t;}
#define TSTADD t+=tst(i);

TSTBEGIN(test3)
	fr2(i, x1, x2)TSTADD
TSTEND

TSTBEGIN(test3d)
	fr2s(i, x1, x2, 2)
	{
		t+=tst2(i);
		if(tst2(i)&&s[i]=='D')t+=3;
	}
TSTEND

TSTBEGIN(test3m)
	fr2s(i, x1, x2, 2)
	{
		t+=tst2(i);
		if(i>15&&tst2(i)&&s[i]!='U'&&s[i+1]!='U')t+=3;
		if(i<16)t+=9*tst2(i);
	}
TSTEND

TSTBEGIN(testc)
	fr2(i, 24, 36)t+=s[i]=='D';
TSTEND

TSTBEGIN(test4c)
	fr2(i, 27, 33)TSTADD
	fr2(i, 42, 48)TSTADD
TSTEND

#define PUSH(x) mvs.push_back(x);
#define fi6 fr(i, 6)
#define frk fr2(k, '1', '4')
#define S2 {s2[0]=faces[i];s2[1]=k;PUSH(s2);}
#define CLR mvs.clear();
#define RUN(t, x) while(transf(t, x));
#define frj frs(j, 72, 3)
#define OF orient[j+2]
#define ORI(x) transl(x[i], orient.substr(j, 3))
#define PUSH2(x) {PUSH(x) PUSH(inv(x))}

void solve()
{
	string s2="  ";
	fi6 frk S2
	x1=8;
	x2=16;
	RUN(test3d, 4)
	x2=24;
	CLR
	fi6 if(i!=4) frk if(k!='2') S2
	PUSH("U2")
	while(true)
	{
		RUN(test3m, 4)
		if(!test3m())break;
		out=true;
		apply("U1");
	}
	CLR
	PUSH("U1")
	PUSH("U2")
	fr(i, 7) frj if(OF=='U') PUSH2(ORI(e1))
	x1=0;
	RUN(test3, 4);
	CLR
	fi6 frj if(OF!='D'&&OF!='U') PUSH(ORI(c1))
	RUN(testc, 3);
	CLR
	fi6 frj if(OF=='D'||OF=='U') PUSH2(ORI(c1))
	x1=24;
	x2=48;
	RUN(test4c, 3)
	CLR
	fi6 frj if(OF=='F') PUSH2(ORI(c1))
	RUN(test3, 3);
	if(ar)
	{
		cout<<af<<ar;
		ar=0;
	}
}

int main(int argc, char *argv[])
{
	fr2(i, 1, argc)s+=argv[i];
	solve();
}
