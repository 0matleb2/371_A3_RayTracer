#pragma once
#include "glm.hpp"

class Ray {

public:

	glm::vec3 orig;
	glm::vec3 dir;

	Ray(glm::vec3 orig, glm::vec3 dir) {
		this->orig = orig;
		this->dir = dir;
	}

};