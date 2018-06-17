
#ifndef IMAGEINTERPOLATION_H_
#define IMAGEINTERPOLATION_H_

#include <QString>
#include <QVector>
#include <QImage>

void sampleAndHold(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize);

void bilinearInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize);

double calculateW(double value);

uchar cubicInterpolate(uchar pixels[], double d);

void bicubicWork(uchar colorBuff[], uchar output[], int xSize, int ySize, int newXSize, int newYSize, double horizontalScalingFactor, double verticalScalingFactor, int color);

void bicubicInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize);

void imageRotate(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle);

void imageRotateBilinear(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle);

#endif // IMAGEINTERPOLATION_H_
