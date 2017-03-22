#include "FileReader.h"
#include "Scene.h"
#include "RayTracer.h"
#include "CImg.h"
#include "main.h"


int main() {
	int resScale = 300;

	FileReader::readFile();
	Scene * scene = FileReader::buildScene();

	std::vector<Ray*> rays = RayTracer::generateRays(scene->camera, resScale);
	std::vector<glm::vec3> pixels;

	for (auto ray : rays) {
		pixels.push_back(RayTracer::trace(scene, ray, 0));
	}

	int rWidth = static_cast<int>(round(scene->camera->ar * resScale));
	int rHeight = resScale;

	
	cimg_library::CImg<float> image(rWidth, rHeight, 1, 3, 0);
	
	for (int y = 0; y < rHeight; y++) {

		for (int x = 0; x < rWidth; x++) {

			int index = x + (y * rWidth);
			float red = pixels[index].r * 255;
			float green = pixels[index].g * 255;
			float blue = pixels[index].b * 255;

			image(x, y, 0, 0) = red;
			image(x, y, 0, 1) = green;
			image(x, y, 0, 2) = blue;
		}
	}

	image.save("render.bmp");
	cimg_library::CImgDisplay main_disp(image, "Render");
	while (!main_disp.is_closed()) {
		main_disp.wait();
	}

	char c;
	std::cin >> c;
}