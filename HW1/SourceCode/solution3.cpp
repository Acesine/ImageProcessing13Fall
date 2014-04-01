/**
EE569 HW #1 Problem 3: Noise Removal
Author: Xiang Gao
USCID:  9216348910
Email:  xianggao@usc.edu
Submission Date: Sep 19, 2013
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include "MyIO.h"
#include "MyFiltering.h"

using namespace std;

// (a) Mix Noise in Color Image
void DenoiseImpluse(const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI,int SizeJ, int BytesPerPixel)
{
    // Using Median Filter
    int FilterSize = 5;
    MedianFilter('S',FilterSize,ImageData,ImageDataOut,SizeI,SizeJ,BytesPerPixel);
}

void DenoiseGaussian(const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI,int SizeJ, int BytesPerPixel)
{
    // Using Low-Pass Filter
    int FilterSize = 3;
    //Uniform
    double *FilterData = (double*) malloc(sizeof(double)*FilterSize*FilterSize);
    for(int i=0; i<FilterSize*FilterSize; i++)
    {
        FilterData[i] = 1.0/(FilterSize*FilterSize);
    }


    // Gaussian
    //double FilterData[3][3] = {{1.0/16,2.0/16,1.0/16},
    //    {2.0/16,4.0/16,2.0/16},
    //    {1.0/16,2.0/16,1.0/16}
    //};
    Filter(FilterData,FilterSize,ImageData,ImageDataOut,SizeI,SizeJ,BytesPerPixel);

    free(FilterData);
}

void processA()
{
    printf("Process A begins...\n");

    int Size = 256;
    int BytesPerPixel = 3;
    unsigned char ImageData[Size][Size][BytesPerPixel];
    unsigned char ImageDataOut_Imp[Size][Size][BytesPerPixel];
    unsigned char ImageDataOut_Gau[Size][Size][BytesPerPixel];
    int histogram[256];
    string InputFileName;
    string OutputFileName;

    // Read Image Data
    InputFileName = "peppers_mix.raw";
    ReadImageFromFile(InputFileName.c_str(),&ImageData[0][0][0],Size,Size,BytesPerPixel);

    // Get Histogram of 3 channels
    for(int color=0; color<3; color++)
    {
        memset(histogram,0,256*sizeof(int));
        for(int i=0; i<Size; i++)
        {
            for(int j=0; j<Size; j++)
            {
                histogram[ImageData[i][j][color]] ++;
            }
        }
        OutputFileName = "peppers_mix_histdata_";
        char t = color + '0';
        OutputFileName.push_back(t);
        OutputFileName.append(".dat");
        WriteHistogramToFile(OutputFileName.c_str(),histogram);
    }

    // First low-pass then Median:
    DenoiseGaussian(&ImageData[0][0][0],&ImageDataOut_Gau[0][0][0],Size,Size,BytesPerPixel);
    DenoiseImpluse(&ImageDataOut_Gau[0][0][0],&ImageDataOut_Imp[0][0][0],Size,Size,BytesPerPixel);
    OutputFileName = "peppers_mix_Gau_Imp_out.raw";
    WriteImageToFile(OutputFileName.c_str(),&ImageDataOut_Imp[0][0][0],Size,Size,BytesPerPixel);

    // First Median then low-pass:
    DenoiseImpluse(&ImageData[0][0][0],&ImageDataOut_Imp[0][0][0],Size,Size,BytesPerPixel);
    DenoiseGaussian(&ImageDataOut_Imp[0][0][0],&ImageDataOut_Gau[0][0][0],Size,Size,BytesPerPixel);
    OutputFileName = "peppers_mix_Imp_Gau_out.raw";
    WriteImageToFile(OutputFileName.c_str(),&ImageDataOut_Gau[0][0][0],Size,Size,BytesPerPixel);

    printf("Process A ends...\n");
}

// (b) Bilateral Filtering

void processB()
{
    printf("Process B begins...\n");

    int Size = 256;
    int BytesPerPixel = 1;
    unsigned char ImageData[Size][Size];
    unsigned char ImageDataOut[Size][Size];
    string InputFileName;
    char OutputFileName[20];

    // Read Image
    InputFileName = "lena.raw";
    ReadImageFromFile(InputFileName.c_str(),&ImageData[0][0],Size,Size,BytesPerPixel);

    // Using Bilateral Filter

    int WindowSize = 7;
    int sigma_d = 5, sigma_r = 70;
    for(sigma_d=1; sigma_d<=9; sigma_d+=4)
    {
        for(sigma_r=10; sigma_r<=100; sigma_r+=40)
        {
            printf("sigma_d=%d, sigma_r=%d\n",sigma_d,sigma_r);
            BilateralFilter(WindowSize,sigma_d,sigma_r,&ImageData[0][0],&ImageDataOut[0][0],Size,Size);

            // Write Output Image to File
            sprintf(OutputFileName,"lena_%d_%d_out.raw",sigma_d,sigma_r);
            WriteImageToFile(OutputFileName,&ImageDataOut[0][0],Size,Size,BytesPerPixel);
        }
    }

    /*
    // Different window size:
    int WindowSize = 7;
    int sigma_d = 5, sigma_r = 50;
    for(WindowSize = 3; WindowSize<=27; WindowSize+=8)
    {
        printf("Windowsize=%d\n",WindowSize);
        BilateralFilter(WindowSize,sigma_d,sigma_r,&ImageData[0][0],&ImageDataOut[0][0],Size,Size);

        // Write Output Image to File
        sprintf(OutputFileName,"lena_out_W%d.raw",WindowSize);
        WriteImageToFile(OutputFileName,&ImageDataOut[0][0],Size,Size,BytesPerPixel);
    }
    */

    // Using Low-Pass Filter of the same size..
    int FilterSize = 7;
    // Uniform
    double *FilterData = (double*) malloc(sizeof(double)*FilterSize*FilterSize);
    for(int i=0; i<FilterSize*FilterSize; i++)
    {
        FilterData[i] = 1.0/(FilterSize*FilterSize);
    }
    Filter(FilterData,FilterSize,&ImageData[0][0],&ImageDataOut[0][0],Size,Size,BytesPerPixel);
    free(FilterData);
    WriteImageToFile("lena_lowpass_out.raw",&ImageDataOut[0][0],Size,Size,BytesPerPixel);

    /*
    // Experiment with "black and white"
    unsigned char TmpData[400][400];
    unsigned char TmpDataOut[400][400];
    ReadImageFromFile("blackwhite.raw",&TmpData[0][0],400,400,1);

    int WindowSize = 7;
    int sigma_d = 5, sigma_r = 50;
    BilateralFilter(WindowSize,sigma_d,sigma_r,&TmpData[0][0],&TmpDataOut[0][0],400,400);
    // Write Output Image to File
    WriteImageToFile("blackwhite_out_bf.raw",&TmpDataOut[0][0],400,400,1);

    // Using Low-Pass Filter of the same size..
    int FilterSize = 7;
    // Uniform
    double *FilterData = (double*) malloc(sizeof(double)*FilterSize*FilterSize);
    for(int i=0; i<FilterSize*FilterSize; i++)
    {
        FilterData[i] = 1.0/(FilterSize*FilterSize);
    }
    Filter(FilterData,FilterSize,&TmpData[0][0],&TmpDataOut[0][0],400,400,1);
    free(FilterData);
    WriteImageToFile("blackwhite_out_lp.raw",&TmpDataOut[0][0],400,400,1);
    */

    printf("Process B ends...\n");
}

