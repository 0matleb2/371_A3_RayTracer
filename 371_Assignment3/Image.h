#pragma once
#include <vector>

#include "Pixel.h"


class Image {

public:
	int resW;
	int resH;
	std::vector<Pixel*> pixels;

	Image(int resWi, int resHi, std::vector<Pixel*> pixelsi) : resW(resWi), resH(resHi) {
		pixels = pixelsi;
	}
};