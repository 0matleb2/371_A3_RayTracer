#pragma once

#include <vector>

#include "Scene.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "Camera.h"

#define DMAX 5

class RayTracer {
public:

	static std::vector<Ray*> rays;

	static std::vector<Ray*> generateRays(Camera* camera, int resScale);

	static bool intersectRaySphere(Ray* ray, Sphere* sphere, float & t);

	static bool intersectRayPlane(Ray* ray, Plane* sphere, float & t);

	static bool intersectRayTriangle(Ray* ray, Triangle* sphere, float & t);

	static glm::vec3 trace(Scene * scene, Ray * ray, int depth);

private:

	static bool solveQuadratic(const float & a, const float & b, const float & c, float & x0, float & x1);
};


