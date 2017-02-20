#ifndef HANDWRITTEN_DIGIT_HEADER
#define HANDWRITTEN_DIGIT_HEADER

#include <armadillo>
#include "Bitmap.h"

using namespace arma;

class HandwrittenDigit
{
public:
	HandwrittenDigit(char * imageBitmapFile);
	HandwrittenDigit(unsigned char * pixelsValue);
	HandwrittenDigit(Col<double> pixelsPercentage);

	Col<double> GetPixelsPercentage();
	char * GetPixelsValue();

	void Save(char * filename);
	void Open(char * filename);

private:
	Col<double> pixels;

};

HandwrittenDigit::HandwrittenDigit(char * imageBitmapFile)
{
	Open(imageBitmapFile);
}
HandwrittenDigit::HandwrittenDigit(unsigned char * pixelsValue)
{
	pixels.set_size(28 /*Pixel Width*/ * 28 /*Pixel Height*/);
	for (int i = 0; i < 28 /*Pixel Width*/ * 28 /*Pixel Height*/; i++)
		pixels[i] = (double)(pixelsValue[i]) / (double)256;
}
HandwrittenDigit::HandwrittenDigit(Col<double> pixelsPercentage)
{
	pixels.set_size(28 /*Pixel Width*/ * 28 /*Pixel Height*/);
	for (int i = 0; i < 28 /*Pixel Width*/ * 28 /*Pixel Height*/; i++)
		pixels[i] = pixelsPercentage[i];
}

Col<double> HandwrittenDigit::GetPixelsPercentage()
{
	return pixels;
}
char * HandwrittenDigit::GetPixelsValue()
{
	char * output = (char *)malloc(28 /*Pixel Width*/ * 28 /*Pixel Height*/);

	for (int i = 0; i < 28 /*Pixel Width*/ * 28 /*Pixel Height*/; i++)
		output[i] = (unsigned char)(pixels[i] * (double)256);

	return output;
}

void HandwrittenDigit::Open(char * imageBitmapFile)
{
	pixels.set_size(28 /*Pixel Width*/ * 28 /*Pixel Height*/);

	Bitmap bmp(imageBitmapFile);

	char digitData[28 /*Pixel Width*/ * 28 /*Pixel Height*/ * 3 /*RGB*/];

	int width = 28, height = 28;

	memcpy(digitData, bmp.GetPixels(&width, &height), width * height * 3 /*RGB*/);

	char * newDigit = (char *)malloc(784);
	char * digit = (char *)malloc(784);
	
	for (int i = 0, j = 783; i < 784; i++, j--)
		newDigit[i] = digitData[j * 3];

	for (int i = 0; i < 28; i++)
		for (int j = 0, k = 27; j < 28; j++, k--)
			digit[k + i * 28] = newDigit[j + i * 28];

	for (int i = 0; i < 784; i++)
		pixels[i] = (double)((unsigned char)(255 - digit[i])) / (double)256;
}
void HandwrittenDigit::Save(char * imageBitmapFile)
{
	char digit[28 /*Pixel Width*/ * 28 /*Pixel Height*/];
	memcpy(digit, GetPixelsValue(), 28 /*Pixel Width*/ * 28 /*Pixel Height*/);
	
	char * digitData = (char *)malloc(2352);
	char * newdigit = (char *)malloc(784);

	for (int i = 0; i < 784; i++)
		digit[i] = 255 - digit[i];

	for (int i = 0; i < 28; i++)
		for (int j = 0, k = 27; j < 28; j++, k--)
			newdigit[j + i * 28] = digit[k + i * 28];

	for (int i = 0, j = 783; i < 784; i++, j--)
		digitData[j * 3] = newdigit[i];
	
	Bitmap bmp(28, 28, (unsigned char *)digitData);
	bmp.Save(imageBitmapFile);
	
	free(digitData);
	free(newdigit);
}

#endif