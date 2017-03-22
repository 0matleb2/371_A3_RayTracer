#include <iostream>

#include "RayTracer.h"

#define PI 3.14159265
#define EPSILON 0.000001


std::vector<Ray*> RayTracer::rays = std::vector<Ray*>();


std::vector<Ray*> RayTracer::generateRays(Camera* camera, int resScale) {

	std::cout << "Computing pixel positions..." << std::endl;

	std::vector<Ray*> tempRays;
	std::vector<glm::vec3> pixels;


	int rWidth = static_cast<int>(round(camera->ar * resScale));
	int rHeight = resScale;

	std::cout << "Resolution: " << rWidth << " x " << rHeight << std::endl;

	float sHeight = 2 * camera->fl * tan((camera->fov / 2) * PI / 180);
	float sWidth = camera->ar * sHeight;

	std::cout << "Scene width: " << sWidth << std::endl;
	std::cout << "Scene height: " << sHeight << std::endl;

	float pixelWidth = sWidth / rWidth;
	float pixelHeight = sHeight / rHeight;

	std::cout << "Pixel width: " << pixelWidth << std::endl;
	std::cout << "Pixel height: " << pixelHeight << std::endl;

	glm::vec3 iCenter = glm::vec3(camera->pos.x, camera->pos.y, camera->pos.z - camera->fl);


	// Calculate pixel locations
	for (int y = 0; y < rHeight; ++y) {

		for (int x = 0; x < rWidth; ++x) {

			glm::vec3 pixel = {
				(iCenter.x - sWidth / 2) + (pixelWidth / 2) + (x * pixelWidth),
				(iCenter.y + sHeight / 2) - (pixelHeight / 2) - (y * pixelHeight),
				iCenter.z
			};

			//std::cout << "Pixel(" << x << ", " << y << "): " << pixel.x << " " << pixel.y << " " << pixel.z << std::endl;

			pixels.push_back(pixel);
		}
	}

	// Generate rays
	for (auto pixel : pixels) {
		glm::vec3 direction = glm::normalize(pixel - camera->pos);
		tempRays.push_back(new Ray(camera->pos, direction));
	}

	rays = tempRays;

	return rays;
}

bool RayTracer::intersectRaySphere(Ray * ray, Sphere * sphere, float &t, glm::vec3 & norm) {
	
	float t0, t1; // Intersection solutions
	
	glm::vec3 l = ray->orig - sphere->center;
	float a = glm::dot(ray->dir, ray->dir);
	float b = 2 * glm::dot(ray->dir, l);
	float c = glm::dot(l, l) - pow(sphere->r, 2);

	if (!solveQuadratic(a, b, c, t0, t1)) return false;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) return false; // both t0 and t1 are negative 
	}

	t = t0;

	// Calculate normal at intersection point
	norm = glm::normalize((ray->orig + t*ray->dir) - sphere->center);

	return true;
}







bool RayTracer::intersectRayPlane(Ray * ray, Plane * plane, float & t, glm::vec3 & norm) {

	float denom = glm::dot(plane->normal, ray->dir);

	if (abs(denom) > 1e-6) {

		t = glm::dot(plane->point - ray->orig, plane->normal) / denom;

		// Calculate normal
		norm = plane->normal;

		return (t >= 0);
	}

	return false;
}







