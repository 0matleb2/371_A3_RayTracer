#pragma once
#include "glm.hpp"


class Pixel {

public:

	glm::vec3 pos;
	glm::vec3 color;

	Pixel() : pos(glm::vec3(0.0f, 0.0f, 0.0f)), color(glm::vec3(0.0f, 0.0f, 0.0f)) {};

};