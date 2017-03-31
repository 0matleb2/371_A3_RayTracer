#include <random>
#include <chrono>
#include <thread>

#include "FileReader.h"
#include "Scene.h"
#include "RayTracer.h"
#include "CImg.h"
#include "Options.h"


bool display = true;

void renderScene(Options * options);


// _,.-~'"` MAIN `"'~-.,_
int main() {


	Options * options = new Options();
	options->chooseOptions();

	if (options->sceneNum == 0) {
		display = false;
		for (int i = 1; i <= 17; ++i) {
			options->sceneNum++;
			renderScene(options);
		}
	}
	else {
		renderScene(options);
	}
}




void renderScene(Options * options) {

	// Read file and build scene
	FileReader::readFile(options->sceneNum);
	Scene * scene = FileReader::buildScene();

	// Render
	Image * image = RayTracer::render(scene, options);

	// Transfer rendered pixel color data into CImg object
	cimg_library::CImg<float> cImage(image->resW, image->resH, 1, 3, 0);

	int bitmappedPixels = 0;
	int pixelsToBitmap = image->resH * image->resW;
	int lastPercentComplete = 0;

	printf("Preparing bitmap... %i%%\r", int(lastPercentComplete));

	for (int y = 0; y < image->resH; y++) {

		for (int x = 0; x < image->resW; x++) {

			int index = x + (y * image->resW);
			float red = image->pixels[index]->color.r * 255;
			float green = image->pixels[index]->color.g * 255;
			float blue = image->pixels[index]->color.b * 255;

			cImage(x, y, 0, 0) = red;
			cImage(x, y, 0, 1) = green;
			cImage(x, y, 0, 2) = blue;

			bitmappedPixels++;
			int percentComplete = round((float(bitmappedPixels) / pixelsToBitmap) * 100);

			if (percentComplete != lastPercentComplete) {
				printf("Preparing bitmap... %i%%\r", int(percentComplete));
				fflush(stdout);
				lastPercentComplete = percentComplete;
			}

		}
	}
	printf("\n\n");
	std::cout << "Rendering of scene " + std::to_string(options->sceneNum) + " complete!" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	// Save image
	std::string outputPath = "renders\\scene" + std::to_string(options->sceneNum) + ".bmp";
	cImage.save(outputPath.c_str());
	std::cout << "Image saved to \"" << outputPath << "\"" << std::endl << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	// Deallocate memory
	for (auto pixel : image->pixels) {
		delete pixel;
	}
	delete image;

	if (display) {
		// Display rendered image
		cimg_library::CImgDisplay main_disp(cImage, "Mathieu's Ray Tracer");
		while (!main_disp.is_closed()) {
			main_disp.wait();
		}
	}
}