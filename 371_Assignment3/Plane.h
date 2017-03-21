#pragma once
#include "glm.hpp"


class Plane {

public:

	glm::vec3 normal;
	glm::vec3 point;

	glm::vec3 ambColor;
	glm::vec3 difColor;
	glm::vec3 speColor;
	float shiny;

};