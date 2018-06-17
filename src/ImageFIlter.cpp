#include "ImageFilter.h"


void convolve2D (uchar Y_buff[], int xSize, int ySize, double filterCoeff, int N)
{
	//TO DO
}

void extendBorders(uchar input[], int xSize, int ySize, uchar output[], int delta)
{
	int i;

	for (i = 0; i<delta; i++)
	{
		memcpy(&output[i*(xSize + 2 * delta) + delta], &input[0], xSize);
		memcpy(&output[(ySize + delta + i)*(xSize + 2 * delta) + delta], &input[(ySize - 1)*(xSize)], xSize);
	}

	for (i = 0; i<ySize; i++)
	{
		memcpy(&output[(i + delta)*(xSize + 2 * delta) + delta], &input[i*(xSize)], xSize);
	}

	for (i = 0; i<ySize + 2 * delta; i++)
	{
		memset(&output[i*(xSize + 2 * delta)], output[i*(xSize + 2 * delta) + delta], delta);
		memset(&output[i*(xSize + 2 * delta) + xSize + delta], output[i*(xSize + 2 * delta) + xSize + delta - 1], delta);
	}
}

void performNFFilter (uchar input[], int xSize, int ySize)
{
	//TO DO
}

void performVFFilter (uchar input[], int xSize, int ySize)
{
	//TO DO
}

void performSuccessiveVFFilter (uchar input[], int xSize, int ySize, int stages)
{
	//TO DO
}

void performSobelEdgeDetection(uchar input[], int xSize, int ySize, uchar threshold)
{
	//TO DO
}

void performNFplusSobelEdgeDetection(uchar input[], int xSize, int ySize, int stages, uchar threshold)
{
	//TO DO
}
