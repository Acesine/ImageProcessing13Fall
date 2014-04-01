/**
EE569 HW #1 Problem 2: Image Enhancement
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

using namespace std;



// (a) Histogram Equalization for Grayscale Images
void processA()
{
    printf("Part A begins...\n");
    int Size = 512; // Files are 512*512
    int BytesPerPixel = 1;
    unsigned char ImageData[Size][Size]; // Allocate image data array
    unsigned char ImageDataOut[Size][Size];
    string InputFileNameArray[] = {"barbara_dark","barbara_mid","barbara_bright"};
    string OutputFileName; // Name of output image file
    string OutDataFileName; // Name of histogram file
    string TransferFileName; // Name of transfer mapping file
    int histogram[256];
    int histCumu[256];
    int Transfer[256];
    // Processing codes begin here:
    for(int fileCounter=0; fileCounter<3; fileCounter++)
    {
        string InputFileName = InputFileNameArray[fileCounter];
        InputFileName.append(".raw");
        // Read image from file
        ReadImageFromFile(InputFileName.c_str(),&ImageData[0][0],Size,Size,BytesPerPixel);

        // Write histogram of input image to file
        GetHistogram(&ImageData[0][0],Size,Size,histogram);
        OutDataFileName = InputFileNameArray[fileCounter];
        OutDataFileName.append("_histdata.dat");
        WriteHistogramToFile(OutDataFileName.c_str(),histogram);

        // (1)Full-range linear scaling:
        // Construct Transfer function
        memset(Transfer,0,256*sizeof(int));
        int Vmax=0, Vmin=255;
        for(int i=0; i<Size; i++)
        {
            for(int j=0; j<Size; j++)
            {
                if(ImageData[i][j]>Vmax)
                {
                    Vmax = ImageData[i][j];
                }
                if(ImageData[i][j]<Vmin)
                {
                    Vmin = ImageData[i][j];
                }
            }
        }
        double alpha = 255.0 / (Vmax-Vmin);
        double beta = (255.0 * Vmin) / (Vmax-Vmin); // y = alpha*x-beta
        for(int i=0; i<256; i++)
        {
            Transfer[i] = (int)(alpha*i-beta);
        }
        for(int i=0; i<Size; i++)
        {
            for(int j=0; j<Size; j++)
            {
                ImageDataOut[i][j] = Transfer[ImageData[i][j]];
            }
        }

        // Write image data from image data matrix
        OutputFileName = InputFileNameArray[fileCounter];
        OutputFileName.append("_fr_out.raw");
        WriteImageToFile(OutputFileName.c_str(),&ImageDataOut[0][0],Size,Size,BytesPerPixel);

        // Write histogram of output image to file
        GetHistogram(&ImageDataOut[0][0],Size,Size,histogram);
        OutDataFileName = InputFileNameArray[fileCounter];
        OutDataFileName.append("_histdata_fr.dat");
        WriteHistogramToFile(OutDataFileName.c_str(),histogram);

        // Write Transfer mapping to file
        TransferFileName = InputFileNameArray[fileCounter];
        TransferFileName.append("_transferF_fr.dat");
        WriteTransferToFile(TransferFileName.c_str(),Transfer);

        // (2)Histogram equalization:
        GetHistogram(&ImageData[0][0],Size,Size,histogram);
        histCumu[0] = histogram[0];
        for(int i=1; i<256; i++)
        {
            histCumu[i] = histCumu[i-1]+histogram[i];
        }
        const int totalN = Size*Size;
        // Construct Transfer function
        for(int i=0; i<256; i++)
        {
            Transfer[i] = 255*histCumu[i]/totalN;
        }
        for(int i=0; i<Size; i++)
        {
            for(int j=0; j<Size; j++)
            {
                ImageDataOut[i][j] = Transfer[ImageData[i][j]];
            }
        }

        // Write image data from image data matrix
        OutputFileName = InputFileNameArray[fileCounter];
        OutputFileName.append("_he_out.raw");
        WriteImageToFile(OutputFileName.c_str(),&ImageDataOut[0][0],Size,Size,BytesPerPixel);

        // Write histogram of output image to file
        GetHistogram(&ImageDataOut[0][0],Size,Size,histogram);
        OutDataFileName = InputFileNameArray[fileCounter];
        OutDataFileName.append("_histdata_he.dat");
        WriteHistogramToFile(OutDataFileName.c_str(),histogram);

        // Write Transfer mapping to file
        TransferFileName = InputFileNameArray[fileCounter];
        TransferFileName.append("_transferF_he.dat");
        WriteTransferToFile(TransferFileName.c_str(),Transfer);

    }
    printf("Part A ends...\n");
}

// (b) Histogram Equalization for Color Images
void processB()
{
    printf("Part B begins...\n");

    string InputFileName = "ocean_contrast.raw";
    string OutputFileName_1 = "ocean_contrast_cumu_out.raw";
    string OutputFileName_2 = "ocean_contrast_equal_out.raw";
    int SizeI = 332;
    int SizeJ = 500;
    int BytesPerPixel = 3;
    unsigned char ImageData[SizeI][SizeJ][BytesPerPixel];
    unsigned char ImageDataOut[SizeI][SizeJ][BytesPerPixel];
    int histogram[256];
    int histCumu[256];
    int Transfer[256];
    // Read image from file
    ReadImageFromFile(InputFileName.c_str(),&ImageData[0][0][0],SizeI,SizeJ,BytesPerPixel);

    string HistFileNameArray[] = {"ocean_contrast_r","ocean_contrast_g","ocean_contrast_b"};
    string HistFileName;
    const int totalN = SizeI*SizeJ;
    // (1)Histogram equalization using cumulative method:
    for(int color=0; color<3; color++)
    {
        // Get histogram: can't use GetHistogram() here
        memset(histogram,0,256*sizeof(int)); // Initialize it into 0
        for(int i=0; i<SizeI; i++)
        {
            for(int j=0; j<SizeJ; j++)
            {
                histogram[ImageData[i][j][color]] ++;
            }
        }
        HistFileName = HistFileNameArray[color];
        HistFileName.append("_histdata.dat");
        WriteHistogramToFile(HistFileName.c_str(),histogram);

        histCumu[0] = histogram[0];
        for(int i=1; i<256; i++)
        {
            histCumu[i] = histCumu[i-1]+histogram[i];
        }
        // Construct Transfer function
        for(int i=0; i<256; i++)
        {
            Transfer[i] = 255*histCumu[i]/totalN;
        }
        for(int i=0; i<SizeI; i++)
        {
            for(int j=0; j<SizeJ; j++)
            {
                ImageDataOut[i][j][color] = Transfer[ImageData[i][j][color]];
            }
        }

        // Get histogram of output image: can't use GetHistogram() here
        memset(histogram,0,256*sizeof(int)); // Initialize it into 0
        for(int i=0; i<SizeI; i++)
        {
            for(int j=0; j<SizeJ; j++)
            {
                histogram[ImageDataOut[i][j][color]] ++;
            }
        }
        HistFileName = HistFileNameArray[color];
        HistFileName.append("_out_cumu_histdata.dat");
        WriteHistogramToFile(HistFileName.c_str(),histogram);
    }
    // Write image data from image data matrix
    WriteImageToFile(OutputFileName_1.c_str(),&ImageDataOut[0][0][0],SizeI,SizeJ,BytesPerPixel);

    // (2)Histogram equalization using equalized histogram method:
    int histLevelIndex[256];
    int histCumuBefore[256];
    int N = totalN / 256; // Average N pixel for each level
    for(int color=0; color<3; color++)
    {
        memset(histCumuBefore,0,256*sizeof(int));
        memset(histLevelIndex,0,256*sizeof(int));
        memset(histogram,0,256*sizeof(int)); // Initialize it into 0
        for(int i=0; i<SizeI; i++)
        {
            for(int j=0; j<SizeJ; j++)
            {
                histogram[ImageData[i][j][color]] ++;
            }
        }
        histCumuBefore[0] = 0;
        for(int i=1; i<256; i++)
        {
            histCumuBefore[i] = histCumuBefore[i-1] + histogram[i-1];
        }
        // No explicit transfer function used here, since transformation of some pixel is based on pixel's order
        for(int i=0; i<SizeI; i++)
        {
            for(int j=0; j<SizeJ; j++)
            {
                histLevelIndex[ImageData[i][j][color]] ++;
                int L = (histCumuBefore[ImageData[i][j][color]]+histLevelIndex[ImageData[i][j][color]]-1) / N;
                if(L>=256)
                    L = 255;
                ImageDataOut[i][j][color] = L;
            }
        }

        // Get histogram of output image: can't use GetHistogram() here
        memset(histogram,0,256*sizeof(int)); // Initialize it into 0
        for(int i=0; i<SizeI; i++)
        {
            for(int j=0; j<SizeJ; j++)
            {
                histogram[ImageDataOut[i][j][color]] ++;
            }
        }
        HistFileName = HistFileNameArray[color];
        HistFileName.append("_out_equal_histdata.dat");
        WriteHistogramToFile(HistFileName.c_str(),histogram);
    }
    // Write image data from image data matrix
    WriteImageToFile(OutputFileName_2.c_str(),&ImageDataOut[0][0][0],SizeI,SizeJ,BytesPerPixel);

    /*
    // An experiment:
    // Read image from file
    unsigned char tmpData[256][256];
    unsigned char tmpDataOut[256][256];
    int tmphistogram[256];
    int tmphistCumu[256];
    int tmpTransfer[256];
    ReadImageFromFile("hbhw.raw",&tmpData[0][0],256,256,1);

    const int tmptotalN = 256*256;
    // (1)Histogram equalization using cumulative method:
    // Get histogram: can't use GetHistogram() here
    memset(tmphistogram,0,256*sizeof(int)); // Initialize it into 0
    for(int i=0; i<256; i++)
    {
        for(int j=0; j<256; j++)
        {
            tmphistogram[tmpData[i][j]] ++;
        }
    }

    tmphistCumu[0] = tmphistogram[0];
    for(int i=1; i<256; i++)
    {
        tmphistCumu[i] = tmphistCumu[i-1]+tmphistogram[i];
    }
    // Construct Transfer function
    for(int i=0; i<256; i++)
    {
        tmpTransfer[i] = 255*tmphistCumu[i]/tmptotalN;
    }
    for(int i=0; i<256; i++)
    {
        for(int j=0; j<256; j++)
        {
            tmpDataOut[i][j] = tmpTransfer[tmpData[i][j]];
        }
    }

    // Get histogram of output image: can't use GetHistogram() here
    memset(tmphistogram,0,256*sizeof(int)); // Initialize it into 0
    for(int i=0; i<256; i++)
    {
        for(int j=0; j<256; j++)
        {
            tmphistogram[tmpDataOut[i][j]] ++;
        }
    }
    // Write image data from image data matrix
    WriteImageToFile("hbhw_cm.raw",&tmpDataOut[0][0],256,256,1);

    // (2)Histogram equalization using equalized histogram method:
    int tmphistLevelIndex[256];
    int tmphistCumuBefore[256];
    int tmpN = tmptotalN / 256; // Average N pixel for each level
    memset(tmphistCumuBefore,0,256*sizeof(int));
    memset(tmphistLevelIndex,0,256*sizeof(int));
    memset(tmphistogram,0,256*sizeof(int)); // Initialize it into 0
    for(int i=0; i<256; i++)
    {
        for(int j=0; j<256; j++)
        {
            tmphistogram[tmpData[i][j]] ++;
        }
    }
    tmphistCumuBefore[0] = 0;
    for(int i=1; i<256; i++)
    {
        tmphistCumuBefore[i] = tmphistCumuBefore[i-1] + tmphistogram[i-1];
    }
    // No explicit transfer function used here, since transformation of some pixel is based on pixel's order
    for(int i=0; i<256; i++)
    {
        for(int j=0; j<256; j++)
        {
            tmphistLevelIndex[tmpData[i][j]] ++;
            int L = (tmphistCumuBefore[tmpData[i][j]]+tmphistLevelIndex[tmpData[i][j]]-1) / tmpN;
            if(L>=256)
                L = 255;
            tmpDataOut[i][j] = L;
        }
    }

    // Get histogram of output image: can't use GetHistogram() here
    memset(tmphistogram,0,256*sizeof(int)); // Initialize it into 0
    for(int i=0; i<256; i++)
    {
        for(int j=0; j<256; j++)
        {
            tmphistogram[tmpDataOut[i][j]] ++;
        }
    }
    // Write image data from image data matrix
    WriteImageToFile("hbhw_eq.raw",&tmpDataOut[0][0],256,256,1);
    ///
    */
    printf("Part B ends...\n");
}

