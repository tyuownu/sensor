#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <XnCppWrapper.h>


int main()
{
	xn::Context xContext;
	xContext.Init();

	xn::DepthGenerator xDepth;
	xDepth.Create(xContext);

	xn::ImageGenerator xImage;
	xImage.Create(xContext);
/*
 *
 *    xn::IRGenerator xIR;
 *    xIR.Create(xContext);
 *
 */

	xDepth.GetAlternativeViewPointCap().SetViewPoint(xImage);

//	cv::namedWindow("Depth Image",CV_WINDOW_AUTOSIZE);
//	cv::namedWindow("Color Image",CV_WINDOW_AUTOSIZE);
//	cv::namedWindow("Depth Edge",CV_WINDOW_AUTOSIZE);
//	cv::namedWindow("Color Edge",CV_WINDOW_AUTOSIZE);

	xContext.StartGeneratingAll();
	int i = 0;
	char a1[50],a2[50];
	cv::Mat cBGRImg;
	cv::Mat c8BitDepth;
	XnMapOutputMode mapMode;
	mapMode.nXRes = 640;
	mapMode.nYRes = 480;
	mapMode.nFPS = 30;
	xDepth.SetMapOutputMode(mapMode);
	xImage.SetMapOutputMode(mapMode);
	//xIR.SetMapOutputMode(mapMode);
	while(i<500)
	{
		std::cout<<i<<std::endl;
		xContext.WaitAndUpdateAll();
		{
			xn::ImageMetaData xColorData;
			xImage.GetMetaData(xColorData);
			cv::Mat cColorImg(xColorData.FullYRes(),xColorData.FullXRes(),
					CV_8UC3,(void*)xColorData.Data());

			cvtColor(cColorImg,cBGRImg,CV_RGB2BGR);
			cv::imshow("Color Image",cBGRImg);}

/*
 *
 *            xn::IRMetaData xIRData;
 *            xIR.GetMetaData(xIRData);
 *            cv::Mat ir16(xIRData.FullYRes(),xIRData.FullXRes(),
 *                    CV_16SC1, (unsigned short*)xIRData.WritableData());
 *
 *            cv::imshow("IR image",ir16);
 *
 */

/*			cv::Mat cColorEdge;
			cvtColor(cColorImg,cBGRImg,CV_RGB2GRAY);
			cv::Canny(cBGRImg,cColorEdge,5,100);
			cv::imshow("Color Edge",cColorEdge);*/
		


			xn::DepthMetaData xDepthData;
			xDepth.GetMetaData(xDepthData);

			cv::Mat cDepthImg(xDepthData.FullYRes(),xDepthData.FullXRes(),
					CV_16UC1,(void*)xDepthData.Data());
		
			int max = cDepthImg.at<unsigned short>(0,0) ;
			for(int ii = 0; ii < cDepthImg.rows;ii++)
				for(int j = 0;j < cDepthImg.cols;j++)
					if(cDepthImg.at<unsigned short>(ii,j) > max)
						max = cDepthImg.at<unsigned short>(ii,j);

			std::cout<<max<<std::endl;
			
//			cDepthImg.convertTo(c8BitDepth,CV_8U,255.0/7000);
			cv::imshow("Depth Image",cDepthImg);

/*			cv::Mat CDepthEdge;
			cv::Canny(c8BitDepth,CDepthEdge,5,100);
			cv::imshow("Depth Edge",CDepthEdge);*/

		sprintf(a1,"/home/tyu/allwork/sensor/rgb/image%d.png",i);
		sprintf(a2,"/home/tyu/allwork/sensor/depth/depth%d.png",i++);
			/*
			 *cv::imwrite("/home/tyu/bagfiles/tools/image200.png",cBGRImg);
			 *cv::imwrite("/home/tyu/bagfiles/tools/depth200.png",cDepthImg);
			 */
/*
 *        cv::imwrite(a1,cBGRImg);
 *        cv::imwrite(a2,cDepthImg);
 *
 */

//		 imwrite RGB image
		cv::imwrite(a1,cBGRImg);

//		imwrite ir image
//		cv::imwrite(a1,ir16);

//		imwrite depth image
		cv::imwrite(a2,cDepthImg);
		cv::waitKey(10);
	}
	xContext.StopGeneratingAll();
	xContext.Release();

	return 0;
}
