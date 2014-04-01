/**
EE569 HW #2 Problem 2: Morphological Processing
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

// Part (a): Shrinking
void processA()
{
    printf("Process A begins...\n");

    int SizeI = 768;
    int SizeJ = 1024;
    unsigned char* InputImage = new unsigned char[SizeI*SizeJ*3];
    unsigned char* ImageBuff1 = new unsigned char[SizeI*SizeJ];
    unsigned char* ImageBuff2 = new unsigned char[SizeI*SizeJ];
    ReadImageFromFile("stars.raw",InputImage,SizeI,SizeJ,3);
    Color2Gray(InputImage,ImageBuff1,SizeI,SizeJ);
    WriteImageToFile("stars_gray_out.raw",ImageBuff1,SizeI,SizeJ,1);
    //Calculate histogram
    int hist[256];
    memset(hist,0,256*sizeof(int));
    for(int i=0; i<SizeI; i++)
    {
        for(int j=0; j<SizeJ; j++)
        {
            hist[ImageBuff1[i*SizeJ+j]] ++;
        }
    }
    WriteHistogramToFile("stars_gray_hist.dat",hist);
    Gray2Binary(ImageBuff1,ImageBuff2,SizeI,SizeJ,30,'L');
    WriteImageToFile("stars_binary_out.raw",ImageBuff2,SizeI,SizeJ,1);

    int starsize = 1;
    int *objsizes_cumu = new int[SizeI*SizeJ];
    objsizes_cumu[0] = 0;
    objsizes_cumu[starsize] = CountSingleDots(ImageBuff2,SizeI,SizeJ);

    int remain_pre = 0;
    int remain = MorphOperation(ImageBuff2,ImageBuff1,SizeI,SizeJ,'S');
    starsize ++;
    objsizes_cumu[starsize] = CountSingleDots(ImageBuff1,SizeI,SizeJ);
    printf("%d ",remain);
    fflush(stdout);
    int outbuff = 2;

    while(remain != remain_pre)
    {
        remain_pre = remain;
        printf("%d ",remain);
        fflush(stdout);
        if(outbuff == 1)
        {
            remain = MorphOperation(ImageBuff2,ImageBuff1,SizeI,SizeJ,'S');
            starsize ++;
            objsizes_cumu[starsize] = CountSingleDots(ImageBuff1,SizeI,SizeJ);
            outbuff = 2;
        }
        else
        {
            remain = MorphOperation(ImageBuff1,ImageBuff2,SizeI,SizeJ,'S');
            starsize ++;
            objsizes_cumu[starsize] = CountSingleDots(ImageBuff2,SizeI,SizeJ);
            outbuff = 1;
        }
    }
    int StarNum = 0;
    if(outbuff == 1)
    {
        for(int i=0; i<SizeI; i++)
        {
            for(int j=0; j<SizeJ; j++)
            {
                if(ImageBuff2[i*SizeJ+j]==255)
                    StarNum ++;
            }
        }
        WriteImageToFile("stars_out.raw",ImageBuff2,SizeI,SizeJ,1);
    }
    else
    {
        for(int i=0; i<SizeI; i++)
        {
            for(int j=0; j<SizeJ; j++)
            {
                if(ImageBuff1[i*SizeJ+j]==255)
                    StarNum ++;
            }
        }
        WriteImageToFile("stars_out.raw",ImageBuff1,SizeI,SizeJ,1);
    }
    printf("\nNumber of Stars: %d\n",StarNum);

    for(int i=1; i<=starsize; i++)
        printf("Star of size %d: %d\n",i,objsizes_cumu[i]-objsizes_cumu[i-1]);
    delete InputImage;
    delete ImageBuff1;
    delete ImageBuff2;
    delete objsizes_cumu;
    printf("Process A ends...\n");
}

// Part (b): Thinning
void processB()
{
    printf("Process B begins...\n");

    int SizeI = 959;
    int SizeJ = 1231;
    unsigned char* InputImage = new unsigned char[SizeI*SizeJ*3];
    unsigned char* ImageBuff1 = new unsigned char[SizeI*SizeJ];
    unsigned char* ImageBuff2 = new unsigned char[SizeI*SizeJ];
    int countDigits = 0;
    ReadImageFromFile("digits.raw",InputImage,SizeI,SizeJ,3);
    Color2Gray(InputImage,ImageBuff1,SizeI,SizeJ);
    WriteImageToFile("digits_gray_out.raw",ImageBuff1,SizeI,SizeJ,1);
    //Calculate histogram
    int hist[256];
    memset(hist,0,256*sizeof(int));
    for(int i=0; i<SizeI; i++)
    {
        for(int j=0; j<SizeJ; j++)
        {
            hist[ImageBuff1[i*SizeJ+j]] ++;
        }
    }
    WriteHistogramToFile("digits_gray_hist.dat",hist);
    Gray2Binary(ImageBuff1,ImageBuff2,SizeI,SizeJ,125,'D');
    WriteImageToFile("digits_binary_out.raw",ImageBuff2,SizeI,SizeJ,1);
    int remain_pre = 0;
    int remain = MorphOperation(ImageBuff2,ImageBuff1,SizeI,SizeJ,'T');
    printf("%d ",remain);
    fflush(stdout);
    int outbuff = 2;

    while(remain != remain_pre)
    {
        remain_pre = remain;
        printf("%d ",remain);
        fflush(stdout);
        if(outbuff == 1)
        {
            remain = MorphOperation(ImageBuff2,ImageBuff1,SizeI,SizeJ,'T');
            outbuff = 2;
        }
        else
        {
            remain = MorphOperation(ImageBuff1,ImageBuff2,SizeI,SizeJ,'T');
            outbuff = 1;
        }
    }
    int Thres = 10; // Each digit contains at least 10 pixels
    if(outbuff == 1)
    {
        WriteImageToFile("digits_out.raw",ImageBuff2,SizeI,SizeJ,1);
        countDigits = CountConnectedObjects(ImageBuff2,SizeI,SizeJ,Thres);
    }
    else
    {
        WriteImageToFile("digits_out.raw",ImageBuff1,SizeI,SizeJ,1);
        countDigits = CountConnectedObjects(ImageBuff1,SizeI,SizeJ,Thres);
    }

    printf("\nNumber of Digits: %d\n",countDigits);
    delete InputImage;
    delete ImageBuff1;
    delete ImageBuff2;
    printf("Process B ends...\n");
}

// Part (C): Skeletonizing
void processC()
{
    printf("Process C begins...\n");

    int SizeI = 440;
    int SizeJ = 960;

    unsigned char* InputImage = new unsigned char[SizeI*SizeJ*3];
    unsigned char* ImageBuff1 = new unsigned char[SizeI*SizeJ];
    unsigned char* ImageBuff2 = new unsigned char[SizeI*SizeJ];
    ReadImageFromFile("dinosaur.raw",InputImage,SizeI,SizeJ,3);
    Color2Gray(InputImage,ImageBuff1,SizeI,SizeJ);
    WriteImageToFile("dinosaur_gray_out.raw",ImageBuff1,SizeI,SizeJ,1);
    Gray2Binary(ImageBuff1,ImageBuff2,SizeI,SizeJ,80,'L');
    WriteImageToFile("dinosaur_binary_out.raw",ImageBuff2,SizeI,SizeJ,1);


    Erode4(ImageBuff2,ImageBuff1,SizeI,SizeJ);
    Erode4(ImageBuff1,ImageBuff2,SizeI,SizeJ);
    WriteImageToFile("dinosaur_binary_erode_out.raw",ImageBuff2,SizeI,SizeJ,1);

    int remain_pre = 0;
    int remain = MorphOperation(ImageBuff2,ImageBuff1,SizeI,SizeJ,'K');
    printf("%d ",remain);
    fflush(stdout);
    int outbuff = 2;

    while(remain != remain_pre)
    {
        remain_pre = remain;
        printf("%d ",remain);
        fflush(stdout);
        if(outbuff == 1)
        {
            remain = MorphOperation(ImageBuff2,ImageBuff1,SizeI,SizeJ,'K');
            outbuff = 2;
        }
        else
        {
            remain = MorphOperation(ImageBuff1,ImageBuff2,SizeI,SizeJ,'K');
            outbuff = 1;
        }
    }
    if(outbuff == 1)
    {
        WriteImageToFile("dinosaur_out.raw",ImageBuff2,SizeI,SizeJ,1);
    }
    else
    {
        WriteImageToFile("dinosaur_out.raw",ImageBuff1,SizeI,SizeJ,1);
    }
    delete InputImage;
    delete ImageBuff1;
    delete ImageBuff2;
    printf("Process C ends...\n");
}

// Part (D): Pacman Game
void processD()
{
    printf("Process D begins...\n");

    int SizeI = 441;
    int SizeJ = 441;

    unsigned char* InputImage = new unsigned char[SizeI*SizeJ*3];
    unsigned char* ImageBuff1 = new unsigned char[SizeI*SizeJ];
    unsigned char* ImageBuff2 = new unsigned char[SizeI*SizeJ];
    ReadImageFromFile("pacman.raw",InputImage,SizeI,SizeJ,3);
    Color2Gray(InputImage,ImageBuff1,SizeI,SizeJ);
    WriteImageToFile("pacman_gray_out.raw",ImageBuff1,SizeI,SizeJ,1);
    Gray2Binary(ImageBuff1,ImageBuff2,SizeI,SizeJ,15,'L');
    WriteImageToFile("pacman_binary_out.raw",ImageBuff2,SizeI,SizeJ,1);

    /*
    // Count number of point balls:
    char opertion = 'S';
    int objsize = 1;
    int *objsizes_cumu = new int[SizeI*SizeJ];
    objsizes_cumu[0] = 0;
    objsizes_cumu[objsize] = CountSingleDots(ImageBuff2,SizeI,SizeJ);

    int remain_pre = 0;
    int remain = MorphOperation(ImageBuff2,ImageBuff1,SizeI,SizeJ,opertion);
    objsize ++;
    objsizes_cumu[objsize] = CountSingleDots(ImageBuff1,SizeI,SizeJ);
    printf("%d ",remain);
    fflush(stdout);
    int outbuff = 2;

    while(remain != remain_pre)
    {
        remain_pre = remain;
        printf("%d ",remain);
        fflush(stdout);
        if(outbuff == 1)
        {
            remain = MorphOperation(ImageBuff2,ImageBuff1,SizeI,SizeJ,opertion);
            objsize ++;
            objsizes_cumu[objsize] = CountSingleDots(ImageBuff1,SizeI,SizeJ);
            outbuff = 2;
        }
        else
        {
            remain = MorphOperation(ImageBuff1,ImageBuff2,SizeI,SizeJ,opertion);
            objsize ++;
            objsizes_cumu[objsize] = CountSingleDots(ImageBuff2,SizeI,SizeJ);
            outbuff = 1;
        }
    }

    printf("\n");
    for(int i=1;i<=objsize;i++)
        printf("Object of size %d: %d\n",i,objsizes_cumu[i]-objsizes_cumu[i-1]);

    if(outbuff == 1)
    {
        WriteImageToFile("pacman_out.raw",ImageBuff2,SizeI,SizeJ,1);
    }
    else
    {
        WriteImageToFile("pacman_out.raw",ImageBuff1,SizeI,SizeJ,1);
    }
    */

    /*
    // Count number of walls
    for(int i=0;i<5;i++)
    {
        Erode4(ImageBuff2,ImageBuff1,SizeI,SizeJ);
        Erode4(ImageBuff1,ImageBuff2,SizeI,SizeJ);
    }
    int Thres = 1;
    WriteImageToFile("pacman_out.raw",ImageBuff2,SizeI,SizeJ,1);
    printf("Connected objects number(size >= %d): %d\n",Thres,CountConnectedObjects(ImageBuff2,SizeI,SizeJ,Thres));
    */


    // Count number of turns
    int count = 0;
    //count += CountPattern(ImageBuff2,SizeI,SizeJ,0b1111110000100001000010000,5);
    count += CountPattern(ImageBuff2,SizeI,SizeJ,0b0000001111011110111101111,5);

    //count += CountPattern(ImageBuff2,SizeI,SizeJ,0b1000010000100001000011111,5);
    count += CountPattern(ImageBuff2,SizeI,SizeJ,0b0111101111011110111100000,5);

    //count += CountPattern(ImageBuff2,SizeI,SizeJ,0b1111100001000010000100001,5);
    count += CountPattern(ImageBuff2,SizeI,SizeJ,0b0000011110111101111011110,5);

    //count += CountPattern(ImageBuff2,SizeI,SizeJ,0b0000100001000010000111111,5);
    count += CountPattern(ImageBuff2,SizeI,SizeJ,0b1111011110111101111000000,5);

    printf("Number of turns: %d\n",count);

    delete InputImage;
    delete ImageBuff1;
    delete ImageBuff2;
    printf("Process D ends...\n");
}

void test()
{
    int SizeI = 441;
    int SizeJ = 441;

    unsigned char* InputImage = new unsigned char[SizeI*SizeJ*3];
    unsigned char* ImageBuff1 = new unsigned char[SizeI*SizeJ];
    unsigned char* ImageBuff2 = new unsigned char[SizeI*SizeJ];
    ReadImageFromFile("pacman.raw",InputImage,SizeI,SizeJ,3);
    Color2Gray(InputImage,ImageBuff1,SizeI,SizeJ);
    Gray2Binary(ImageBuff1,ImageBuff2,SizeI,SizeJ,15,'L');

    // Fill single hole and Erosion
    for(int i=0; i<5; i++)
    {
        Erode4(ImageBuff2,ImageBuff1,SizeI,SizeJ);
        Erode4(ImageBuff1,ImageBuff2,SizeI,SizeJ);
    }

    WriteImageToFile("test_out.raw",ImageBuff2,SizeI,SizeJ,1);

    delete InputImage;
    delete ImageBuff1;
    delete ImageBuff2;
}

int main()
{
    processA();
    processB();
    processC();
    processD();
//    test();
    return 0;
}