#include"function_declare.h"

/**
* @function deviation
* @brief 纠正错误
* @return
*/
void deviation(int Start_color[54])
{
   //waiting!!
}

/**
* @function Decoding
* @brief 解码
* @return
*/
char* Decoding(const int a, const int b, const int c, char* des)
{
	char temp[4];

	if (a == Start_color[4]) temp[0] = facelet[0];
	else if (a == Start_color[13]) temp[0] = facelet[1];
	else if (a == Start_color[22]) temp[0] = facelet[2];
	else if (a == Start_color[31]) temp[0] = facelet[3];
	else if (a == Start_color[40]) temp[0] = facelet[4];
	else temp[0] = facelet[5];

	if (b == Start_color[4]) temp[1] = facelet[0];
	else if (b == Start_color[13]) temp[1] = facelet[1];
	else if (b == Start_color[22]) temp[1] = facelet[2];
	else if (b == Start_color[31]) temp[1] = facelet[3];
	else if (b == Start_color[40]) temp[1] = facelet[4];
	else temp[1] = facelet[5];

	if (c == Start_color[4]) temp[2] = facelet[0];
	else if (c == Start_color[13]) temp[2] = facelet[1];
	else if (c == Start_color[22]) temp[2] = facelet[2];
	else if (c == Start_color[31]) temp[2] = facelet[3];
	else if (c == Start_color[40]) temp[2] = facelet[4];
	else if (c == Start_color[49]) temp[2] = facelet[5];
	else temp[2] = '\0';

	strcpy_s(des, 4, temp);
	return des;
}

/**
* @function standardize_color
* @brief 转化为标准魔方描述顺序
* @return
*/
void standardize_color(const int Start_color[54], char Start_cube[20][4])
{
	cout << "Start_color:";
	for(int i=0;i<54;i++){ cout << Start_color[i]<<" "; }
	cout << endl<< "I`m going to standardize the color data." << endl;

	///Start_color和Start_cube转化,建立在上述图片的前提上
	char *temp_here = new char[4];
	Decoding(Start_color[7], Start_color[19], 6, temp_here);
	strcpy_s(Start_cube[0], temp_here);
	free(temp_here);

	Decoding(Start_color[5], Start_color[10], 6, temp_here);
	strcpy_s(Start_cube[1], temp_here);
	delete[]temp_here;

	Decoding(Start_color[1], Start_color[46], 6, temp_here);
	strcpy_s(Start_cube[2], temp_here);
	delete[]temp_here;

	Decoding(Start_color[3], Start_color[37], 6, temp_here);
	strcpy_s(Start_cube[3], temp_here);
	delete[]temp_here;

	Decoding(Start_color[28], Start_color[25], 6, temp_here);
	strcpy_s(Start_cube[4], temp_here);
	delete[]temp_here;

	Decoding(Start_color[32], Start_color[16], 6, temp_here);
	strcpy_s(Start_cube[5], temp_here);
	delete[]temp_here;

	Decoding(Start_color[34], Start_color[52], 6, temp_here);
	strcpy_s(Start_cube[6], temp_here);
	delete[]temp_here;

	Decoding(Start_color[30], Start_color[43], 6, temp_here);
	strcpy_s(Start_cube[7], temp_here);
	delete[]temp_here;

	Decoding(Start_color[23], Start_color[12], 6, temp_here);
	strcpy_s(Start_cube[8], temp_here);
	delete[]temp_here;

	Decoding(Start_color[21], Start_color[41], 6, temp_here);
	strcpy_s(Start_cube[9], temp_here);
	delete[]temp_here;

	Decoding(Start_color[48], Start_color[14], 6, temp_here);
	strcpy_s(Start_cube[10], temp_here);
	delete[]temp_here;

	Decoding(Start_color[50], Start_color[39], 6, temp_here);
	strcpy_s(Start_cube[11], temp_here);
	delete[]temp_here;

	Decoding(Start_color[8], Start_color[20], Start_color[9], temp_here);
	strcpy_s(Start_cube[12], temp_here);
	delete[]temp_here;

	Decoding(Start_color[2], Start_color[11], Start_color[45], temp_here);
	strcpy_s(Start_cube[13], temp_here);
	delete[]temp_here;

	Decoding(Start_color[0], Start_color[47], Start_color[36], temp_here);
	strcpy_s(Start_cube[14], temp_here);
	delete[]temp_here;

	Decoding(Start_color[6], Start_color[38], Start_color[18], temp_here);
	strcpy_s(Start_cube[15], temp_here);
	delete[]temp_here;

	Decoding(Start_color[29], Start_color[15], Start_color[26], temp_here);
	strcpy_s(Start_cube[16], temp_here);
	delete[]temp_here;

	Decoding(Start_color[27], Start_color[24], Start_color[44], temp_here);
	strcpy_s(Start_cube[17], temp_here);
	delete[]temp_here;

	Decoding(Start_color[33], Start_color[42], Start_color[53], temp_here);
	strcpy_s(Start_cube[18], temp_here);
	delete[]temp_here;

	Decoding(Start_color[35], Start_color[51], Start_color[17], temp_here);
	strcpy_s(Start_cube[19], temp_here);
	delete[]temp_here;

	cout << "Start_cube:";
	for (int i = 0; i<20; i++) { cout << Start_cube[i] << " "; }

}
