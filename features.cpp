#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "highgui.h"
#include<iostream>
#include<fstream>

using namespace cv;
using namespace std;
# define db double

void getroi(Rect roi[7],int r,int c);
db getspfreq(Mat I);
void GLCM(Mat src,Mat dest,int d);
db getcontrast(Mat I);
db getentropy(Mat I);
Mat cropimg(Mat src);

int main( int argc, char** argv )
{
	Mat src,lab,dest;
	Rect roi[7];
	int r,c;
    ofstream fout("feature.txt",ios::out);

	Scalar avg,stddev,av,st;
	Scalar mu[7],dev[7],skew[7],kurt[7],var[7],spfreq[7],contrast[7],entropy[7];
 	
    //Read in image and crop and resize it
    src= imread( argv[1], 1 );
    src=cropimg(src);
    resize(src,src,Size(650,560));
    
    //Splitting into 7 regions of interest
    r=src.rows; c=src.cols;
    getroi(roi,r,c);

    //Converting to CIE*Lab space
 	cvtColor( src, dest, CV_BGR2Lab );
 	
    for(int j=0;j<7;j++){
 		lab=dest(roi[j]);

        //Calculating mean and standard deviation
 		meanStdDev(lab,mu[j],dev[j]);
 		
        //Splitting into the three channels L,a,b
        Mat channel[3];
 		split(lab,channel);
 		
        //Calculating the skew for each of the three channels L,a,b 
        Mat x3; pow(lab,3,x3);
 		meanStdDev(x3,avg,stddev);
 		for(int i=0;i<3;i++)
 			skew[j][i]=(avg[i]-3*mu[j][i]*dev[j][i]*dev[j][i]-mu[j][i]*mu[j][i]*mu[j][i])/(dev[j][i]*dev[j][i]*dev[j][i]);
 		
        Mat x4; pow(lab,4,x4);
 		meanStdDev(x4,av,st);
 		
        //Calculating the kurtosis for each of the three channels L,a,b 
        for(int i=0;i<3;i++)
 			kurt[j][i]=(av[i]-4*mu[j][i]*avg[i]+6*mu[j][i]*mu[j][i]*dev[j][i]*dev[j][i]+3*mu[j][i]*mu[j][i]*mu[j][i]*mu[j][i])/(dev[j][i]*dev[j][i]*dev[j][i]*dev[j][i]);
 		
        //Calculating the variance for each of the three channels L,a,b 
        for(int i=0;i<3;i++)
 			var[j][i]=dev[j][i]*dev[j][i];
 		
        //Calculating the spatial frequency for each of the three channels L,a,b 
        for(int i=0;i<3;i++){
 			spfreq[j][i]=getspfreq(channel[i]);
 		}

        //Calculating the contrast and entropy(Haralick features) for each of the three channels L,a,b 
        for(int i=0;i<3;i++){
            Mat frame=Mat::zeros(256,256,CV_64FC1);
            GLCM(channel[i],frame,1);
            contrast[j][i]=getcontrast(frame);
            entropy[j][i]=getentropy(frame);
        }

        //Writing the feature vector into a text file
 		fout<<mu[j][0]<<"\n"<<mu[j][1]<<"\n"<<mu[j][2]<<"\n";
 		fout<<dev[j][0]<<"\n"<<dev[j][1]<<"\n"<<dev[j][2]<<"\n";
 		fout<<skew[j][0]<<"\n"<<skew[j][1]<<"\n"<<skew[j][2]<<"\n";
 		fout<<kurt[j][0]<<"\n"<<kurt[j][1]<<"\n"<<kurt[j][2]<<"\n";
 		fout<<var[j][0]<<"\n"<<var[j][1]<<"\n"<<var[j][2]<<"\n";
 		fout<<spfreq[j][0]<<"\n"<<spfreq[j][1]<<"\n"<<spfreq[j][2]<<"\n";
        fout<<contrast[j][0]<<"\n"<<contrast[j][1]<<"\n"<<contrast[j][2]<<"\n";
        fout<<entropy[j][0]<<"\n"<<entropy[j][1]<<"\n"<<entropy[j][2]<<"\n"; 
    }

    fout.close();
 	return 0;
}

