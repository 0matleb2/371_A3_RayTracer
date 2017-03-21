#pragma once
#include "glm.hpp"

class Triangle {

public:

	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;

	glm::vec3 ambColor;
	glm::vec3 difColor;
	glm::vec3 speColor;
	float shiny;


};