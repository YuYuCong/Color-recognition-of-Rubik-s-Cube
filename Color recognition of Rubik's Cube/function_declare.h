#ifndef FUNCTION_DECLARE_H
#define FUNCTION_DECLARE_H

#include "include_all.h"

/// Global Variables 
///白红绿蓝橙黄-->白x绿y红z
//WHITE
const int W_LowH = 100;
const int W_HighH = 140;
const int W_LowS = 90;
const int W_HighS = 255;
const int W_LowV = 90;
const int W_HighV = 255;
//RED
const int R_LowH = 100;
const int R_HighH = 140;
const int R_LowS = 90;
const int R_HighS = 255;
const int R_LowV = 90;
const int R_HighV = 255;
//GREEN
const int G_LowH = 50;
const int G_HighH = 95;
const int G_LowS = 51;
const int G_HighS = 251;
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
const int Y_LowH = 22;
const int Y_HighH = 29;
const int Y_LowS = 51;
const int Y_HighS = 251;
const int Y_LowV = 0;
const int Y_HighV = 255;
/// Function headers 

/* 采集魔方颜色，修改参数 */
int check_value(Mat & srcImage);
/* 鉴别魔方颜色 */
int get_colour(Mat & srcImage);
#endif // !FUNCTION_DECLARE_H