// (c) Non-Local Means (NLM) Filtering

void processC()
{
    printf("Process C begins...\n");

    int Size = 256;
    int BytesPerPixel = 1;
    unsigned char ImageData[Size][Size];
    unsigned char ImageDataOut[Size][Size];
    string InputFileName;
    string OutputFileName;

    // Read Image
    InputFileName = "lena.raw";
    ReadImageFromFile(InputFileName.c_str(),&ImageData[0][0],Size,Size,BytesPerPixel);

    // Using NLM Filter
    int NeighborSize = 7;
    int SearchWindowSize = 21;
    double sigma = 18;
    double h = 10*sigma;
    printf("Neighborhood Size: %d\nSearching WindowSize: %d\nParameter \"h=10*sigma=\": %.1f\n",NeighborSize,SearchWindowSize,h);
    NLMFilter(NeighborSize,SearchWindowSize,h,&ImageData[0][0],&ImageDataOut[0][0],Size,Size);

    // Write Output to File
    OutputFileName = "lena_NLM_out.raw";
    WriteImageToFile(OutputFileName.c_str(),&ImageDataOut[0][0],Size,Size,BytesPerPixel);

    /*
    // experiment with 'blackwhite.raw'
    unsigned char TmpData[400][400];
    unsigned char TmpDataOut[400][400];
    ReadImageFromFile("blackwhite.raw",&TmpData[0][0],400,400,1);

    // Using NLM Filter
    int NeighborSize = 7;
    int SearchWindowSize = 21;
    double sigma = 18;
    double h = 10*sigma;
    printf("Neighborhood Size: %d\nSearching WindowSize: %d\nParameter \"h=10*sigma=\": %.1f\n",NeighborSize,SearchWindowSize,h);
    NLMFilter(NeighborSize,SearchWindowSize,h,&TmpData[0][0],&TmpDataOut[0][0],400,400);

    // Write Output to File
    WriteImageToFile("blackwhite_NLM_out.raw",&TmpDataOut[0][0],400,400,1);
    */
    printf("\nProcess C ends...\n");
}
int main()
{
    processA();
    processB();
    processC();
    return 0;
}
