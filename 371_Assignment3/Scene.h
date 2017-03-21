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
	Plane* plane;

	std::vector<Sphere*> spheres;
	std::vector<Triangle*> triangles;
	std::vector<Light*> lights;



};