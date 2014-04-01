/**
EE569 HW #2 Problem 1: Special Effect Image Filters
Author: Xiang Gao
USCID:  9216348910
Email:  xianggao@usc.edu
Submission Date:
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "MyIO.h"
#include "MyFiltering.h"
#include "MyDIPFunction.h"
#include <math.h>

using namespace std;

// Part (a): Color to Gray-Scale Conversion
void processA()
{
    printf("Process A begins...\n");

    int SizeI = 256;
    int SizeJ = 256;
    unsigned char *ImageData = (unsigned char*)malloc(sizeof(unsigned char)*SizeI*SizeJ*3);
    unsigned char *ImageDataOut = (unsigned char*)malloc(sizeof(unsigned char)*SizeI*SizeJ);

    string inputFile = "naruto";
    string filename = inputFile;
    filename.append(".raw");
    ReadImageFromFile(filename.c_str(),ImageData,SizeI,SizeJ,3);

    Color2Gray(ImageData,ImageDataOut,SizeI,SizeJ);

    filename = inputFile;
    filename.append("_gray_out.raw");
    WriteImageToFile(filename.c_str(),ImageDataOut,SizeI,SizeJ,1);

    free(ImageData);
    free(ImageDataOut);

    printf("Process A ends...\n");
}

// Part (b): Pencil Sketch using edge detectors
void processB()
{
    printf("Process B begins...\n");

    int SizeI = 256;
    int SizeJ = 256;
    unsigned char *ColorImage = new unsigned char[SizeI*SizeJ*3];
    unsigned char *GrayImage = new unsigned char[SizeI*SizeJ];
    unsigned char *DenoisedGrayImage = new unsigned char[SizeI*SizeJ];
    unsigned char *EdgeImage = new unsigned char[SizeI*SizeJ];
    string inputFile = "harrypotter";

    string filename = inputFile;
    filename.append(".raw");
    ReadImageFromFile(filename.c_str(),ColorImage,SizeI,SizeJ,3);
    Color2Gray(ColorImage,GrayImage,SizeI,SizeJ);

    // Pre-Denoising
    //GaussianFilter(7,0.95,GrayImage,DenoisedGrayImage,SizeI,SizeJ);
    //NLMFilter(7,21,150,GrayImage,DenoisedGrayImage,SizeI,SizeJ);

    // 1st order Edge Detection
    EdgeDetectionGradient(GrayImage,EdgeImage,SizeI,SizeJ,0,12); //0: Pixel difference; 1: Roberts; 2: Sobel;
    filename = inputFile;
    filename.append("_ed_1st_out.raw");
    WriteImageToFile(filename.c_str(),EdgeImage,SizeI,SizeJ,1);

    // 2nd Order Edge Detection
    EdgeDetectionLaplacian(GrayImage,EdgeImage,SizeI,SizeJ,15); // using laplacian
    filename = inputFile;
    filename.append("_ed_2nd_out.raw");
    WriteImageToFile(filename.c_str(),EdgeImage,SizeI,SizeJ,2);

    // DOG Edge Detection
    double sigma = 2.0;
    double k = 1.6;
    int FilterSize = 7;
    EdgeDetectionDOG(sigma,k,FilterSize,GrayImage,EdgeImage,SizeI,SizeJ,2); //
    filename = inputFile;
    filename.append("_ed_dog_out.raw");
    WriteImageToFile(filename.c_str(),EdgeImage,SizeI,SizeJ,2);

    delete ColorImage;
    delete GrayImage;
    delete DenoisedGrayImage;
    delete EdgeImage;
    printf("Process B ends...\n");
}

void processC()
{
    printf("Process C begins...\n");

    int SizeI = 256;
    int SizeJ = 256;
    unsigned char *BackgroundImage = new unsigned char[SizeI*SizeJ];
    unsigned char *EdgeImage = new unsigned char[SizeI*SizeJ];
    unsigned char *OutputImage = new unsigned char[SizeI*SizeJ];

    string inputFile = "lena_noisy";
    string filename;
    // Read background image
    filename = "grainy.raw";
    ReadImageFromFile(filename.c_str(),BackgroundImage,SizeI,SizeJ,1);
    // Read edge image
    filename = inputFile;
    filename.append("_ed_best_out.raw"); // Following this naming standard
    ReadImageFromFile(filename.c_str(),EdgeImage,SizeI,SizeJ,1);

    // Background Effect
    double alpha = -0.95;
    double beta = 30;
    for(int i=0;i<SizeI;i++)
    {
        for(int j=0;j<SizeJ;j++)
        {
            double tmp = EdgeImage[i*SizeJ+j] + alpha*BackgroundImage[i*SizeJ+j] + beta;
            if(tmp > 255)
                tmp = 255;
            if(tmp < 0)
                tmp = 0;
            OutputImage[i*SizeJ+j] = tmp;
        }
    }

    filename = inputFile;
    filename.append("_bgeffect_out.raw");
    WriteImageToFile(filename.c_str(),OutputImage,SizeI,SizeJ,1);

    delete BackgroundImage;
    delete EdgeImage;
    delete OutputImage;
    printf("Process C ends...\n");
}

void processD()
{
    printf("Process D begins...\n");

    int SizeI = 256;
    int SizeJ = 256;
    unsigned char *ColorImage = new unsigned char[SizeI*SizeJ*3];
    unsigned char *EdgeImage = new unsigned char[SizeI*SizeJ];
    unsigned char *OutputImage = new unsigned char[SizeI*SizeJ*3];

    string inputFile = "lena_noisy";
    string filename;
    // Read color image
    filename = inputFile;
    filename.append(".raw");
    ReadImageFromFile(filename.c_str(),ColorImage,SizeI,SizeJ,3);
    // Read edge image
    filename = inputFile;
    filename.append("_ed_best_out.raw"); // Following this naming standard
    ReadImageFromFile(filename.c_str(),EdgeImage,SizeI,SizeJ,1);

    char tmp[50];

    // Sliding Effect
    // (1) Left to Right

    for(int frame=0,filecounter=0;frame<=SizeJ;frame+=8,filecounter++)
    {
        for(int i=0;i<SizeI;i++)
        {
            for(int j=0;j<frame;j++)
            {
                OutputImage[3*(i*SizeJ+j)] = ColorImage[3*(i*SizeJ+j)];
                OutputImage[3*(i*SizeJ+j)+1] = ColorImage[3*(i*SizeJ+j)+1];
                OutputImage[3*(i*SizeJ+j)+2] = ColorImage[3*(i*SizeJ+j)+2];
            }
            for(int j=frame;j<SizeJ;j++)
            {
                OutputImage[3*(i*SizeJ+j)] = EdgeImage[i*SizeJ+j];
                OutputImage[3*(i*SizeJ+j)+1] = EdgeImage[i*SizeJ+j];
                OutputImage[3*(i*SizeJ+j)+2] = EdgeImage[i*SizeJ+j];
            }
        }
        filename = inputFile;
        sprintf(tmp,"_l2r_%d_out.raw",filecounter);
        filename.append(tmp);
        WriteImageToFile(filename.c_str(),OutputImage,SizeI,SizeJ,3);
    }
    // (2) Left-top to Right-bottom
    for(int frame=0,filecounter=0;frame<=2*SizeJ;frame+=8,filecounter++)
    {
        for(int i=0;i<SizeI;i++)
        {
            for(int j=0;j<SizeJ;j++)
            {
                if(i+j<=frame)
                {
                    OutputImage[3*(i*SizeJ+j)] = ColorImage[3*(i*SizeJ+j)];
                    OutputImage[3*(i*SizeJ+j)+1] = ColorImage[3*(i*SizeJ+j)+1];
                    OutputImage[3*(i*SizeJ+j)+2] = ColorImage[3*(i*SizeJ+j)+2];
                }
                else
                {
                    OutputImage[3*(i*SizeJ+j)] = EdgeImage[i*SizeJ+j];
                    OutputImage[3*(i*SizeJ+j)+1] = EdgeImage[i*SizeJ+j];
                    OutputImage[3*(i*SizeJ+j)+2] = EdgeImage[i*SizeJ+j];
                }
            }
        }
        filename = inputFile;
        sprintf(tmp,"_lt2rb_%d_out.raw",filecounter);
        filename.append(tmp);
        WriteImageToFile(filename.c_str(),OutputImage,SizeI,SizeJ,3);
    }
    // Fade-in Effect
    int filecounter = 0;
    for(float alpha=-0.5;alpha<=0.5;alpha+=0.01,filecounter++)
    {
        for(int i=0;i<SizeI;i++)
        {
            for(int j=0;j<SizeJ;j++)
            {
                OutputImage[3*(i*SizeJ+j)] = (0.5-alpha)*ColorImage[3*(i*SizeJ+j)]+(0.5+alpha)*EdgeImage[i*SizeJ+j];
                OutputImage[3*(i*SizeJ+j)+1] = (0.5-alpha)*ColorImage[3*(i*SizeJ+j)+1]+(0.5+alpha)*EdgeImage[i*SizeJ+j];
                OutputImage[3*(i*SizeJ+j)+2] = (0.5-alpha)*ColorImage[3*(i*SizeJ+j)+2]+(0.5+alpha)*EdgeImage[i*SizeJ+j];
            }
        }
        filename = inputFile;
        sprintf(tmp,"_fadein_%d_out.raw",filecounter);
        filename.append(tmp);
        WriteImageToFile(filename.c_str(),OutputImage,SizeI,SizeJ,3);
    }
    delete ColorImage;
    delete EdgeImage;
    delete OutputImage;
    printf("Process D ends...\n");
}

void processE()
{
    printf("Process E begins...\n");

    int SizeI = 256;
    int SizeJ = 256;
    unsigned char *ColorImage = new unsigned char[SizeI*SizeJ*3];
    unsigned char *ChannelR = new unsigned char[SizeI*SizeJ];
    unsigned char *ChannelG = new unsigned char[SizeI*SizeJ];
    unsigned char *ChannelB = new unsigned char[SizeI*SizeJ];
    unsigned char *GrayImage = new unsigned char[SizeI*SizeJ];
    unsigned char *Y = new unsigned char[SizeI*SizeJ];
    int *U = new int[SizeI*SizeJ];
    int *V = new int[SizeI*SizeJ];

    unsigned char *EdgeImageR = new unsigned char[SizeI*SizeJ];
    unsigned char *EdgeImageG = new unsigned char[SizeI*SizeJ];
    unsigned char *EdgeImageB = new unsigned char[SizeI*SizeJ];
    unsigned char *EdgeImage = new unsigned char[SizeI*SizeJ];

    unsigned char *OutputImage = new unsigned char[SizeI*SizeJ*3];
    string inputFile = "lena_noisy";
    string filename;

    filename = inputFile;
    filename.append(".raw");
    ReadImageFromFile(filename.c_str(),ColorImage,SizeI,SizeJ,3);

    /*
    // Method 1: Edge detection to RGB channels
    SplitRGB(ColorImage,ChannelR,ChannelG,ChannelB,SizeI,SizeJ);
    EdgeDetectionGradient(ChannelR,EdgeImageR,SizeI,SizeJ,0,13); //0: Pixel difference; 1: Roberts; 2: Sobel;
    EdgeDetectionGradient(ChannelG,EdgeImageG,SizeI,SizeJ,0,13); //0: Pixel difference; 1: Roberts; 2: Sobel;
    EdgeDetectionGradient(ChannelB,EdgeImageB,SizeI,SizeJ,0,13); //0: Pixel difference; 1: Roberts; 2: Sobel;
    for(int i=0;i<SizeI;i++)
    {
        for(int j=0;j<SizeJ;j++)
        {
            OutputImage[3*(i*SizeJ+j)] = EdgeImageR[i*SizeJ+j];
            OutputImage[3*(i*SizeJ+j)+1] = EdgeImageG[i*SizeJ+j];
            OutputImage[3*(i*SizeJ+j)+2] = EdgeImageB[i*SizeJ+j];
        }
    }
    */

    // Method 2: Blending Method:

    filename = inputFile;
    filename.append("_ed_best_out.raw");
    ReadImageFromFile(filename.c_str(),EdgeImage,SizeI,SizeJ,1);
    for(int i=0;i<SizeI;i++)
    {
        for(int j=0;j<SizeJ;j++)
        {
            double a = 1.0*EdgeImage[i*SizeJ+j]/255;
            double b1 = 1.0*ColorImage[3*(i*SizeJ+j)]/255;
            double b2 = 1.0*ColorImage[3*(i*SizeJ+j)+1]/255;
            double b3 = 1.0*ColorImage[3*(i*SizeJ+j)+2]/255;

            if(a<0.5)
            {   OutputImage[3*(i*SizeJ+j)] = 255*(2*a*b1);
                OutputImage[3*(i*SizeJ+j)+1] = 255*(2*a*b2);
                OutputImage[3*(i*SizeJ+j)+2] = 255*(2*a*b3);
            }
            else
            {
                OutputImage[3*(i*SizeJ+j)] = 255*(1-2*(1-a)*(1-b1));
                OutputImage[3*(i*SizeJ+j)+1] = 255*(1-2*(1-a)*(1-b2));
                OutputImage[3*(i*SizeJ+j)+2] = 255*(1-2*(1-a)*(1-b3));
            }
        }
    }


    filename = inputFile;
    filename.append("_color_ed_out.raw");
    WriteImageToFile(filename.c_str(),OutputImage,SizeI,SizeJ,3);

    delete ColorImage;
    delete ChannelR;
    delete ChannelG;
    delete ChannelB;
    delete GrayImage;
    delete EdgeImageR;
    delete EdgeImageG;
    delete EdgeImageB;
    delete Y;
    delete U;
    delete V;
    delete EdgeImage;
    delete OutputImage;
    printf("Process E ends...\n");
}

