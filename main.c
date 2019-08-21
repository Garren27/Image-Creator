#include "libpnm.h"
#include <stdio.h>

//Implementation of Bresenham's Line Drawing algorithm for the case where 0 <= slope <= 1
int prog1SlopeBetween01(int width, int height, struct PBM_Image *pbmPtr){
	int hQuarter = height / 4;
	int wQuarter = width / 4;

	int startHeight = hQuarter;
	int endHeight = height - hQuarter - 1;

	int startWidth = wQuarter;
	int endWidth = width - wQuarter - 1;
	int deltaX, deltaY, xc, yc, pAti;
	int x1 = startWidth;
	int y1 = startHeight;
	
	yc = y1;
	deltaX = endWidth - x1;
	deltaY = endHeight - y1;
	pAti = (2 * deltaY) - deltaX; // Used to calculate offset to test where the pixel in the line should go

	for (xc = x1; xc <= endWidth; xc++){ // Loop through Bresenham's algorithm in the 0 < slope < 1 case
		pbmPtr->image[yc][xc] = 1;
		pbmPtr->image[height - yc - 1][xc] = 1; // Performs a calculation to plot the pixel on the opposite side
		if(pAti<0)
 			pAti += 2 * deltaY;
		else{
 			yc++; //Increment y to head to the bottom of the screen
 			pAti += 2 * (deltaY-deltaX);
		}
	}

	return 0;
}

//Implementation of Bresenham's Line Drawing algorithm for the case where slope >= 1
int prog1SlopeGreater1(int width, int height, struct PBM_Image *pbmPtr){
	int hQuarter = height / 4;
	int wQuarter = width / 4;

	int startHeight = hQuarter;
	int endHeight = height - hQuarter - 1;

	int startWidth = wQuarter;
	int endWidth = width - wQuarter - 1;
	int deltaX, deltaY, xc, yc, pAti;
	int x1 = startWidth;
	int y1 = startHeight;
	
	xc = x1;
	deltaX = endWidth - x1;
	deltaY = endHeight - y1;
	pAti = (2 * deltaX) - deltaY;
	for (yc = y1; yc <= endHeight; yc++){ //Loop through Bresenham's algorithm for the case where slope > 1
		pbmPtr->image[yc][xc] = 1;
		pbmPtr->image[height - yc - 1][xc] = 1; //Perform the calculation to plot the pixel on the opposite side
		if(pAti<0)
			pAti += 2 * deltaX;
		else{
 			xc++; //Increment x to head to the right of the screen
 			pAti += 2 * (deltaX - deltaY);
		}
	}
	
	return 0;
}

// Function for creating the Binary Image
int program1(int width, int height, int formatCode, char outputName[]){
	//Determine if the format is raw or not
	int raw;
	if (formatCode == 1)
		raw = 1;
	else
		raw = 0;

	struct PBM_Image pbmIm;
	struct PBM_Image *pbmPtr;
	pbmPtr = &pbmIm;

	//Library call to initialize the PBM_Image struct
	create_PBM_Image(pbmPtr, width, height);

	//Set the whole image to black
	for (int row = 0; row < height; row++){
		for (int col = 0; col < width; col++){
			pbmIm.image[row][col] = 1;
		}
	}

	//Calculate the area that should be white
	int hQuarter = height / 4;
	int wQuarter = width / 4;

	int startHeight = hQuarter;
	int endHeight = height - hQuarter - 1;

	int startWidth = wQuarter;
	int endWidth = width - wQuarter - 1;

	//Set the area that should be white, to white
	for (int i = startHeight; i <= endHeight; i++){
		for (int j = startWidth; j <= endWidth; j++){
			pbmIm.image[i][j] = 0;
			
		}
	}
	// Determine the slope so we know which version of Bresenham's algorithm to use
	float slope = ((float)(endHeight + 1 - startHeight)) / ((float)(endWidth + 1 - startWidth));

	if (slope >= 0 && slope <= 1){
		prog1SlopeBetween01(width, height, pbmPtr);
	}
	else{
		prog1SlopeGreater1(width, height, pbmPtr);
	}

	//Save the image and free memory
	save_PBM_Image(pbmPtr, outputName, raw);
	free_PBM_Image(pbmPtr);
	return 0;
}


