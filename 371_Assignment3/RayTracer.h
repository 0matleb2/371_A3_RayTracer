#pragma once

#include <vector>
#include <limits>

#include "Scene.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "Camera.h"
#include "Options.h"
#include "Pixel.h"
#include "Image.h"

#define DMAX 5
#define EPSILON 0.000001

class RayTracer {
public:

	static std::vector<Ray*> rays;

	static Image* render(Scene * scene, Options * options);

	static Image* generatePixels(Scene * scene, Options * options);

	static std::vector<Ray*> generateRays(Scene * scene, Options * options, Pixel * pixel);

	static bool intersectRaySphere(Ray* ray, Sphere* sphere, float & t, glm::vec3 & norm);

	static bool intersectRayPlane(Ray* ray, Plane* sphere, float & t, glm::vec3 & norm);

	static bool intersectRayTriangle(Ray* ray, Triangle* sphere, float & t, glm::vec3 & norm);

	static glm::vec3 trace(Scene * scene, Ray * ray, int depth);

private:

	static bool solveQuadratic(const float & a, const float & b, const float & c, float & x0, float & x1);

	static glm::vec3 accLight(Scene * scene, glm::vec3 intersection, SceneGeometry * obj, glm::vec3 norm, glm::vec3 v);

	static glm::vec3 average(std::vector<glm::vec3> traces);

	static Image * downsample(Image * image, int & downsampledPixels, int pixelsToDownsample, int & lastPercent);

	static glm::vec3 phong(Light * light, Ray * shadowRay, SceneGeometry * obj, glm::vec3 norm, glm::vec3 v);
};


