#include "FileReader.h"
#include "Scene.h"
#include "RayTracer.h"
#include "main.h"


int main() {

	FileReader::readFile();
	Scene * scene = FileReader::buildScene();

	std::vector<Ray*> rays = RayTracer::generateRays(scene->camera, 1);
	std::vector<glm::vec3> pixels;

	for (auto ray : rays) {
		pixels.push_back(RayTracer::trace(scene, ray, 0));
	}




	char c;
	std::cin >> c;
}