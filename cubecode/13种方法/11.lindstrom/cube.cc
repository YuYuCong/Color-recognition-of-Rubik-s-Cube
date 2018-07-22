// UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR
#include <string>
#include <queue>
#include <iostream>

using namespace std;

typedef string cube;

short rotations[][20] = {
	// U
	{0, 6, 4, 2,  
	 1, 7, 5, 3,  
	 26, 35, 32, 29,  
	 25, 34, 31, 28,  
	 24, 33, 30, 27},
	// F
	{0, 17, 8, 19,  
	 1, 16, 9, 18,  
	 24, 37, 39, 34,  
	 25, 38, 40, 35,  
	 26, 36, 41, 33},

	// L
	{7, 19, 15, 23,
	 6, 18, 14, 22,
	 33, 40, 42, 31,
	 35, 39, 44, 30,
	 34, 41, 43, 32},

	// D
	{8,10,12,14,
	 9,11,13,15,
	 38, 47, 44, 41,
	 36, 45, 42, 39,
	 37, 46, 43, 40},

	// B
	{5, 22, 13, 20,
	 4, 23, 12, 21,
	 30, 43, 45, 28,
	 31, 44, 46, 29,
	 32, 42, 47, 27},

	// R
	{3, 21, 11, 17,
	 2, 20, 10, 16,
	 25, 27, 46, 36,
	 24, 29, 45, 38,
	 26, 28, 47, 37}
};

const char *mnemonics = "UFLDBR";

inline bool opposite(int a, int b) {
	return (a+3)%6==b;
}

void rotate(cube &c, int op, int steps) {
	for (int k = 0; k < steps; k++) {
	for (int i = 0; i < 5; i++) {
		char temp = c[rotations[op][i*4+3]];
		for (int j = 3; j > 0; j--) {
			c[rotations[op][i*4+j]] = c[rotations[op][i*4+j-1]];
		}
		c[rotations[op][i*4]] = temp;
	}
	}
}

const cube goals[] = {
/*pre_goal =*/    "??????????????????FL?????????????ULF????????????",
/*i2x2x2_goal =*/ "UF????UL??????????FL?????????????ULF????????????",
/*pre2_goal =   "UFUR??UL????????FRFL?????????????ULF????????????";*/
/*i2x2x3_goal =*/ "UFUR??UL????????FRFL????UFR??????ULF????????????",
/*step3_goal =*/  "UFUR??ULDF??????FRFL????UFR??????ULFDRFDFL??????",
/*step4_goal =*/  "UFURUBULDF??????FRFL????UFRURB???ULFDRFDFL??????",
/*step5_goal =*/  "UFURUBULDF??????FRFL????UFRURBUBLULFDRFDFL??????",
/*step6a_goal =*/ "UFURUBULDF??????FRFLBR??UFRURBUBLULFDRFDFL??????",
/*step6_goal =*/  "UFURUBULDF??????FRFLBRBLUFRURBUBLULFDRFDFL??????",
/*step7_goal =*/  "UFURUBULDF??????FRFLBRBLUFRURBUBLULFDRFDFLDLB???",
/*step8_goal =*/  "UFURUBULDF??????FRFLBRBLUFRURBUBLULFDRFDFLDLBDBR",
/*step9_goal =*/  "UFURUBULDFDR????FRFLBRBLUFRURBUBLULFDRFDFLDLBDBR",
/*step10_goal =*/ "UFURUBULDFDRDB??FRFLBRBLUFRURBUBLULFDRFDFLDLBDBR",
/*final_goal =*/  "UFURUBULDFDRDBDLFRFLBRBLUFRURBUBLULFDRFDFLDLBDBR" };

int heuristic(const cube &goal, const cube &c) {
	int k = 0;
	// ick...what one doesn't do for speed...
	const char *p = goal.c_str();
	const char *q = c.c_str();
	while (*p) {
		if (*p != '?' && *p != *q) k++;
		p++; q++;
	}
	return k;
}

inline bool goalp(const cube &goal, const cube &c) {
	return heuristic(goal, c)==0;
}

const int MAXDEPTH = 1000;

int solution[MAXDEPTH];
int solp;

#include <map>

map<cube,int> fulsols;
bool useful;

bool dfs(cube &start, const cube &goal, int depth, int maxdepth) {
	int h = heuristic(goals[12], start);
	if (useful && fulsols.find(start)!=fulsols.end()) {
		while (fulsols[start]!=-1) {
			solution[solp++] = fulsols[start]-1;
			rotate(start, (fulsols[start]-1)/3, (fulsols[start]-1)%3+1);
		}
		return true;
	}
	if (depth+h > maxdepth) return false;
	if (goalp(goal, start)) return true;
	for (int i = 0; i < 6*3; i++) {
		if (solp>0 && (solution[solp-1]/3 == i/3) || (opposite(solution[solp-1]/3,
																													 i/3)))
			continue;
		rotate(start, i/3, 1);
		solution[solp++] = i;
		if (dfs(start, goal, depth+1, maxdepth)) {
			return true;
		}
		solp--;
		if (i%3==2)
			rotate(start, i/3, 1);
	}
	return false;
}

void solve(cube &c, const cube &goal) {
	for (int i = 0; i < MAXDEPTH; i++) {
		if (dfs(c, goal, 0, i)) {
			for (int i = 0; i < solp; i++) {
				cout << mnemonics[solution[i]/3] << solution[i]%3+1 << " ";
			}
			solp = 0;
			cout << endl;
			return;
		}
	}
}

int main(int argc, char **argv) {
	cube c;
	for (int i = 0; i < 20; i++) {
		c+=argv[i+1];
	}

	//	cerr << "BFS" << endl;

	fulsols[goals[12]] = -1;

	queue<pair<cube,pair<int,int> > > Q;
	Q.push(make_pair(goals[12],make_pair(0, -1)));

	while (!Q.empty()) {
		pair<cube,pair<int,int> > curr = Q.front();
		Q.pop();
		for (int i = 9; i < 15; i++) {
			if (i/3 == curr.second.second/3) continue;
			rotate(curr.first, i/3, 1);
			int &val = fulsols[curr.first];
			if (val == 0) {
				val = (i/3)*3+(2-i%3)+1;
				if (curr.second.first<9)
					Q.push(make_pair(curr.first, make_pair(curr.second.first+1, val-1)));
			}
			if (i%3==2)
				rotate(curr.first, i/3, 1);
		}
	}

	//	cerr << "IDA*" << endl;
	for (int i = 0; i < 13; i++) {
		if (i==3) useful = true;
		solve(c, goals[i]);
	}
}
