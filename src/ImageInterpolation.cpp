#include "ImageInterpolation.h"
#include "ColorSpaces.h"
#include <math.h>

void sampleAndHold(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	const double horizontalScalingFactor = (double)newXSize / xSize;
	const double verticalScalingFactor = (double)newYSize / ySize;

	for (int x = 0; x < newXSize; x++)
	{
		for (int y = 0; y < newYSize; y++)
		{

			int closestX = (x - 1) / horizontalScalingFactor + 1;
			int closestY = (y - 1) / verticalScalingFactor + 1;

			if (closestX == xSize) {
				closestX = (x - 1) / horizontalScalingFactor;
			}

			if (closestY == ySize) {
				closestY = (y - 1) / verticalScalingFactor;
			}
		
			output[y * 3 * newXSize + x * 3] = input[closestY * 3 * xSize + closestX * 3]; //R
			output[y * 3 * newXSize + x * 3 + 1] = input[closestY * 3 * xSize + closestX * 3 + 1]; //G
			output[y * 3 * newXSize + x * 3 + 2] = input[closestY * 3 * xSize + closestX * 3 + 2]; //B
		}
	}
}

void bilinearInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
}

void bicubicInterpolate(uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
}

void imageRotate(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	/* TO DO */
}

void imageRotateBilinear(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	/* TO DO */
}