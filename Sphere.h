#pragma once
#include <GLM\glm.hpp>

#include "SceneGeometry.h"


class Sphere : public SceneGeometry {

public:

	glm::vec3 center;
	float r;

};