//#include <stdio.h>  
//#include "SerialPort.h"
//#include <windows.h>
//#include <iostream>
//using namespace std;
//#define TXPackSize 100
//
//void main()
//{
//	CSerialPort s1;			//串口初始化，只检测串口9以内
//
//	for (unsigned int i = 0; i <= 10; i++)
//	{
//		if (s1.InitPort(i))
//		{
//			std::cout << "init port" << i << " successfully\n" << std::endl;
//			break;
//		}
//		if (i >= 10)
//		{
//			std::cout << "error in InitPort\n" << std::endl;
//			system("pause");
//			//return ;
//		}
//	}
//	unsigned char sendBuffer[TXPackSize];
//	sendBuffer[0] = '!';
//	sendBuffer[TXPackSize - 1] = '#';
//	sendBuffer[TXPackSize - 2] = 0;
//	   
//}