void test()
{
    int SizeI = 256;
    int SizeJ = 256;
    unsigned char *ColorImage = new unsigned char[SizeI*SizeJ*3];
    unsigned char *Y = new unsigned char[SizeI*SizeJ];
    int *U = new int[SizeI*SizeJ];
    int *V = new int[SizeI*SizeJ];
    unsigned char *EdgeImage = new unsigned char[SizeI*SizeJ];
    unsigned char *GradientImage = new unsigned char[SizeI*SizeJ];
    unsigned char *SketchImage = new unsigned char[SizeI*SizeJ];

    unsigned char *OutputImage = new unsigned char[SizeI*SizeJ*3];
    string inputFile = "naruto";
    string filename;

    filename = inputFile;
    filename.append(".raw");
    ReadImageFromFile(filename.c_str(),ColorImage,SizeI,SizeJ,3);
    RGB2YUV(ColorImage,Y,U,V,SizeI,SizeJ);

    EdgeDetectionGradient(Y,EdgeImage,SizeI,SizeJ,0,21);
    ReadImageFromFile("GradientImage_1st_out.raw",GradientImage,SizeI,SizeJ,1);
    GrayScaleInverse(GradientImage,SketchImage,SizeI,SizeJ);

    YUV2RGB(SketchImage,U,V,OutputImage,SizeI,SizeJ);

    filename = inputFile;
    filename.append("_color_ed_out.raw");
    WriteImageToFile(filename.c_str(),OutputImage,SizeI,SizeJ,3);

    delete ColorImage;
    delete Y;
    delete EdgeImage;
    delete GradientImage;
    delete SketchImage;
    delete OutputImage;
}

int main()
{
    processA();
    processB();
//    processC();
//    processD();
    processE();
//    test();
    return 0;
}
