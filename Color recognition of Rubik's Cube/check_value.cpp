#include"function_declare.h"

/**
* @function check_value
*/
int check_value(Mat& srcImage)
{
	namedWindow("Control", WINDOW_NORMAL); //create a window called "Control"
	int iLowH = 100;
	int iHighH = 140;
	int iLowS = 90;
	int iHighS = 255;
	int iLowV = 90;
	int iHighV = 255;
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);
	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);
	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	while (true)
	{
		Mat imgHSV;
		vector<Mat> hsvSplit;
		cvtColor(srcImage, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		//在HSV空间做直方图均衡化
		split(imgHSV, hsvSplit);
		equalizeHist(hsvSplit[2], hsvSplit[2]);
		merge(hsvSplit, imgHSV);

		Mat imgThresholded;
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

		//开操作 (去噪点)
		Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
		morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
		//闭操作 (连接连通域)
		morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);

		namedWindow("[1]Cube", WINDOW_NORMAL);
		imshow("[1]Cube", srcImage);
		moveWindow("[1]Cube", 20, 20);
		namedWindow("[2]Thresholded_Image", WINDOW_NORMAL);
		imshow("[2]Thresholded_Image", imgThresholded);
		moveWindow("[2]Thresholded_Image", 300, 20);

		char key = (char)waitKey(300);
		if (key == 27)
			break;
	}

	return 0;

}