int prog2SlopeBetween01(int height, int width, struct PGM_Image * pgmPtr){
	float widthOffset = 255.0/(width/4);
	float widthStep = 0.0;

	int color = 255;

	int hQuarter = height / 4;
	int wQuarter = width / 4;

	int startHeight = hQuarter;
	int endHeight = height - hQuarter - 1;

	int startWidth = wQuarter;
	int endWidth = width - wQuarter - 1;

	// Implementation of Bresenham's algorithm from CS 3388
	int deltaX, deltaY, xc, yc, pAti;
	int x1 = startWidth;
	int y1 = startHeight;
		
	yc = y1;
	deltaX = endWidth - x1;
	deltaY = endHeight - y1;
	pAti = (2 * deltaY) - deltaX; // Used to calculate offset to test where the next line pixel should go

	for (xc = x1; xc <= endWidth; xc++){ // Loop through Bresenham's algorithm in the 0 < slope < 1 case
		for (int i = (yc + 1); i < (height - yc - 1); i++){
			pgmPtr->image[i][xc] = color - widthStep;
			pgmPtr->image[i][endWidth - (xc - startWidth) - 1] = color - widthStep; // Map the left and right sides of the square
		}
		for (int i = xc ; i < endWidth - (xc - startWidth); i++){
			pgmPtr->image[yc][i] = color - widthStep;
			pgmPtr->image[endHeight - (yc - startHeight)][i] = color - widthStep; // Map the top and bottom sides of the square
		}
			
		widthStep += widthOffset;

		if(pAti<0)
 			pAti += 2 * deltaY;
		else{
 			yc++; //Increment y to head to the top of the screen
 			pAti += 2 * (deltaY-deltaX);
		}
	}
	return 0;
}

int prog2SlopeGreater1(int height, int width, struct PGM_Image * pgmPtr){
	float heightOffset = 255.0/(height/4);
	float heightStep = 0.0;

	int color = 255;

	int hQuarter = height / 4;
	int wQuarter = width / 4;

	int startHeight = hQuarter;
	int endHeight = height - hQuarter - 1;

	int startWidth = wQuarter;
	int endWidth = width - wQuarter - 1;

	// Implementation of Bresenham's algorithm from CS 3388
	int deltaX, deltaY, xc, yc, pAti;
	int x1 = startWidth;
	int y1 = startHeight;
		
	xc = x1;
	deltaX = endWidth - x1;
	deltaY = endHeight - y1;
	pAti = (2 * deltaX) - deltaY;
	for (yc = y1; yc <= endHeight; yc++){ //Loop through Bresenham's algorithm for the case where slope > 1
		for (int i = (xc + 1); i < (width - xc - 1); i++){
			pgmPtr->image[yc][i] = color - heightStep;
			pgmPtr->image[endHeight - (yc - startHeight) - 1][i] = color - heightStep; // Map the left and right sides of the square
		}
		for (int i = yc; i < endHeight - (yc - startHeight); i++){
			pgmPtr->image[i][xc] = color - heightStep;
			pgmPtr->image[i][endWidth - (xc - startWidth)] = color - heightStep; // Map the top and bottom sides of the square
		}

		heightStep += heightOffset;
		if(pAti<0)
			pAti += 2 * deltaX;
		else{
 			xc++; //Increment x to head to the right of the screen
 			pAti += 2 * (deltaX - deltaY);
		}
	}
	return 0;
}
// Function for creating the grayscale image
int program2(int width, int height, int formatCode, char outputName[], int maxgray){
	//Determine if the format is raw or not
	int raw;
	if (formatCode == 1)
		raw = 1;
	else
		raw = 0;

	struct PGM_Image pgmIm;
	struct PGM_Image *pgmPtr;
	pgmPtr = &pgmIm;

	//Library call to initialize the PBM_Image struct
	create_PGM_Image(pgmPtr, width, height, maxgray);

	//Set the whole image to black
	for (int row = 0; row < height; row++){
		for (int col = 0; col < width; col++){
			pgmIm.image[row][col] = 0;
		}
	}

	//Calculate the area that should be white
	int hQuarter = height / 4;
	int wQuarter = width / 4;

	int startHeight = hQuarter;
	int endHeight = height - hQuarter - 1;

	int startWidth = wQuarter;
	int endWidth = width - wQuarter - 1;

	//Set the area that should be white, to white
	for (int i = startHeight; i <= endHeight; i++){
		for (int j = startWidth; j <= endWidth; j++){
			pgmIm.image[i][j] = 254;
			
		}
	}

	float slope = ((float)(endHeight + 1 - startHeight)) / ((float)(endWidth + 1 - startWidth));

	if (slope >= 0 && slope <= 1){
		prog2SlopeBetween01(height, width, pgmPtr);
	}
	// Algorithm for Octants 1 and 5
	if (slope > 1){
		prog2SlopeGreater1(height, width, pgmPtr);
	}
	//Save the image and free memory
	save_PGM_Image(pgmPtr, outputName, raw);
	free_PGM_Image(pgmPtr);
	return 0;
}

