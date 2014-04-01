/**
EE569 HW #1 Problem 1: Image Manipulation
Author: Xiang Gao
USCID:  9216348910
Email:  xianggao@usc.edu
Submission Date: Sep 19, 2013
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "MyIO.h"

using namespace std;

struct Point
{
    int i;
    int j;
};

// (a) Blue Screen Technique
void processA()
{
    printf("Part A begins:\n");
    int BytesPerPixel = 3;
    int Size = 257; // "shapes.raw" file is a 257*257*3 image
    unsigned char ImageData[Size][Size][BytesPerPixel]; // Allocate image data array
    string InputFileName = "shapes.raw";
    string OutputFileName = "shapes_a_out.raw";

    // Read image from file
    ReadImageFromFile(InputFileName.c_str(),&ImageData[0][0][0],Size,Size,BytesPerPixel);

    // Processing codes begin here:
    int histogram[3][256]; // Array used to store histograms of RGB
    memset(histogram,0,3*256*sizeof(int)); // Initialize it into 0
    for(int i=0; i<Size; i++)
    {
        for(int j=0; j<Size; j++)
        {
            histogram[0][ImageData[i][j][0]] ++;
            histogram[1][ImageData[i][j][1]] ++;
            histogram[2][ImageData[i][j][2]] ++;
        }
    }
    /*
    printf("Part (a): Histograms are as follows:\n");
    printf("%6s\t%10s%10s%10s\n","Index:","R:","G:","B:");
    for(int i=0; i<256; i++)
    {
        printf("%6d\t%10d%10d%10d\n",i,histogram[0][i],histogram[1][i],histogram[2][i]);
    }
    */
    WriteHistogramToFile("shapes_histdata_r.dat",&histogram[0][0]);
    WriteHistogramToFile("shapes_histdata_g.dat",&histogram[1][0]);
    WriteHistogramToFile("shapes_histdata_b.dat",&histogram[2][0]);
    /*
    Based on the above histogram, I set the RGB range of the backgound as follows:
    R: [0,30]; G: [0,50]; B: [200,255]
    */
    for(int i=0; i<Size; i++)
    {
        for(int j=0; j<Size; j++)
        {
            if(ImageData[i][j][0]>=0&&ImageData[i][j][0]<=30&&
                    ImageData[i][j][1]>=0&&ImageData[i][j][1]<=50&&
                    ImageData[i][j][2]>=200&&ImageData[i][j][2]<=255) // Detect background
            {
                ImageData[i][j][0] = 255;
                ImageData[i][j][1] = 255;
                ImageData[i][j][2] = 255;
            }
        }
    }

    // Write image data (filename specified by second argument) from image data matrix
    WriteImageToFile(OutputFileName.c_str(),&ImageData[0][0][0],Size,Size,BytesPerPixel);

    printf("Part A ends.\n");
}

