/**
Author: Xiang Gao
USCID:  9216348910
Email:  xianggao@usc.edu
*/

#ifndef __MYFILTERING_H__
#define __MYFILTERING_H__

void ImageExtendRepeat(int Margin, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ, int BytesPerPixel);

void ImageExtendSymmetry(int Margin, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ, int BytesPerPixel);

void Filter(const double *FilterData, int FilterSize, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ, int BytesPerPixel);

void MedianFilter(char FilterShape,int FilterSize, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ, int BytesPerPixel);

void BilateralFilter(int FilterSize,double sigma_d, double sigma_r, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ);

void NLMFilter(int NeighborSize, int SearchWindowSize, double h, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ);

#endif
