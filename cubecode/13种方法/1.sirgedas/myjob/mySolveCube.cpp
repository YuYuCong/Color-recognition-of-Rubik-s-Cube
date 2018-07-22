// src: http://tomas.rokicki.com/cubecontest/winners.html
// By: Tomas Sirgedas, Ann Arbor, MI, USA 
// fix by noski
// transplant by WlliamYu_2018.01.30
//main function Input Just Like UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR

//and I made some work on this program for myself as well 
//as other new_here to understand Tomas`job easily
//上述顺序描述了一个标准已还原魔方
//there`re something wrong still
#include <string.h>
#include <iostream>

using std::string;
using std::cout;
using std::swap;
using std::endl;

string data = "2#6'&78)5+1/AT[NJ_PERLQO@IAHPNSMBJCKLRMSDHEJNPOQFKGIQLSNF@DBROPMAGCEMPOACSRQDF";
char inva[48], b[48], cur_phase, search_mode, history_idx, history_mov[48], history_rpt[48], depth_to_go[5 << 20], hash_table[48][6912];

struct Cubelet {
	char pos, twi;
} cubelet[48];

int min(int a, int b) {
	return a<b ? a : b;
}

void rot(char cur_phase) {
	if (cur_phase < 4)
		for (int i = -1; ++i < 4;)
			cubelet[64 ^ data[20 + cur_phase * 8 + i]].twi =
			(cubelet[64 ^ data[20 + cur_phase * 8 + i]].twi + 2 - i % 2) % 3, cubelet[64 ^ data[20 + cur_phase * 8 + i + 4]].twi ^= cur_phase < 2;
	for (int i = -1; ++i < 7;)
		swap(cubelet[64 ^ data[20 + cur_phase * 8 + i + (i != 3)]], cubelet[64 ^ data[20 + cur_phase * 8 + i]]);
}

int hashf() {
	int ret = 0;
	switch (cur_phase) {
	case 0:
		for (int i = -1; ++i < 11;)
			ret += ret + cubelet[i].twi;
		return ret;

	case 1:
		for (int i = -1; ++i < 7;)
			ret = ret * 3 + cubelet[i + 12].twi;
		for (int i = -1; ++i < 11;)
			ret += ret + (cubelet[i].pos > 7);
		return ret - 7;

	case 2:
		for (int i = -1; ++i < 8;)
			if (cubelet[i + 12].pos<16)
				inva[cubelet[i + 12].pos & 3] = ret++;
			else b[i - ret] = cubelet[i + 12].pos & 3;
			for (int i = -1; ++i < 7;)
				ret += ret + (cubelet[i].pos > 3);
			for (int i = -1; ++i < 7;)
				ret += ret + (cubelet[i + 12].pos > 15);
			return ret * 54 + (inva[b[0]] ^ inva[b[1]]) * 2 + ((inva[b[0]] ^ inva[b[2]]) >(inva[b[0]] ^ inva[b[3]])) - 3587708;
	}

	for (int i = -1; ++i < 5;) {
		ret *= 24;
		int cur_phase;
		for (cur_phase = -1; ++cur_phase < 4;)
			for (int k = -1; ++k < cur_phase;)
				if (cubelet[i * 4 + cur_phase].pos < cubelet[i * 4 + k].pos)
					ret += cur_phase << cur_phase / 3;
	}
	return ret / 2;
}

int do_search(int dpt) {
	int h = hashf(), q = cur_phase / 2 * 19 + 8 << 7;
	if ((dpt < hash_table[cur_phase][h%q] | dpt < hash_table[cur_phase + 4][h / q]) ^ search_mode) {
		if (search_mode)
			if (dpt <= depth_to_go[h])
				return !h;
			else depth_to_go[h] = dpt;

			if (hash_table[cur_phase][h%q] > dpt)
				hash_table[cur_phase][h%q] = dpt;
			if (hash_table[cur_phase + 4][h / q] > dpt)
				hash_table[cur_phase + 4][h / q] = dpt;

			for (int k = -1; ++k < 6;)
				for (int i = -1; ++i < 4;) {
					rot(k);
					if (k < cur_phase * 2 & i != 1 || i > 2)
						continue;
					history_mov[history_idx] = k;
					history_rpt[history_idx++] = i;
					if (do_search(dpt - search_mode * 2 + 1))
						return 1;
					history_idx--;
				}
	}
	return 0;
}


int main(int argc, char** argv) {
	//if (argc != 21) {
	//	cout << endl
	//		<< "### Input Error. Argument Format:" << endl
	//		<< "rubik.exe UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR" << endl;
	//	system("pause");
	//	return 0;
	//}
	char Current_cube[20][4] = { "UR","UB","UL","UF","DF","DR","DB","DL","FR","FL","BR","BL",
								"URB","UBL","ULF","UFR","DRF","DFL","DLB","DBR" };
	memset(hash_table, 6, sizeof(hash_table));
	for (int i = -1; ++i < 20;)
		cubelet[i].pos = i;
	for (cur_phase = -1; ++cur_phase < 4;)
		do_search(0);
	for (int i = -1; ++i < 20;) {
		string s = Current_cube[i + 1] + string("!");
		cubelet[i].pos = data.find(s[0] ^ s[1] ^ s[2]);
		int x = min(s.find(85), s.find(68));
		cubelet[i].twi = ~x ? x : s[0]>70;
	}
	for (int i = -1; ++i < 5;)
		swap(cubelet[64 ^ data[20 + cur_phase * 8 + i + 16]], cubelet[64 ^ data[20 + cur_phase * 8 + i + 21]]);

	search_mode = 1;

	for (cur_phase = -1; ++cur_phase < 4;)
		for (int i = -1; ++i < 20;)
			if (do_search(i))
				break;
	for (int k = -1; ++k < history_idx;)
		cout << "FBRLUD"[history_mov[k]] << history_rpt[k] + 1 << " ";

	cout << endl;
	return 0;
}
