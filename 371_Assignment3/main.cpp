#include "FileReader.h"
#include "Scene.h"
#include "RayTracer.h"
#include "CImg.h"
#include "Options.h"



int main() {


	Options * options = new Options();
	options->chooseOptions();


	FileReader::readFile(options->sceneNum);
	Scene * scene = FileReader::buildScene();

	Image * image = RayTracer::render(scene, options);



	cimg_library::CImg<float> cImage(image->resW, image->resH, 1, 3, 0);

	for (int y = 0; y < image->resH; y++) {

		for (int x = 0; x < image->resW; x++) {

			int index = x + (y * image->resW);
			float red = image->pixels[index]->color.r * 255;
			float green = image->pixels[index]->color.g * 255;
			float blue = image->pixels[index]->color.b * 255;

			cImage(x, y, 0, 0) = red;
			cImage(x, y, 0, 1) = green;
			cImage(x, y, 0, 2) = blue;
		}
	}

	cImage.save("render.bmp");
	cimg_library::CImgDisplay main_disp(cImage, "Mathieu's Ray Tracing Render");
	while (!main_disp.is_closed()) {
		main_disp.wait();
	}

}