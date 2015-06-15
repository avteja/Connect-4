#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
#define RED1L 150
#define RED1H 179
#define RED2L 0
#define RED2H 10
#define YELLOWL 20
#define YELLOWH 38
#define REDSATL 100
#define SATH 255
#define YELLOWSATL 100
#define VALL 120
#define VALH 255

using namespace cv;
using namespace std;

struct CartesianPoints
{
    double x;
    double y;

    CartesianPoints()
    {
        x=0;
        y=0;
    }
    CartesianPoints(double X, double Y)
    {
        x=X;
        y=Y;
    }
};


void edgeDetect(Mat EdgeImage, Mat InImage)
{
    unsigned char* input= (unsigned char*)InImage.data;
    unsigned char* output= (unsigned char*)EdgeImage.data;
    for(int i=0 ;i<InImage.rows; i++)
    {
        for(int j=0; j<InImage.cols; j++)
        {
            if(input[i*InImage.cols+j]>50&&input[i*InImage.cols + j]<150)
                output[i*InImage.cols+j]=255;
            else
                output[i*InImage.cols+j]=0;
        }
    }
}

void locateChip( vector<CartesianPoints> allPoints, vector<CartesianPoints> &chipCentres )
{
    long double sumX=0, sumY=0, countC=0;
    vector<CartesianPoints>::iterator it;

    for(it=allPoints.begin(); it<allPoints.end(); it++ )
    {
        sumX+= it->x*10E-4;
        sumY+= it->y*10E-4;
        countC++;
    }
    CartesianPoints aChipCentre(sumX*1000/countC, sumY*1000/countC);
    chipCentres.push_back(aChipCentre);
}

void findOneCompleteChip(Mat Image, int i, int j, vector<CartesianPoints> &allPoints )
{
    unsigned char* output= (unsigned char*) Image.data;

    if( output[i*Image.cols + j]==255)
    {
        CartesianPoints CPTemp(j,i);
        allPoints.push_back(CPTemp);
        output[i*Image.cols+j]=0;
        for(int m=i-1; m<=i+1; m++)
        {
            for(int n=j-1; n<=j+1; n++)
            {
                if( output[m*Image.cols + n]==255&&m>=0&&m<Image.rows&&n>=0&&n<Image.cols)
                {
                    findOneCompleteChip(Image, m, n, allPoints);
                }
            }
        }
    }
}

void findAllChipCentres(Mat Image, vector<CartesianPoints> &chipCentres)
{
    unsigned char*output=(unsigned char*)Image.data;
    vector<CartesianPoints> allPoints;

    for(int i=0; i<Image.rows; i++)
    {
        for(int j=0; j<Image.cols; j++)
        {
            if(output[i*Image.cols+j]==255)
            {
                findOneCompleteChip(Image, i, j, allPoints);               // store all points representing a chip in allPoints
                locateChip(allPoints,chipCentres);                         // to find centre of chip and store it in chipCentres
                allPoints.clear();                                         // clear allPoints vector for next chip
            }

        }
    }
}


// void digitaliseImage(Mat Image, Mat RYImage[2])
// {
//     unsigned char* input= (unsigned char*)Image.data;
//     unsigned char* redoutput;
//     unsigned char* yeloutput;


//     //for(int i=0; i<2; i++)
//     //{
//     //    output[i]= (unsigned char*)RYImage[i].data;
//     //}
//      redoutput= (unsigned char*)RYImage[0].data;

//      redoutput[0]=0;
//      yeloutput= (unsigned char*)RYImage[1].data;

//     for(int i=0; i<Image.rows; i++)
//     {
//         for(int j=0; j<Image.cols; j++)
//         {

//             if((input[i*(Image.cols)+j]<REDH )&& (input[i*(Image.cols)+j]>REDL))
//               {  redoutput[i*Image.cols+j]=255;}
//             else
//                { redoutput[i*Image.cols+j]=0;   }
//                // thresholding to store the points corresponding to Red chips

//             if(input[i*Image.cols+j]<YELLOWH && input[i*Image.cols+j]>YELLOWL)
//                 yeloutput[i*Image.cols+j]=255;
//             else
//                 yeloutput[i*Image.cols+j]=0;                                           //thresholding to store all the points corresponding to Yellow chips
//         }
//     }
// }