// (b) Identifying Geometrical Objects
// Define a global variable for storing the beginning pixel of each object. This will be used in (c)
int ObjectNumber = 0; // Store the number of objects
int ObjectPixel[20]; // First pixel of each object
char ObjectShape[20]; // Object shape: S for Square, C for Circle
void processB()
{
    printf("Part B begins:\n");
    int BytesPerPixel = 3;
    int Size = 257; // A 257*257*3 image
    unsigned char ImageData[Size][Size][BytesPerPixel]; // Allocate image data array
    string InputFileName = "shapes_a_out.raw"; // Output file from (a) is the input file here

    // Read image from file
    ReadImageFromFile(InputFileName.c_str(),&ImageData[0][0][0],Size,Size,BytesPerPixel);

    // Processing codes begin here:
    int CircleNum = 0;
    int SquareNum = 0;
    int CircleRadius[20];
    int SquareSideLen[20];
    int centerI = Size/2;
    int centerJ = Size/2;
    int l = 0;
    for(int j=centerJ; j<Size; j++,l++)
    {
        if(ImageData[centerI][j][0]!=255 ||
                ImageData[centerI][j][1]!=255 ||
                ImageData[centerI][j][2]!=255)
        {
            if(ImageData[centerI][j-1][0]==255 &&
                    ImageData[centerI][j-1][1]==255 &&
                    ImageData[centerI][j-1][2]==255)
            {
                ObjectPixel[ObjectNumber] = l;
                // Check if is a square
                bool isSquare = true;
                for(int k=-l; k<=l; k++)
                {
                    if(ImageData[centerI-l][centerJ+k][0]==255&&
                            ImageData[centerI-l][centerJ+k][1]==255&&
                            ImageData[centerI-l][centerJ+k][2]==255)
                        isSquare = false;
                    if(ImageData[centerI+l][centerJ+k][0]==255&&
                            ImageData[centerI+l][centerJ+k][1]==255&&
                            ImageData[centerI+l][centerJ+k][2]==255)
                        isSquare = false;
                    if(ImageData[centerI+k][centerJ-l][0]==255&&
                            ImageData[centerI+k][centerJ-l][1]==255&&
                            ImageData[centerI+k][centerJ-l][2]==255)
                        isSquare = false;
                    if(ImageData[centerI+k][centerJ+l][0]==255&&
                            ImageData[centerI+k][centerJ+l][1]==255&&
                            ImageData[centerI+k][centerJ+l][2]==255)
                        isSquare = false;
                }
                if(isSquare)
                {
                    SquareSideLen[SquareNum] = l;
                    SquareNum ++;
                    ObjectShape[ObjectNumber] = 'S';
                }
                else
                {
                    CircleRadius[CircleNum] = l;
                    CircleNum ++;
                    ObjectShape[ObjectNumber] = 'C';
                }
                ObjectNumber ++;
            }
        }
    }
    printf("The total number of circles is: %d , and their respective radius is: ",CircleNum);
    for(int i=0; i<CircleNum; i++)
    {
        cout<<CircleRadius[i]<<" ";
    }
    cout<<endl;
    printf("The total number of squares is: %d , and their respective side length is: ",SquareNum);
    for(int i=0; i<SquareNum; i++)
    {
        cout<<SquareSideLen[i]*2<<" ";
    }
    cout<<endl;
    printf("The total number of geometrical objects is: %d \n",ObjectNumber);
    printf("Part B ends.\n");
}

// (c) Extracting Geometrical Objects
void processC()
{
    printf("Part C begins:\n");
    if(ObjectNumber==0)
    {
        printf("No Object.\n");
        return;
    }
    int BytesPerPixel = 3;
    int Size = 257;
    unsigned char ImageData[Size][Size][BytesPerPixel]; // Allocate image data array
    unsigned char ImageDataOut[Size][Size][BytesPerPixel]; // Allocate image data array for output
    bool Checker[Size][Size]; // Used to determine whether pixel is used already
    string InputFileName = "shapes_a_out.raw"; // Use result of (a) as input
    string OutputFileName = "shapes_c_out.raw";

    // Read image from file
    ReadImageFromFile(InputFileName.c_str(),&ImageData[0][0][0],Size,Size,BytesPerPixel);

    memset(ImageDataOut,255,Size*Size*BytesPerPixel);
    memset(Checker,0,Size*Size*sizeof(bool));

    // Processing codes begin here:
    int neighbor8X[] = {-1,-1,-1,0,0,1,1,1};
    int neighbor8Y[] = {-1,0,1,-1,1,-1,0,1};
    printf("Please selete an object to print[1-%d:",ObjectNumber);
    for(int i=0; i<ObjectNumber; i++)
    {
        printf("%c ",ObjectShape[i]);
    }
    printf("]: ");
    int N;
    scanf("%d",&N);
    // Print the Nth object to the output file
    Point pointStack[Size*Size];
    int pointStackIndex = 0;
    pointStack[pointStackIndex].i = Size/2;
    pointStack[pointStackIndex].j = Size/2+ObjectPixel[N-1];
    pointStackIndex ++;
    Checker[pointStack[pointStackIndex-1].i][pointStack[pointStackIndex-1].j] = true;
    while(pointStackIndex!=0)
    {
        Point currentP;
        currentP.i = pointStack[pointStackIndex-1].i;
        currentP.j = pointStack[pointStackIndex-1].j;
        pointStackIndex --;
        ImageDataOut[currentP.i][currentP.j][0] = ImageData[currentP.i][currentP.j][0];
        ImageDataOut[currentP.i][currentP.j][1] = ImageData[currentP.i][currentP.j][1];
        ImageDataOut[currentP.i][currentP.j][2] = ImageData[currentP.i][currentP.j][2];
        for(int i=0; i<8; i++)
        {
            if(ImageData[currentP.i+neighbor8X[i]][currentP.j+neighbor8Y[i]][0]!=255||
                    ImageData[currentP.i+neighbor8X[i]][currentP.j+neighbor8Y[i]][1]!=255||
                    ImageData[currentP.i+neighbor8X[i]][currentP.j+neighbor8Y[i]][2]!=255)
            {
                if(Checker[currentP.i+neighbor8X[i]][currentP.j+neighbor8Y[i]] == false)
                {
                    pointStack[pointStackIndex].i = currentP.i + neighbor8X[i];
                    pointStack[pointStackIndex].j = currentP.j + neighbor8Y[i];
                    pointStackIndex ++;
                    Checker[pointStack[pointStackIndex-1].i][pointStack[pointStackIndex-1].j] = true;
                }
            }
        }
    }

    // Write image data (filename specified by second argument) from image data matrix
    WriteImageToFile(OutputFileName.c_str(),&ImageDataOut[0][0][0],Size,Size,BytesPerPixel);

    printf("Part C ends.\n");
}

