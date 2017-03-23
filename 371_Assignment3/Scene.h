#pragma once

#include <vector>

#include "Camera.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"



class Scene {

public:

	Camera* camera;

	std::vector<Sphere*> spheres;
	std::vector<Triangle*> triangles;
	std::vector<Plane*> planes;
	std::vector<Light*> lights;

	glm::vec3 backgroundColor;

	Scene() : backgroundColor(glm::vec3(0.01f, 0.01f, 0.02f)) {};

};