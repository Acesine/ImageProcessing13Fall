/**
Author: Xiang Gao
USCID:  9216348910
Email:  xianggao@usc.edu
*/

#ifndef __MYIO_H__
#define __MYIO_H__

void ReadImageFromFile(const char* filename,unsigned char *ImageData,int SizeI,int SizeJ,int BytesPerPixel);

void WriteImageToFile(const char* filename,const unsigned char *ImageData,int SizeI,int SizeJ,int BytesPerPixel);

void GetHistogram(const unsigned char *ImageData,int SizeI,int SizeJ, int *histogram);

void WriteHistogramToFile(const char* filename, const int *histogram);

void WriteTransferToFile(const char* filename, const int *Transfer);

void WriteDataToFile(const char* filename, const int *data, int length);

#endif
