#include"function_declare.h"

/**
* @function get_colour
*/
int get_colour(Mat & srcImage)
{
		while (true)
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
			////开操作 (去除一些噪点)
			//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
			//morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

			////闭操作 (连接一些连通域)
			//morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);

			namedWindow("[1]Cube", CV_WINDOW_AUTOSIZE);
			imshow("[1]Cube", srcImage);

			namedWindow("[2]W_Thresholded", CV_WINDOW_AUTOSIZE);
			imshow("[2]W_Thresholded", W_Thresholded);
			namedWindow("[3]R_Thresholded", CV_WINDOW_AUTOSIZE);
			imshow("[3]R_Thresholded", R_Thresholded);
			namedWindow("[4]G_Thresholded", CV_WINDOW_AUTOSIZE);
			imshow("[4]G_Thresholded", G_Thresholded);
			namedWindow("[5]B_Thresholded", CV_WINDOW_AUTOSIZE);
			imshow("[5]B_Thresholded", B_Thresholded);
			namedWindow("[6]O_Thresholded", CV_WINDOW_AUTOSIZE);
			imshow("[6]O_Thresholded", O_Thresholded);
			namedWindow("[7]Y_Thresholded", CV_WINDOW_AUTOSIZE);
			imshow("[7]Y_Thresholded", Y_Thresholded);


			char key = (char)waitKey(300);
			if (key == 27)
				break;
		}

		return 0;

}