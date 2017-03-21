#pragma once
#include "glm.hpp"


class Sphere {

public:

	glm::vec3 pos;
	float r;

	glm::vec3 ambColor;
	glm::vec3 difColor;
	glm::vec3 speColor;
	float shiny;
};