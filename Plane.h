#pragma once
#include <GLM\glm.hpp>

#include "SceneGeometry.h"


class Plane : public SceneGeometry {

public:

	glm::vec3 normal;
	glm::vec3 point;

};