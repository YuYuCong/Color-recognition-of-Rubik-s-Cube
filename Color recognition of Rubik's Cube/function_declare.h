#ifndef FUNCTION_DECLARE_H
#define FUNCTION_DECLARE_H

#include "include_all.h"

/// Function headers 
/* 采集魔方颜色，修改参数 */
//#define check_color_value
#ifdef check_color_value
int check_value(Mat& srcImage);
#endif
/* 鉴别魔方颜色 */
void get_color(Mat& srcImage, int color_this_face[9]);
/**/
void deviation(int Start_color[54]);
/**/
void standardize_color(const int Start_color[54], char Start_cube[20][4]);
/* 解魔方 */
int mySolveCube(char start_cube[20][4]);

/// Global Variables
extern string colorlet;
extern string facelet;              //position->|  U  |  R    |   F    |   D    |   L    |   B    |
extern int Start_color[54];         //          |0-4-8|9-13-17|18-22-26|27-31-35|36-40-44|45-49-53|
extern int color_this_face[9];      //          |0-4-9|0 -4- 9|0 -4-  9|0 -4-  9|0 -4-  9|0 -4-  9|	
extern char Start_cube[20][4];

///白红绿蓝橙黄-->白x绿y红z
//WHITE
const int W_LowH = 10;
const int W_HighH = 43;
const int W_LowS = 0;
const int W_HighS = 26;
const int W_LowV = 0;
const int W_HighV = 255;
//RED
const int R_LowH = 0;
const int R_HighH = 8;
const int R_LowS = 124;
const int R_HighS = 255;
const int R_LowV = 0;
const int R_HighV = 255;
//GREEN
const int G_LowH = 41;
const int G_HighH = 85;
const int G_LowS = 10;
const int G_HighS = 255;
const int G_LowV = 0;
const int G_HighV = 255;
//BLUE
const int B_LowH = 83;
const int B_HighH = 124;
const int B_LowS = 51;
const int B_HighS = 251;
const int B_LowV = 0;
const int B_HighV = 255;
//ORANGE
const int O_LowH = 100;
const int O_HighH = 140;
const int O_LowS = 90;
const int O_HighS = 255;
const int O_LowV = 90;
const int O_HighV = 255;
//YELLOW
const int Y_LowH = 12;
const int Y_HighH = 15;
const int Y_LowS = 71;
const int Y_HighS = 255;
const int Y_LowV = 0;
const int Y_HighV = 255;



#endif // !FUNCTION_DECLARE_H