// Function for creating the Colour Image and 3 Grayscale images
int program3(int width, int height, int formatCode, char outputName[], int maxgray){
	//Determine if the format is raw or not
	int raw;
	if (formatCode == 1)
		raw = 1;
	else
		raw = 0;

	struct PPM_Image ppmIm;
	struct PPM_Image *ppmPtr;
	ppmPtr = &ppmIm;

	//Library call to initialize the PPM_Image struct
	create_PPM_Image(ppmPtr, width, height, maxgray);

	int halfHeight = height/2; //To define the dimensions of the blocks
	int halfWidth = width/2;

	int thirdWidth = width/3; //To define the edges of the three coloured blocks

	float offset = 255.0/halfHeight;  //To calculate the amount to change the colour by to appear smooth
	int colour = 255;
	float step = 0.0;

	// Code for the red block
	// We need to add the step to the green and blue components so each step gets closer to white
	for (int i = 0; i < halfHeight; i++){
		for (int j = 0; j < thirdWidth; j++){
			ppmIm.image[i][j][0] = colour;   // red
			ppmIm.image[i][j][1] = 0 + step; // green
			ppmIm.image[i][j][2] = 0 + step; // blue
		}
		step += offset; // Calculate the next colour offset
	}

	// Code for the green block
	// We need to add the step to the red and blue components so each step gets closer to white
	step = 0.0;
	for (int i = 0; i < halfHeight; i++){
		for (int j = thirdWidth; j < width - thirdWidth; j++){
			ppmIm.image[i][j][0] = colour - step; // red
			ppmIm.image[i][j][1] = colour;        // green 
			ppmIm.image[i][j][2] = colour - step; // blue			
		}
		step += offset; // Calculate the next colour offset
	}

	// Code for the blue block
	// We need to add the step to the red and green components so each step gets closer to white
	step = 0.0;
	for (int i = 0; i < halfHeight; i++){
		for (int j = width - thirdWidth; j < width; j++){
			ppmIm.image[i][j][0] = 0 + step;  // red
			ppmIm.image[i][j][1] = 0 + step;  // green 
			ppmIm.image[i][j][2] = colour;	  // blue		
		}
		step += offset; // Calculate the next colour offset
	}

	// Code for the left gray block
	// We need to add the step to all three components to produce grey and shift to white
	step = 0.0;
	colour = 0;
	for (int i = halfHeight; i < height; i++){
		for (int j = 0; j < halfWidth; j++){
			for (int k = 0; k < 3; k++){				
				ppmIm.image[i][j][k] = colour + step;
			}
		}
		step += offset;
	}

	// Code for the right gray block
	// We need to add the step to all three components to produce gray and shift to white
	step = 0.0;
	colour = 254;
	for (int i = halfHeight; i < height; i++){
		for (int j = halfWidth; j < width; j++){
			for (int k = 0; k < 3; k++){			
				ppmIm.image[i][j][k] = colour - step;
			}
		}
		step += offset;
	}

	//Save the colour image
	save_PPM_Image(ppmPtr, outputName, raw);
	

	struct PGM_Image pgmIm;
	struct PGM_Image *pgmPtr;
	pgmPtr = &pgmIm;

	//Library call to initialize the PGM_Image struct
	create_PGM_Image(pgmPtr, width, height, maxgray);

	// Create the three grayscale images using each colour as the maximum white
	copy_PPM_to_PGM(ppmPtr, pgmPtr, RED);
	save_PGM_Image(pgmPtr, "RedGrayScale.pgm", raw);

	copy_PPM_to_PGM(ppmPtr, pgmPtr, GREEN);
	save_PGM_Image(pgmPtr, "GreenGrayScale.pgm", raw);

	copy_PPM_to_PGM(ppmPtr, pgmPtr, BLUE);
	save_PGM_Image(pgmPtr, "BlueGrayScale.pgm", raw);
	
	// Free the memory
	free_PGM_Image(pgmPtr);
	free_PPM_Image(ppmPtr);
	return 0;
}

