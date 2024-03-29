/**
Author: Xiang Gao
USCID:  9216348910
Email:  xianggao@usc.edu
*/

#ifndef __MYFILTERING_H__
#define __MYFILTERING_H__

void ImageExpandRepeat(int Margin, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ, int BytesPerPixel);

void ImageExpandSymmetry(int Margin, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ, int BytesPerPixel);

void ImageExpandBlack(int Margin, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ, int BytesPerPixel);

void Filter(const double *FilterData, int FilterSize, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ, int BytesPerPixel);

void MedianFilter(char FilterShape,int FilterSize, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ, int BytesPerPixel);

void BilateralFilter(int FilterSize,double sigma_d, double sigma_r, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ);

void NLMFilter(int NeighborSize, int SearchWindowSize, double h, const unsigned char* ImageData, unsigned char* ImageDataOut, int SizeI, int SizeJ);

void GaussianFilter(int FilterSize,double sigma, const unsigned char* src, unsigned char* tar, int SizeI, int SizeJ);

void UniformFilter(int FilterSize,const unsigned char* src, unsigned char* tar, int SizeI, int SizeJ);


#endif