// (c) Histogram Transform
double normal(int mu,int sigma,double x)
{
    double PI = 3.1415926;
    return 1/(sqrt(2*PI)*sigma) * exp(-(x-mu)*(x-mu)/(2*sigma*sigma));
}
void processC()
{
    printf("Part C begins...\n");
    string InputFileName = "barbara_contrast.raw";
    string OutputFileName = "barbara_contrast_out.raw";
    string HistogramFileName;
    string TransferFunctionFileName;
    int Size = 512;
    int BytesPerPixel = 1;
    unsigned char ImageData[Size][Size];
    unsigned char ImageDataOut[Size][Size];
    int histogram[256];
    double NormalArray[256];
    int histCumu[256];
    double NormalCumu[256];
    int Transfer[256];
    // Read image from file
    ReadImageFromFile(InputFileName.c_str(),&ImageData[0][0],Size,Size,BytesPerPixel);

    // Write histogram of input image to file
    GetHistogram(&ImageData[0][0],Size,Size,histogram);
    HistogramFileName = "barbara_contrast_histdata.dat";
    WriteHistogramToFile(HistogramFileName.c_str(),histogram);

    histCumu[0] = histogram[0];
    for(int i=1; i<256; i++)
    {
        histCumu[i] = histCumu[i-1] + histogram[i];
    }
    int mu = 70, sigma = 20;
    NormalArray[0] = normal(mu,sigma,0);
    double sum = NormalArray[0];
    for(int i=1; i<256; i++)
    {
        NormalArray[i] = normal(mu,sigma,i);
        sum += NormalArray[i];
    }
    NormalArray[0] /= sum;
    NormalCumu[0] = NormalArray[0];
    for(int i=1; i<255; i++)
    {
        NormalArray[i] /= sum;
        NormalCumu[i] = NormalCumu[i-1] + NormalArray[i];
    }
    NormalArray[255] = 1.0 - NormalCumu[254];
    NormalCumu[255] = 1.0;

    int totalN = Size*Size;
    for(int i=0; i<256; i++)
    {
        for(int k=1; k<256; k++)
        {
            if(NormalCumu[k]*totalN>=0.9999*histCumu[i]) // A little bit modification
            {
                if(NormalCumu[k]*totalN-histCumu[i] < histCumu[i]-NormalCumu[k-1]*totalN)
                {
                    Transfer[i] = k;
                }
                else
                    Transfer[i] = k-1;
                break;
            }
        }
    }


    for(int i=0; i<Size; i++)
    {
        for(int j=0; j<Size; j++)
        {
            ImageDataOut[i][j] = Transfer[ImageData[i][j]];
        }
    }

    // Write image data from image data matrix
    WriteImageToFile(OutputFileName.c_str(),&ImageDataOut[0][0],Size,Size,BytesPerPixel);

    // Write histogram of output image to file
    GetHistogram(&ImageDataOut[0][0],Size,Size,histogram);
    HistogramFileName = "barbara_contrast_out_histdata.dat";
    WriteHistogramToFile(HistogramFileName.c_str(),histogram);

    // Write Transfer Function data to file
    TransferFunctionFileName = "barbara_contrast_transferF.dat";
    WriteTransferToFile(TransferFunctionFileName.c_str(),Transfer);

    printf("Part C ends...\n");
}

int main()
{
    processA();
    processB();
    processC();
    return 0;
}

