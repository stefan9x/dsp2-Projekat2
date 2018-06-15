
#include "ImageProcessing.h"
#include "ImageInterpolation.h"
#define N 4
#include <QDebug>

void imageProcessingFun(const QString& progName, QImage* const outImgs, const QImage* const inImgs, const QVector<double>& params) 
{
	int inputXSize = inImgs->width();
	int inputYSize = inImgs->height();

	if(progName == "Sample and hold") 
	{	
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Vertical scale factor is params[0] */
		/* Horizontal scale factor is params[1] */

		/* Calculate output image resolution and construct output image object */
		int tempXSize = inputXSize * params[1];
		int tempYSIze = inputYSize * params[0];
		int outputXSize = tempXSize + (N - (tempXSize % N));
		int outputYSize = tempYSIze + (N - (tempYSIze % N));

		new (outImgs) QImage(outputXSize, outputYSize, inImgs->format());

		/* Perform Sample and hold interpolation  */
		sampleAndHold(inImgs->bits(), inputXSize, inputYSize, outImgs->bits(), outputXSize, outputYSize);

	}
	else if (progName == "Bilinear") 
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Vertical scale factor is params[0] */
		/* Horizontal scale factor is params[1] */

		/* TO DO: Calculate output image resolution and construct output image object */
		int tempXSize = inputXSize * params[1];
		int tempYSIze = inputYSize * params[0];
		int outputXSize = tempXSize + (N - (tempXSize % N));
		int outputYSize = tempYSIze + (N - (tempYSIze % N));

		new (outImgs) QImage(outputXSize, outputYSize, inImgs->format());

		/* TO DO: Perform Bilinear interpolation  */
		bilinearInterpolate(inImgs->bits(), inputXSize, inputYSize, outImgs->bits(), outputXSize, outputYSize);

	}
	else if(progName == "Rotation") 
	{	
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Rotation angle in degrees is params[0]*/
		/* Center of rotation coordinates are (XSIZE/2, YSIZE/2) */

		/* TO DO: Construct output image object */

		/* TO DO: Perform image rotation */
	
	}
	else if (progName == "Rotation Bilinear") 
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Rotation angle in degrees is params[0]*/
		/* Center of rotation coordinates are (XSIZE/2, YSIZE/2) */

		/* TO DO: Construct output image object */

		/* TO DO: Perform image rotation with bilinear interpolation */
	}

}

