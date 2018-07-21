//*=====================================================================================
//*                          main.c--
//*				    Copyleft! 2018 William Yu
//*          Some rights reserved：CC(creativecommons.org)BY-NC-SA
//*				    Copyleft! 2018 William Yu
//*      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
//*
//* Filename                : main.cpp
//* Description             : (编译环境：win10 + OpenCV3.3.0 + VS2017)
//* Reference               :
//* Programmer(s)           : William Yu, windmillyucong@163.com
//* Company                 : HUST ThinkFly Studio
//* Modification History	  : ver1.0, 2018.01.20, William Yu
//*
//=====================================================================================*/

#include"function_declare.h"

//----------------------------------------
//TODO:color_this_face数组内部的排列顺序，
//     事实上并不能是cubielet所定义的顺序,与旋转方式有关，等机械结构确定之后，调整顺序即可，
//     需要完善函数deviation;
//     此处standardize_color函数暂且认为是cubielet所定义的顺序
//----------------------------------------


/// Global Variables 
string colorlet = "WRGBOY";
string facelet = "URFDLB";   //position->|  U  |  R    |   F    |   D    |   L    |   B    |
int Start_color[54];         //          |0-4-8|9-13-17|18-22-26|27-31-35|36-40-44|45-49-53|
int color_this_face[9];      //          |0-4-9|0 -4- 9|0 -4-  9|0 -4-  9|0 -4-  9|0 -4-  9|	
char Start_cube[20][4];

/**
* @function main
*/

//int main(int argc, const char** argv)
//{
//	VideoCapture cap;
//	Mat srcImage;
//
//	#ifdef check_color_value
//		cap.open(0);
//		if (!cap.isOpened())
//		{
//			cout << "Can`t open camera\n";
//		}
//		cap >> srcImage;
//		cap.release();
//		if (srcImage.empty())
//			return -1;
//		srcImage=imread("Cube.jpg");
//		check_value(srcImage);
//		return -1;
//	#endif // !check_color_value
//
//	
//	for(int face = 0; face<6; face++)
//	{
//		cout << "Please show me the face" << " " << facelet[face] << endl;
//		//--TODO：串口发送下位机转动命令
//		//--TODO：串口回传ok
//		cap.open(0);
//		if (!cap.isOpened()) { cout << "Can`t open camera\n"; return -1; }
//		cap >> srcImage;
//		cap.release();
//		if (srcImage.empty()) return -1;
//		get_color(srcImage, color_this_face);
//		for (int i = 0; i < 9; i++)
//		{
//			Start_color[i + 9 * face] = color_this_face[i];
//		}
//	}
//
//	cout << "Please go back to the start position and I`m going to solve it." << endl;
//	//--TODO：串口发送下位机转动命令
//	//--TODO：串口回传ok
//
//	deviation(Start_color);
//	standardize_color(Start_color, Start_cube);
//	mySolveCube(Start_cube);
//	//move();
//	
//	return 0;
//
//}

