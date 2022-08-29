#include <iostream>
#include <algorithm>
#include <map>
#include <conio.h>
#include <thread>
#include <cmath>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "crender.h"
#include "media.h"

int getBrightness (cv::Mat *img, int x, int y)
{
	return img->data[y * img->cols + x];
}

void generateAscii (cv::Mat *img, int scaleW, int scaleH, 
                    char *ascii, int width, int height)
{
	int asciiHeight = std::ceil((float)img->rows / (float)scaleH);
	int asciiWidth = std::ceil((float)img->cols / (float)scaleW);

	int yStart = (height - asciiHeight) / 2;
	int yEnd = yStart + asciiHeight;
	int xStart = (width - asciiWidth) / 2;
	int xEnd = xStart + asciiWidth;

	for (int y = yStart; y < yEnd; y++)
		for (int x = xStart; x < xEnd; x++) {
			int brightness = 0;
			for (int i = 0; i < scaleH; i++)
				for (int j = 0; j < scaleW; j++)
					brightness += getBrightness(img, 
					                            (x-xStart)*scaleW+j, 
					                            (y-yStart)*scaleH+i);
			brightness /= scaleH * scaleW;

			brightness = ((float)brightness / 256.0) * 4;
			switch (brightness) {
				case 0:
					ascii[y * width + x] = ' ';
					break;
				case 1:
					ascii[y * width + x] = 176;
					break;
				case 2:
					ascii[y * width + x] = 177;
					break;
				case 3:
				case 4:
					ascii[y * width + x] = 178;
					break;
			}
		}
}

int main () 
{
	ConsoleRender crender;

	MediaObject *photo = MediaLoader::getInstance()->load("TEST.jpg");
	std::thread photoShowcase([](MediaObject *img) {img->show();}, photo);
	photoShowcase.detach();

	int fontH;
	int fontW;
	crender.getFontSize(&fontW, &fontH);

	int screenW = crender.getWidth();
	int screenH = crender.getHeight();

	int scaleH;
	int scaleW;
	if (screenH < screenW) {
		scaleH = std::ceil((float)photo->getData()->rows / screenH);
		scaleW = std::ceil((float)scaleH * (float)fontW / (float)fontH);
	} else {
		scaleW = std::ceil((float)photo->getData()->cols / screenW);
		scaleH = std::ceil((float)scaleW * (float)fontH / (float)fontW);
	}

	char *screenBuffer = new char[crender.getWidth() * crender.getHeight()];
	memset(screenBuffer, ' ', crender.getWidth() * crender.getHeight());
	generateAscii(photo->getData(), scaleW, scaleH, 
	              screenBuffer, crender.getWidth(), crender.getHeight());
	
	bool isDone = false;

	int iter = 0;

	while (!isDone) {
		crender.clear();

		crender.drawBuffer(screenBuffer, 
		                   crender.getWidth(), 
		                   crender.getHeight(),
		                   0, 0);

		crender.show();

		getch();
	}

	// int pos = 0;
	// int counter = 0;

	// int width, height, channelsAmount;
	// RGB Unsigned Byte
	// unsigned char *data = stbi_load("pic2.bmp", &width, &height, 
	//                                 &channelsAmount, 0);

	// try {
	// 	MediaObject *pic1 = MediaLoader::getInstance()->load("cat_2.png");
	// 	pic1->show();
	// } 
	// catch (Error err) {
	// 	std::cout << "ERROR::";
	// 	switch (err) {
	// 		case INCORRECT_FILENAME:
	// 			std::cout << "incorrect filename\n";
	// 			break;
	// 		case INCORRECT_FORMAT:
	// 			std::cout << "incorrect format\n";
	// 			break;
	// 		case LOADING_ERROR:
	// 			std::cout << "unable to load an image\n";
	// 			break;
	// 	}
	// }

	// for (int i = 0; i < height; i++)
	// 	for (int j = 0; j < width; j++)
	// 		std::cout << "pixel " << j << 'x' << i << ": "
	// 		          << (int)data[((i*width)+j)*3+0] << ' '
	// 		          << (int)data[((i*width)+j)*3+1] << ' '
	// 		          << (int)data[((i*width)+j)*3+2] << '\n';
	// getchar();

	// stbi_image_free(data);
	delete screenBuffer;
	return 0;
}