/*
main takes 5 arguments
image type code: 1 for pbm, 2 for pgm, 3 for ppm
image width: x >= 4 and divisible by 4 for pbm, pgm
             x >= 6 and divisible by 6 for ppm
image height: x >= 4 and divisble by 4
output image name
image format code: 0 for ASCII, 1 for raw
*/
int main(int argc, char *argv[]){

	// Validate the number of arguments, there should be 5 besides the program name
	if (argc != 6){
		printf("Error: Number of arguments must be 5\n");
		return 0;

	}
	int imageTypeCode = atoi(argv[1]);
	int imageWidth = atoi(argv[2]);
	int imageHeight = atoi(argv[3]);
	int imageFormat = atoi(argv[5]);

	// Validate the Image Type Code, should be 1, 2 of 3
	if (imageTypeCode > 3 || imageTypeCode < 1){
		printf("Error: First argument can only be 1, 2, or 3\n");
		return 0;
	}

	// Validate the the image width
	// if the image type is ppm (code 3) then width has to be >= 6 and divisible by 6
	if (imageTypeCode == 3){
		if (imageWidth < 6 || (imageWidth % 6 != 0)){
			printf("Error: ppm images must have width greater than 6 and divisble by 6\n");
			return 0;
		}
	}

	// if the image type is pbm or pgm then width has to be >= 4 and divisible by 4
	else{
		if (imageWidth < 4 || (imageWidth % 4 != 0)){
			printf("Error: Width must be greater than 4 and divisible by 4\n");
			return 0;
		}
	}

	// Validate the image height is greater than 4 and divisible by 4
	if (imageHeight < 4 || (imageHeight % 4 != 0)){
		printf("Error: Height must be greater than 4 and divisible by 4\n");
		return 0;
	}

	// Validate the Image Format Code, should be 0 or 1
	if (imageFormat < 0 || imageFormat > 1){
		printf("Error: Fifth argument can only be 0 or 1, was %d\n", imageFormat);
		return 0;
	}

	// If image type is 1, we go to program 1 and build the PBM image
	if (imageTypeCode == 1){
		program1(imageWidth, imageHeight, imageTypeCode, argv[4]);
	}

	// If image type is 2, we go to program 2 and build the PGM image
	if (imageTypeCode == 2){
		program2(imageWidth, imageHeight, imageTypeCode, argv[4], 255);
	}

	if (imageTypeCode == 3){
		program3(imageWidth, imageHeight, imageTypeCode, argv[4], 255);
	}
}