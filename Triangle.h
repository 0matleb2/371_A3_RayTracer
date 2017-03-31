#pragma once
#include "glm.hpp"

#include "SceneGeometry.h"

class Triangle : public SceneGeometry {

public:

	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;

};