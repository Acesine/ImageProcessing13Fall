/**
EE569 HW #2 Problem 3: Digital Half-toning
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

using namespace std;

// Part (a): Dithering
void processA()
{
    printf("Process A begins...\n");
    int SizeI = 1024;
    int SizeJ = 1024;
    unsigned char* InputImage = new unsigned char[SizeI*SizeJ];
    unsigned char* ImageBuff1 = new unsigned char[SizeI*SizeJ];
    ReadImageFromFile("man.raw",InputImage,SizeI,SizeJ,1);
    // Fixed Threshold
    Dithering(InputImage,ImageBuff1,SizeI,SizeJ,0,0,0);
    WriteImageToFile("man_1_out.raw",ImageBuff1,SizeI,SizeJ,1);

    // Random Threshold
    Dithering(InputImage,ImageBuff1,SizeI,SizeJ,1,0,0);
    WriteImageToFile("man_2_out.raw",ImageBuff1,SizeI,SizeJ,1);

    // Dithering Matrix
    int indexM_2[] = {1,2,3,0};
    int indexM_4[] = {5,9,6,10,13,1,14,2,7,11,4,8,15,3,12,0};
    Dithering(InputImage,ImageBuff1,SizeI,SizeJ,2,indexM_4,4);
    WriteImageToFile("man_3_out.raw",ImageBuff1,SizeI,SizeJ,1);

    delete InputImage;
    delete ImageBuff1;
    printf("Process A ends...\n");
}

// Error Diffusion
void processB()
{
    printf("Process B begins...\n");
    int SizeI = 256;
    int SizeJ = 256;
    unsigned char* InputImage = new unsigned char[SizeI*SizeJ];
    unsigned char* ImageBuff1 = new unsigned char[SizeI*SizeJ];
    ReadImageFromFile("gradient.raw",InputImage,SizeI,SizeJ,1);

    // Floyd-Steinberg
    ErrorDiffusion(InputImage,ImageBuff1,SizeI,SizeJ,1,127);
    WriteImageToFile("gradient_1_out.raw",ImageBuff1,SizeI,SizeJ,1);
    //JJN
    ErrorDiffusion(InputImage,ImageBuff1,SizeI,SizeJ,2,127);
    WriteImageToFile("gradient_2_out.raw",ImageBuff1,SizeI,SizeJ,1);
    //Stucki
    ErrorDiffusion(InputImage,ImageBuff1,SizeI,SizeJ,3,127);
    WriteImageToFile("gradient_3_out.raw",ImageBuff1,SizeI,SizeJ,1);

    delete InputImage;
    delete ImageBuff1;
    printf("Process B ends...\n");
}

// Inverse Halftoning
void processC()
{
    printf("Process C begins...\n");
    int SizeI = 256;
    int SizeJ = 256;
    unsigned char* InputImage = new unsigned char[SizeI*SizeJ];
    unsigned char* ImageBuff1 = new unsigned char[SizeI*SizeJ];
    ReadImageFromFile("gradient_1_out.raw",InputImage,SizeI,SizeJ,1);
    //ReadImageFromFile("gradient_3_test_out.raw",InputImage,SizeI,SizeJ,1);

    InverseHalftoning(InputImage,ImageBuff1,SizeI,SizeJ);
    WriteImageToFile("InverseHalftoning_out.raw",ImageBuff1,SizeI,SizeJ,1);

    delete InputImage;
    delete ImageBuff1;
    printf("Process C ends...\n");
}

void test()
{
    int SizeI = 1024;
    int SizeJ = 1024;
    unsigned char* InputImage = new unsigned char[SizeI*SizeJ];
    unsigned char* ImageBuff1 = new unsigned char[SizeI*SizeJ];
    ReadImageFromFile("man_errordif_out.raw",InputImage,SizeI,SizeJ,1);
    //ReadImageFromFile("man_3_out.raw",InputImage,SizeI,SizeJ,1);

    InverseHalftoning(InputImage,ImageBuff1,SizeI,SizeJ);
    WriteImageToFile("InverseHalftoning_test_out.raw",ImageBuff1,SizeI,SizeJ,1);

    delete InputImage;
    delete ImageBuff1;
}

int main()
{
    processA();
    processB();
    processC();
//    test();
    return 0;
}
