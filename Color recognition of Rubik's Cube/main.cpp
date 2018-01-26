//*=====================================================================================
//*                          main.c--
//*	             Copyleft! 2018 William Yu
//*          Some rights reserved：CC(creativecommons.org)BY-NC-SA
//*              Copyleft! 2018 William Yu
//*      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
//*
//* Filename                : main.c
//* Description             : (编译环境：win10 + OpenCV3.3.0 + VS2017)
//* Reference               :
//* Programmer(s)           : William Yu, windmillyucong@163.com
//* Company                 : HUST ThinkFly Studio
//* Modification History	  : ver1.0, 2018.01.20, William Yu
//*
//=====================================================================================*/

#include"function_declare.h"
/**
* @function main
*/
void main()
{
	Mat srcImage = imread("Cube.jpg");
	check_value(srcImage);
	//get_colour(srcImage);

	waitKey(0);
	destroyAllWindows();
}