bool RayTracer::intersectRayTriangle(Ray * ray, Triangle * triangle, float & t, glm::vec3 & norm) {
	glm::vec3 e1, e2;  //Edge1, Edge2
	glm::vec3 P, Q, T;
	float det, inv_det, u, v;
	float out;

	//Find vectors for two edges sharing V1
	e1 = triangle->v2 - triangle->v1;
	e2 = triangle->v3 - triangle->v1;

	//Begin calculating determinant - also used to calculate u parameter
	P = glm::cross(ray->dir, e2);
	det = glm::dot(e1, P);
	
	//if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
	//NOT CULLING
	if (det > -EPSILON && det < EPSILON) return false;
	inv_det = 1.f / det;

	//calculate distance from V1 to ray origin
	T = ray->orig - triangle->v1;

	//Calculate u parameter and test bound
	u = glm::dot(T, P) * inv_det;
	//The intersection lies outside of the triangle
	if (u < 0.f || u > 1.f) return false;

	//Prepare to test v parameter
	Q = glm::cross(T, e1);

	//Calculate V parameter and test bound
	v = glm::dot(ray->dir, Q) * inv_det;
	//The intersection lies outside of the triangle
	if (v < 0.f || u + v  > 1.f) return false;

	out = glm::dot(e2, Q) * inv_det;

	if (out > EPSILON) { //ray intersection
		t = out;

		// Calculate normal
		norm = glm::normalize(glm::cross(e2, e1));

		return true;
	}

	// No hit, no win
	return false;
}








glm::vec3 RayTracer::trace(Scene * scene, Ray * ray, int depth) {
	if (depth > DMAX) return scene->backgroundColor;
	float t;
	float tmin = std::numeric_limits<float>::max();
	glm::vec3 n, norm;
	SceneGeometry * obj;

	// Intersect each sphere
	for (int i = 0, m = scene->spheres.size(); i < m; ++i) {
		if (RayTracer::intersectRaySphere(ray, scene->spheres[i], t, n)) {
			if (t < tmin) {
				tmin = t;
				norm = n;
				obj = scene->spheres[i];
			}
		}
	}

	// Intersect each triangle
	for (int i = 0, m = scene->triangles.size(); i < m; ++i) {
		if (RayTracer::intersectRayTriangle(ray, scene->triangles[i], t, n)) {
			if (t < tmin) {
				tmin = t;
				norm = n;
				obj = scene->triangles[i];
			}
		}
	}
	
	// Intersect the plane
	if (RayTracer::intersectRayPlane(ray, scene->plane, t, n)) {
		if (t < tmin) {
			tmin = t;
			norm = n;
			obj = scene->plane;
		}
	}

	// Return background color if no intersections
	if (abs(tmin - std::numeric_limits<float>::max()) < EPSILON) {
		return scene->backgroundColor;
	}

	glm::vec3 intersection = ray->orig + tmin*ray->dir;
	glm::vec3 color = accLight(scene, intersection, obj);


	return color;
}







bool RayTracer::solveQuadratic(const float & a, const float & b, const float & c, float & x0, float & x1) {
	float discr = b * b - 4 * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5 * b / a;
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discr)) :
			-0.5 * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}







glm::vec3 RayTracer::accLight(Scene* scene, glm::vec3 intersection, SceneGeometry * obj) {
	glm::vec3 color = obj->ambColor;

	//for (auto light : scene->lights) {

	//	float t;
	//	float tmin = glm::distance(light->pos, intersection);
	//	glm::vec3 n;
	//	bool shadowed = false;

	//	Ray* shadowRay = new Ray(intersection, glm::normalize(light->pos - intersection));

	//	// Intersect each sphere
	//	for (int i = 0, m = scene->spheres.size(); i < m; ++i) {
	//		if (RayTracer::intersectRaySphere(shadowRay, scene->spheres[i], t, n)) {
	//			if (t < tmin) {
	//				shadowed = true;
	//				break;
	//			}
	//		}
	//	}

	//	if (!shadowed) {
	//		// Intersect each triangle
	//		for (int i = 0, m = scene->triangles.size(); i < m; ++i) {
	//			if (RayTracer::intersectRayTriangle(shadowRay, scene->triangles[i], t, n)) {
	//				if (t < tmin) {
	//					shadowed = true;
	//				}
	//			}
	//		}
	//	}

	//	if (!shadowed) {
	//		// Intersect the plane
	//		if (RayTracer::intersectRayPlane(shadowRay, scene->plane, t, n)) {
	//			if (t < tmin) {
	//				shadowed = true;
	//			}
	//		}
	//	}

	//	if (!shadowed) {
	//		color += light->color; //LEFT OFF HEREEEE PHONG ETC
	//	}

	//}

	return color;

}
