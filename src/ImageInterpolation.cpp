#include "ImageInterpolation.h"
#include "ColorSpaces.h"
#include "ImageFilter.h"
#include <math.h>
#include <thread>

void sampleAndHold(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	double horizontalScalingFactor = (double)newXSize / xSize;
	double verticalScalingFactor = (double)newYSize / ySize;

	for (int y = 0; y < newYSize; y++)
	{
		for (int x = 0; x < newXSize; x++)
		{

			int closestX = (x - 1) / horizontalScalingFactor + 1;
			int closestY = (y - 1) / verticalScalingFactor + 1;

			if (closestX >= xSize) {
				closestX = (x - 1) / horizontalScalingFactor;
			}

			if (closestY >= ySize) {
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
	double horizontalScalingFactor = (double) newXSize / xSize;
	double verticalScalingFactor = (double) newYSize / ySize;

	for (int y = 0; y < newYSize; y++)
	{
		for (int x = 0; x < newXSize; x++)
		{
			double a = x / horizontalScalingFactor - floor(x / horizontalScalingFactor);
			double b = y / verticalScalingFactor - floor(y / verticalScalingFactor);

			int m = x / horizontalScalingFactor;
			int n = y / verticalScalingFactor;

			int m2 = m + 1;

			if (m2 == xSize) {
				m2 = m;
			}

			int n2 = n + 1;

			if (n2 == ySize) {
				n2 = n;
			}
			
			output[y * 3 * newXSize + x * 3] = (1 - a) * (1 - b) * input[n * xSize * 3 + m * 3] +
											   a * (1 - b) * input[n * xSize * 3 + m2 * 3] +
											   (1 - a) * b * input[n2 * xSize * 3 + m * 3] +
												a * b * input[n2 * xSize * 3 + m2 * 3];

			output[y * 3 * newXSize + x * 3 + 1] = (1 - a) * (1 - b) * input[n * xSize * 3 + m * 3 + 1] +
												   a * (1 - b) * input[n * xSize * 3 + m2 * 3 + 1] +
												   (1 - a) * b * input[n2 * xSize * 3 + m * 3 + 1] +
												    a * b * input[n2 * xSize * 3 + m2 * 3 + 1];

			output[y * 3 * newXSize + x * 3 + 2] = (1 - a) * (1 - b) * input[n * xSize * 3 + m * 3 + 2] +
												   a * (1 - b) * input[n * xSize * 3 + m2 * 3 + 2] +
											       (1 - a) * b * input[n2 * xSize * 3 + m * 3 + 2] +
												    a * b * input[n2 * xSize * 3 + m2 * 3 + 2];
		}
	}
}

double calculateW(double value) {

	double absValue = abs(value);
	if (absValue < 1) {
		return (1.5 * (absValue * absValue * absValue)) - (2.5 * (absValue * absValue)) + 1;
	}
	else if ((absValue >= 1) && (absValue < 2)) {
		return (-0.5 * (absValue * absValue * absValue)) + (2.5 * (absValue * absValue)) - (4 * absValue) + 2;
	}
	else
	{
		return 0;
	}
};

uchar cubicInterpolate(uchar pixels[], double value) {
	double w[4];
	int ret = 0;

	w[0] = calculateW(value + 1);
	w[1] = calculateW(value);
	w[2] = calculateW(1 - value);
	w[3] = calculateW(2 - value);

	for (int i = 0; i < 4; i++) {
		ret += pixels[i] * w[i];
	}

	if (ret > 255) {
		ret = 255;
	}
	else  if (ret < 0) {
		ret = 0;
	}

	return (uchar)ret;
}

void bicubicWork(uchar colorBuff[], uchar output[], int xSize, int ySize, int newXSize, int newYSize, double horizontalScalingFactor, double verticalScalingFactor, int color) {
	
	uchar *horizontalPixels = new uchar[4];
	uchar *verticalPixels = new uchar[4];

	uchar *extendedColorBuff = new uchar[(xSize + 4) * (ySize + 4)];
	extendBorders(colorBuff, xSize, ySize, extendedColorBuff, 2);

	for (int y = 0; y < newYSize; y++) {
		for (int x = 0; x < newXSize; x++) {
			double b = x / horizontalScalingFactor - floor(x / horizontalScalingFactor);
			double a = y / verticalScalingFactor - floor(y / verticalScalingFactor);

			int bicubicY = y / verticalScalingFactor;
			int bicubicX = x / horizontalScalingFactor;

			int v = 0;

			for (int h = bicubicY - 1; h < bicubicY + 3; h++, v++) {

				horizontalPixels[0] = extendedColorBuff[h * (xSize + 4) + bicubicX - 1];
				horizontalPixels[1] = extendedColorBuff[h * (xSize + 4) + bicubicX];
				horizontalPixels[2] = extendedColorBuff[h * (xSize + 4) + bicubicX + 1];
				horizontalPixels[3] = extendedColorBuff[h * (xSize + 4) + bicubicX + 2];

				verticalPixels[v] = cubicInterpolate(horizontalPixels, b);

			}

			output[y * 3 * newXSize + x * 3 + color] = cubicInterpolate(verticalPixels, a);

		}
	}

	delete[]horizontalPixels;
	delete[]verticalPixels;
	delete[]extendedColorBuff;
}

void bicubicInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	uchar *rBuff = new uchar[xSize * ySize];
	uchar *gBuff = new uchar[xSize * ySize];
	uchar *bBuff = new uchar[xSize * ySize];

	for (int y = 0; y < ySize; y++) {
		for (int x = 0; x < xSize; x++) {
			rBuff[y * xSize + x] = input[y * xSize * 3 + x * 3];
			gBuff[y * xSize + x] = input[y * xSize * 3 + x * 3 + 1];
			bBuff[y * xSize + x] = input[y * xSize * 3 + x * 3 + 2];
		}
	}

	double horizontalScalingFactor = (double)newXSize / xSize;
	double vericalScalingFactor = (double)newYSize / ySize;

	std::thread redThread(bicubicWork, rBuff, output, xSize, ySize, newXSize, newYSize, horizontalScalingFactor, vericalScalingFactor, 0);
	std::thread greenThread(bicubicWork, gBuff, output, xSize, ySize, newXSize, newYSize, horizontalScalingFactor, vericalScalingFactor, 1);
	std::thread blueThread(bicubicWork, bBuff, output, xSize, ySize, newXSize, newYSize, horizontalScalingFactor, vericalScalingFactor, 2);
	redThread.join();
	greenThread.join();
	blueThread.join();

	delete[]rBuff;
	delete[]gBuff;
	delete[]bBuff;	
}

void imageRotate(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	double theta = (360-angle) * 3.141592 / 180;

	for (int y = 0; y < ySize; y++)
	{
		for (int x = 0; x < xSize; x++)
		{
			double newPosY = y * cos(theta) + x * sin(theta) - m * sin(theta) - n * cos(theta) + n;
			double newPosX = x * cos(theta) - y * sin(theta) - m * cos(theta) + n * sin(theta) + m;

			int closestX = floor(newPosX);
			int closestY = floor(newPosY);

			if (closestX == xSize) {
				closestX = floor(newPosX) - 1;
			}

			if (closestY == ySize) {
				closestY = floor(newPosY) - 1;
			}

			if (newPosY < 0 || newPosY > ySize - 1 || newPosX < 0 || newPosX > xSize - 1) {
				output[y * 3 * xSize + x * 3] = 0; //R
				output[y * 3 * xSize + x * 3 + 1] = 0; //G
				output[y * 3 * xSize + x * 3 + 2] = 0; //B
			}
			else {
				output[y * 3 * xSize + x * 3] = input[closestY * 3 * xSize + closestX * 3]; //R
				output[y * 3 * xSize + x * 3 + 1] = input[closestY * 3 * xSize + closestX * 3 + 1]; //G
				output[y * 3 * xSize + x * 3 + 2] = input[closestY * 3 * xSize + closestX * 3 + 2]; //B
			}
		}
	}
}

void imageRotateResize(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle, int newXSize, int newYSize)
{
	double theta = angle * 3.141592 / 180;

	int translateX = round((newXSize - xSize) / 2);
	int translateY = round((newYSize - ySize) / 2);

	for (int y = 0; y < ySize; y++)
	{
		for (int x = 0; x < xSize; x++)
		{
			int newPosY = round(y * cos(theta) + x * sin(theta) - m * sin(theta) - n * cos(theta) + n) + translateY;
			int newPosX = round(x * cos(theta) - y * sin(theta) - m * cos(theta) + n * sin(theta) + m) + translateX;

			output[newPosY * 3 * newXSize + newPosX * 3] = input[y * 3 * xSize + x * 3]; //R
			output[newPosY * 3 * newXSize + newPosX * 3 + 1] = input[y * 3 * xSize + x * 3 + 1]; //G
			output[newPosY * 3 * newXSize + newPosX * 3 + 2] = input[y * 3 * xSize + x * 3 + 2]; //B

		}
	}
}

void imageRotateBilinear(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	double theta = (360 - angle) * 3.141592 / 180;

	for (int y = 0; y < ySize; y++)
	{
		for (int x = 0; x < xSize; x++)
		{
			double newPosY = y * cos(theta) + x * sin(theta) - m * sin(theta) - n * cos(theta) + n;
			double newPosX = x * cos(theta) - y * sin(theta) - m * cos(theta) + n * sin(theta) + m;

			double a = newPosX - floor(newPosX);
			double b = newPosY - floor(newPosY);

			int firstX = floor(newPosX);
			int firstY = floor(newPosY);

			int secondX = firstX + 1;

			if (secondX == xSize) {
				secondX = firstX;
			}

			int secondY = firstY + 1;

			if (secondY == ySize) {
				secondY = firstY;
			}

			if (newPosY < 0 || newPosY > ySize - 1 || newPosX < 0 || newPosX > xSize - 1) {
				output[y * 3 * xSize + x * 3] = 0; //R
				output[y * 3 * xSize + x * 3 + 1] = 0; //G
				output[y * 3 * xSize + x * 3 + 2] = 0; //B
			}
			else {
				output[y * 3 * xSize + x * 3] = (1 - a) * (1 - b) * input[firstY * xSize * 3 + firstX * 3] +
												a * (1 - b) * input[firstY * xSize * 3 + secondX * 3] +
												(1 - a) * b * input[secondY * xSize * 3 + firstX * 3] +
												a * b * input[secondY * xSize * 3 + secondX * 3];
				output[y * 3 * xSize + x * 3 + 1] = (1 - a) * (1 - b) * input[firstY * xSize * 3 + firstX * 3 + 1] +
													a * (1 - b) * input[firstY * xSize * 3 + secondX * 3 + 1] +
													(1 - a) * b * input[secondY * xSize * 3 + firstX * 3 + 1] +
													a * b * input[secondY * xSize * 3 + secondX * 3 + 1];
				output[y * 3 * xSize + x * 3 + 2] = (1 - a) * (1 - b) * input[firstY * xSize * 3 + firstX * 3 + 2] +
													a * (1 - b) * input[firstY * xSize * 3 + secondX * 3 + 2] +
													(1 - a) * b * input[secondY * xSize * 3 + firstX * 3 + 2] +
													a * b * input[secondY * xSize * 3 + secondX * 3 + 2];
			}
		}
	}
}