// (d) Smart Camera - Moving Objects Removal
void processD()
{
    printf("Part D begins:\n");
    int BytesPerPixel = 3;
    int SizeI = 453;
    int SizeJ = 750; // whitehouse image is 453*750
    unsigned char ImageData1[SizeI][SizeJ][BytesPerPixel]; // Allocate image data array
    unsigned char ImageData2[SizeI][SizeJ][BytesPerPixel]; // Allocate image data array
    unsigned char ImageDataOut[SizeI][SizeJ][BytesPerPixel]; // Allocate image data array for output
    string InputFileName[] = {"firstclick.raw","secondclick.raw","thirdclick.raw","fourthclick.raw",
                              "fifthclick.raw","sixthclick.raw","seventhclick.raw","eighthclick.raw"
                             }; //
    string OutputFileName = "whitehouse_out.raw";

    // Initialize output image to 0
    memset(ImageDataOut,0,sizeof(unsigned char)*SizeI*SizeJ*BytesPerPixel);
    // Processing codes begin here:
    bool ImageChecker[SizeI][SizeJ]; // To check if this pixel is already filled
    memset(ImageChecker,0,sizeof(bool)*SizeI*SizeJ);
    for(int i=0; i<8; i++)
    {
        for(int j=i+1; j<8; j++)
        {
            // Read image from 2 files
            ReadImageFromFile(InputFileName[i].c_str(),&ImageData1[0][0][0],SizeI,SizeJ,BytesPerPixel);

            ReadImageFromFile(InputFileName[j].c_str(),&ImageData2[0][0][0],SizeI,SizeJ,BytesPerPixel);

            // Compare two images far to each other, store the same pixel in the output file
            for(int m=0; m<SizeI; m++)
            {
                for(int n=0; n<SizeJ; n++)
                {
                    if(ImageData1[m][n][0]==ImageData2[m][n][0] &&
                            ImageData1[m][n][1]==ImageData2[m][n][1] &&
                            ImageData1[m][n][2]==ImageData2[m][n][2] &&
                            ImageChecker[m][n]==false)
                    {
                        ImageDataOut[m][n][0] = ImageData1[m][n][0];
                        ImageDataOut[m][n][1] = ImageData1[m][n][1];
                        ImageDataOut[m][n][2] = ImageData1[m][n][2];
                        ImageChecker[m][n] = true;
                    }
                }
            }
        }
    }

    // Write image data (filename specified by second argument) from image data matrix
    WriteImageToFile(OutputFileName.c_str(),&ImageDataOut[0][0][0],SizeI,SizeJ,BytesPerPixel);

    // Get superman..
    int thres = 20; // need this threshold to get clean superman
    ReadImageFromFile(OutputFileName.c_str(),&ImageData2[0][0][0],SizeI,SizeJ,BytesPerPixel);
    int area = 0; // Store the superman area
    int imgNum = 0;
    for(int i=0; i<8; i++)
    {
        int lt_i = SizeI, lt_j = SizeJ, rb_i = 0, rb_j = 0;
        ReadImageFromFile(InputFileName[i].c_str(),&ImageData1[0][0][0],SizeI,SizeJ,BytesPerPixel);
        for(int m=0; m<SizeI; m++)
        {
            for(int n=0; n<SizeJ; n++)
            {
                if(abs(ImageData1[m][n][0]-ImageData2[m][n][0])>thres ||
                   abs(ImageData1[m][n][1]-ImageData2[m][n][1])>thres ||
                   abs(ImageData1[m][n][2]-ImageData2[m][n][2])>thres) // need a threshold here
                {
                    if(m<=lt_i)
                        lt_i = m;
                    if(m>=rb_i)
                        rb_i = m;
                    if(n<=lt_j)
                        lt_j = n;
                    if(n>=rb_j)
                        rb_j = n;
                }
            }
        }
        int tmp = abs(rb_i-lt_i)*(rb_j-lt_j);
        if(tmp > area)
        {
            area = tmp;
            imgNum = i;
        }
    }
    // Read the image with the largest superman
    ReadImageFromFile(InputFileName[imgNum].c_str(),&ImageData1[0][0][0],SizeI,SizeJ,BytesPerPixel);
    for(int m=0; m<SizeI; m++)
    {
        for(int n=0; n<SizeJ; n++)
        {
            if(abs(ImageData1[m][n][0]-ImageData2[m][n][0])<=thres &&
            abs(ImageData1[m][n][1]-ImageData2[m][n][1])<=thres &&
            abs(ImageData1[m][n][2]-ImageData2[m][n][2])<=thres)
            {
                ImageData1[m][n][0] = 255;
                ImageData1[m][n][1] = 255;
                ImageData1[m][n][2] = 255;
            }
        }
    }
    WriteImageToFile("SuperMan_out.raw",&ImageData1[0][0][0],SizeI,SizeJ,BytesPerPixel);

    // Now superman is stored in ImageData1.

    int SizeI_dog = 507;
    int SizeJ_dog = 900;
    unsigned char ImageDataDog[SizeI_dog][SizeJ_dog][BytesPerPixel]; // Allocate image data array

    ReadImageFromFile("dogjump.raw",&ImageDataDog[0][0][0],SizeI_dog,SizeJ_dog,BytesPerPixel);

    for(int m=0; m<SizeI; m++)
    {
        for(int n=0; n<SizeJ; n++)
        {
            int offsetV = 200;
            int offsetH = 200; // Used to adjust the position of Superman
            if(ImageData1[m][n][0]!=255 ||
                    ImageData1[m][n][1]!=255 ||
                    ImageData1[m][n][2]!=255)
            {
                if(m+offsetV<SizeI_dog && n+offsetH<SizeJ_dog)
                {
                    ImageDataDog[m+offsetV][n+offsetH][0] = ImageData1[m][n][0];
                    ImageDataDog[m+offsetV][n+offsetH][1] = ImageData1[m][n][1];
                    ImageDataDog[m+offsetV][n+offsetH][2] = ImageData1[m][n][2];
                }
            }
        }
    }

    WriteImageToFile("dogjump_out.raw",&ImageDataDog[0][0][0],SizeI_dog,SizeJ_dog,BytesPerPixel);

    printf("Part D ends.\n");
}

int main()
{
    processA();
    processB();
    processC();
    processD();
    return 0;
}