bool cmpare(std::vector<cv::Point> x,std::vector<cv::Point> y){
    return(fabs(contourArea(Mat(x)))>fabs(contourArea(Mat(y))));
}

Mat cropimg(Mat src){//Crops image to remove the black background in the retinal image
    Mat img=src.clone();
    cvtColor(img,img,CV_BGR2GRAY);
    threshold(img,img,10,255,CV_THRESH_BINARY);
    GaussianBlur(img,img,Size(21,21),0,0);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(img,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);
    std::sort(contours.begin(),contours.end(),cmpare);
    Mat dest=src(boundingRect(contours[0]));
    return dest;
}

void getroi(Rect roi[7],int r,int c){//Splitting the retinal image into 7 regions
	roi[0]=Rect(Point(c/5,r/8),Point(c/2,(3*r/8)));
 	roi[1]=Rect(Point(c/2,r/8),Point(4*c/5,3*r/8));
 	roi[2]=Rect(Point(c/20,3*r/8),Point(7*c/20,5*r/8));
 	roi[3]=Rect(Point(7*c/20,3*r/8),Point(13*c/20,5*r/8));
 	roi[4]=Rect(Point(13*c/20,3*r/8),Point(19*c/20,5*r/8));
 	roi[5]=Rect(Point(c/5,5*r/8),Point(c/2,7*r/8));
 	roi[6]=Rect(Point(c/2,5*r/8),Point(4*c/5,7*r/8));
}

db getspfreq(Mat I){//Obtaining the value of spatial frequency
	Mat padded;                            
	int m = getOptimalDFTSize( I.rows );
	int n = getOptimalDFTSize( I.cols ); 
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = {Mat_<db>(padded), Mat::zeros(padded.size(), CV_64F)};
	Mat complexI;
	merge(planes, 2, complexI);         
	dft(complexI, complexI);            
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols/2;
	int cy = magI.rows/2;
	Mat q0(magI, Rect(0, 0, cx, cy));   
	Mat q1(magI, Rect(cx, 0, cx, cy));  
	Mat q2(magI, Rect(0, cy, cx, cy));  
	Mat q3(magI, Rect(cx, cy, cx, cy)); 
	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
    tmp.copyTo(q2);
    db spfreq=0,tot=0;
    for(int i=0;i<magI.rows;i++){
        for(int j=0;j<magI.cols;j++){
            spfreq+=magI.at<db>(i, j)*fabs(i-(magI.rows/2))+magI.at<db>(i, j)*fabs(j-(magI.cols/2));
            tot+=2*magI.at<db>(i, j);
        }
    }
    spfreq=spfreq/tot;
    return spfreq;

}

void GLCM(Mat I,Mat res,int d){//To obtain the GLCM (Gray Level Co-occurence Matrix)
    int p,q;
    for(int i=0;i<I.rows;i++){
        for(int j=d;j<(I.cols-d);j++){
            p=I.at<uchar>(i, j);
            q=I.at<uchar>(i, j+d); 
            res.at<db>(p, q)+=1;
        }
    }
    res=res+res.t();
    normalize(res,res,NORM_L2);
}

db getcontrast(Mat I){//Obtaining the value of contrast(Haralick feature)
    db ans=0;
    for(int i=0;i<I.rows;i++)
        for(int j=0;j<I.cols;j++)
            ans+=(i-j)*(i-j)*I.at<db>(i, j);
    return ans;
}	

db getentropy(Mat I){//Obtaining the value of entropy(Haralick feature)
    db ans=0;
    for(int i=0;i<I.rows;i++)
        for(int j=0;j<I.cols;j++)
            if(I.at<db>(i, j)) 
                ans+=I.at<db>(i, j)*log(I.at<db>(i, j));
    return (-ans);
}