#include"function_declare.h"

Mat getROI(Mat image, Rect rect)
{
	Mat img = image.clone();
	Mat roi;
	int cols = img.cols, rows = img.rows;
	//ROI越界，返回
	if (cols - 1 - rect.x < rect.width || rows - 1 - rect.y < rect.height) 
		{	return image;	}
	roi = img(Rect(rect.x, rect.y, rect.width, rect.height));
	rectangle(img, rect, Scalar(0, 0, 255), 2);
	imshow("SignROI", img);
	image.copyTo(img);    //ROI和它的父图像指向同一块缓冲区，经次操作消除 标记ROI的矩形框
	imshow("ROI", roi);
	return roi;
}


int Sums(Mat& src)
{
	int counter = 0;
	//迭代器访问像素点  
	Mat_<uchar>::iterator it = src.begin<uchar>();
	Mat_<uchar>::iterator itend = src.end<uchar>();
	for (; it != itend; ++it)
	{
		if ((*it)>0)
			counter += 1;
	}
	return counter;
}

/*********************************************************************
* @return: color_this_face[9]
*
*		    position->|0 |1 |2 |
*					  |3 |4 |5 |
*                     |6 |7 |8 |
*           meaning-> 0 1 2 3 4 5
*                     W R G B O Y     (Actually,it doesn`t matter.)
*             	
************************************************************************/
/**
* @function get_color
*/
void get_color(Mat& srcImage, int color_this_face[9])
{
	Mat imgHSV;
	vector<Mat> hsvSplit;
	cvtColor(srcImage, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	//在HSV空间做直方图均衡化
	split(imgHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, imgHSV);

	//Threshold the image
	Mat W_Thresholded;
	Mat R_Thresholded;
	Mat G_Thresholded;
	Mat B_Thresholded;
	Mat O_Thresholded;
	Mat Y_Thresholded;
	inRange(imgHSV, Scalar(W_LowH, W_LowS, W_LowV), Scalar(W_HighH, W_HighS, W_HighV), W_Thresholded);
	inRange(imgHSV, Scalar(R_LowH, R_LowS, R_LowV), Scalar(R_HighH, R_HighS, R_HighV), R_Thresholded);
	inRange(imgHSV, Scalar(G_LowH, G_LowS, G_LowV), Scalar(G_HighH, G_HighS, G_HighV), G_Thresholded);
	inRange(imgHSV, Scalar(B_LowH, B_LowS, B_LowV), Scalar(B_HighH, B_HighS, B_HighV), B_Thresholded);
	inRange(imgHSV, Scalar(O_LowH, O_LowS, O_LowV), Scalar(O_HighH, O_HighS, O_HighV), O_Thresholded);
	inRange(imgHSV, Scalar(Y_LowH, Y_LowS, Y_LowV), Scalar(Y_HighH, Y_HighS, Y_HighV), Y_Thresholded);

	////开操作 (去除噪点)
	//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	//morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
	////闭操作 (连接连通域)
	//morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);

	vector<Mat> color_thre;
	color_thre.push_back(W_Thresholded);
	color_thre.push_back(R_Thresholded);
	color_thre.push_back(G_Thresholded);
	color_thre.push_back(B_Thresholded);
	color_thre.push_back(O_Thresholded);
	color_thre.push_back(Y_Thresholded);

	///show me the result
	namedWindow("[1]Cube", WINDOW_NORMAL);
	imshow("[1]Cube", srcImage);
	namedWindow("[2]W_Thresholded", WINDOW_NORMAL);
	imshow("[2]W_Thresholded", W_Thresholded);
	namedWindow("[3]R_Thresholded", WINDOW_NORMAL);
	imshow("[3]R_Thresholded", R_Thresholded);
	namedWindow("[4]G_Thresholded", WINDOW_NORMAL);
	imshow("[4]G_Thresholded", G_Thresholded);
	namedWindow("[5]B_Thresholded", WINDOW_NORMAL);
	imshow("[5]B_Thresholded", B_Thresholded);
	namedWindow("[6]O_Thresholded", WINDOW_NORMAL);
	imshow("[6]O_Thresholded", O_Thresholded);
	namedWindow("[7]Y_Thresholded", WINDOW_NORMAL);
	imshow("[7]Y_Thresholded", Y_Thresholded);

	///提取感兴趣区域，9块，并判断黑白
	//----------------------------------------
	//TODO:这几个参数视图片大小来调整,与摄像头安装距离有关
	//----------------------------------------
	int width = 50;
	int x = 20;//x轴水平
	int y = 20;
	int thresholded = 200; 

	for (int colour = 0; colour < 6; colour++)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Mat roi = getROI(color_thre[colour], Rect((j + 1)*x, (i + 1)*y, 50, 50));//对一种颜色的二值图提取感兴趣区域
				int isColor = Sums(roi);//调用函数Sums  
				cout<<"This face"<<" "<<colorlet[colour]<<" "<<"thresholded cubie NO."<< 3 * i + j << " " << isColor << endl;
				if (isColor > thresholded)
					color_this_face[3 * i + j] = colour;
			}
		}
	}
	
	waitKey(0);
	destroyAllWindows();

}
