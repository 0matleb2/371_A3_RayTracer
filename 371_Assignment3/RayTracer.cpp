#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <random>

#include "RayTracer.h"

#define PI 3.14159265
#define EPSILON 0.000001



std::vector<Ray*> RayTracer::rays = std::vector<Ray*>();

int sumNdiv4i(int n, int i) {
	int sum = 0;
	for (int j = 0; j < i; j++) {
		sum += n / int(pow(4, j));
	}
	return sum;
}


Image* RayTracer::render(Scene * scene, Options * options) {
	int pixelsRendered = 0;
	int lastPercentComplete = 0;

	Image * image = generatePixels(scene, options);

	for (auto pixel : image->pixels) {
		
		std::vector<Ray*> rays = generateRays(scene, options, pixel);

		std::vector<glm::vec3> traces;
		for (auto ray : rays) {
			traces.push_back(trace(scene, ray, 0));
		}

		pixel->color = average(traces);

		pixelsRendered++;
		int percentComplete = round((float(pixelsRendered) / image->pixels.size()) * 100);
		
		if (percentComplete != lastPercentComplete) {
			printf("Tracing... %i%%\r", int(percentComplete));
			fflush(stdout);
			lastPercentComplete = percentComplete;
		}

	}

	printf("\n");

	int downsampledPixels = 0;
	int lastPercent = 0;
	int n = image->resH * image->resW;
	int downsamplesTimes;
	if (options->antialiasing == 1 || options->antialiasing == 2) {
		downsamplesTimes = 1;
	}
	else if (options->antialiasing == 3) {
		downsamplesTimes = 2;
	}
	int pixelsToDown = sumNdiv4i(n, downsamplesTimes);

	for (int d = downsamplesTimes; d > 0; d--) {
		image = downsample(image, downsampledPixels, pixelsToDown, lastPercent);
	}
	printf("\n");

	return image;

}






Image* RayTracer::generatePixels(Scene * scene, Options * options) {
	int pixelsCalculated = 0;
	int lastPercentComplete = 0;

	std::vector<Pixel*> pixels;


	int renderedResH;
	if (options->antialiasing == 0) {
		renderedResH = options->outputHeight;
	}
	else if (options->antialiasing == 1 || options->antialiasing == 2) {
		renderedResH = options->outputHeight * 2;
	}
	else if (options->antialiasing == 3) {
		renderedResH = options->outputHeight * 4;
	}

	int renderedResW = static_cast<int>(round(renderedResH * scene->camera->ar));

	std::cout << "Rendering at resolution: " << renderedResW << " x " << renderedResH << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	float sHeight = 2 * scene->camera->fl * tan((scene->camera->fov / 2) * PI / 180);
	float sWidth = scene->camera->ar * sHeight;

	float pixelWidth = sWidth / renderedResW;
	float pixelHeight = sHeight / renderedResH;
	options->pixelW = pixelWidth;
	options->pixelH = pixelHeight;

	glm::vec3 iCenter = glm::vec3(scene->camera->pos.x, scene->camera->pos.y, scene->camera->pos.z - scene->camera->fl);

	// Calculate pixel locations
	for (int y = 0; y < renderedResH; ++y) {

		for (int x = 0; x < renderedResW; ++x) {

			glm::vec3 pixelPos = {
				(iCenter.x - sWidth / 2) + (pixelWidth / 2) + (x * pixelWidth),
				(iCenter.y + sHeight / 2) - (pixelHeight / 2) - (y * pixelHeight),
				iCenter.z
			};

			Pixel * pixel = new Pixel();
			pixel->pos = pixelPos;

			//std::cout << "Pixel(" << x << ", " << y << "): " << pixel.x << " " << pixel.y << " " << pixel.z << std::endl;

			pixels.push_back(pixel);

			pixelsCalculated++;
			int percentComplete = round((float(pixelsCalculated) / (renderedResW*renderedResH)) * 100);

			if (percentComplete != lastPercentComplete) {
				printf("Generating rays... %i%%\r", int(percentComplete));
				fflush(stdout);
				lastPercentComplete = percentComplete;
			}
		}
	}

	printf("\n");

	Image * image = new Image(renderedResW, renderedResH, pixels);

	return image;
}





std::vector<Ray*> RayTracer::generateRays(Scene * scene, Options * options, Pixel * pixel) {

	std::vector<Ray *> rays;

	// Generate rays
	if (options->antialiasing == 0) {
		glm::vec3 direction = glm::normalize(pixel->pos - scene->camera->pos);
		rays.push_back(new Ray(scene->camera->pos, direction));
	}
	else if (options->antialiasing == 1) {

		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<> dis(0, 1);

		int numDithRays = 4;
		for (int i = 0; i < numDithRays; ++i) {
			glm::vec3 dCenter = pixel->pos;
			dCenter.x -= options->pixelW / 2;
			dCenter.y -= options->pixelH / 2;
			dCenter.x += options->pixelW * dis(gen);
		    dCenter.y -= options->pixelH * dis(gen);

			glm::vec3 direction = glm::normalize(dCenter - scene->camera->pos);
			rays.push_back(new Ray(scene->camera->pos, direction));
		}
	}
	else if (options->antialiasing > 1) {
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<> dis(0, 1);

		int numDithRays = 16;
		for (int i = 0; i < numDithRays; ++i) {
			glm::vec3 dCenter = pixel->pos;
			dCenter.x -= options->pixelW / 2;
			dCenter.y -= options->pixelH / 2;
			dCenter.x += options->pixelW * dis(gen);
			dCenter.y -= options->pixelH * dis(gen);

			glm::vec3 direction = glm::normalize(dCenter - scene->camera->pos);
			rays.push_back(new Ray(scene->camera->pos, direction));
		}
	}

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
	for (int i = 0, m = scene->planes.size(); i < m; ++i) {
		if (RayTracer::intersectRayPlane(ray, scene->planes[i], t, n)) {
			if (t < tmin) {
				tmin = t;
				norm = n;
				obj = scene->planes[i];
			}
		}
	}

	// Return background color if no intersections
	if (abs(tmin - std::numeric_limits<float>::max()) < EPSILON) {
		return scene->backgroundColor;
	}

	glm::vec3 intersection = ray->orig + tmin*ray->dir;
	glm::vec3 color = accLight(scene, intersection, obj, norm, -ray->dir);


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







glm::vec3 RayTracer::accLight(Scene* scene, glm::vec3 intersection, SceneGeometry * obj, glm::vec3 norm, glm::vec3 v) {
	glm::vec3 color = obj->ambColor;

	for (auto light : scene->lights) {

		float t;
		float tmin = glm::distance(light->pos, intersection);
		glm::vec3 n;
		bool shadowed = false;

		glm::vec3 shadowDir = glm::normalize(light->pos - intersection);
		float o = 0.1;
		glm::vec3 origOffset = o * shadowDir;
		//glm::vec3 origOffset = glm::vec3(shadowDir.x * o, shadowDir.y * o, shadowDir.z * o);
		Ray* shadowRay = new Ray(intersection + origOffset, shadowDir);

		// Intersect each sphere
		for (int i = 0, m = scene->spheres.size(); i < m; ++i) {
			if (RayTracer::intersectRaySphere(shadowRay, scene->spheres[i], t, n)) {
				if (t < tmin) {
					shadowed = true;
					break;
				}
			}
		}

		if (!shadowed) {
			// Intersect each triangle
			for (int i = 0, m = scene->triangles.size(); i < m; ++i) {
				if (RayTracer::intersectRayTriangle(shadowRay, scene->triangles[i], t, n)) {
					if (t < tmin) {
						shadowed = true;
						break;
					}
				}
			}
		}

		if (!shadowed) {
			// Intersect the plane
			for (int i = 0, m = scene->planes.size(); i < m; ++i) {
				if (RayTracer::intersectRayPlane(shadowRay, scene->planes[i], t, n)) {
					if (t < tmin) {
						shadowed = true;
						break;
					}
				}
			}
		}

		if (!shadowed) {
			color += phong(light, shadowRay, obj, norm, v);
		}

	}

	glm::vec3 clamp = glm::vec3(glm::min(1.0f, color.r), glm::min(1.0f, color.g), glm::min(1.0f, color.b));

	return clamp;

}

glm::vec3 RayTracer::average(std::vector<glm::vec3> traces) {
	float r = 0.0f, g = 0.0f, b = 0.0f;
	for (auto trace : traces) {
		r += trace.r;
		g += trace.g;
		b += trace.b;
	}
	int s = traces.size();
	r /= s;
	g /= s;
	b /= s;
	return glm::vec3(r, g, b);
}

Image * RayTracer::downsample(Image * image, int & downsampledPixels, int pixelsToDownsample, int & lastPercent) {


	int dHeight = image->resH / 2;
	int dWidth = image->resW / 2;

	std::vector<Pixel*> dPixels;

	for (int y = 0; y < dHeight; ++y) {

		for (int x = 0; x < dWidth; ++x) {


			std::vector<glm::vec3> samples;

			samples.push_back(image->pixels[x * 2 + image->resW*y * 2]->color);
			samples.push_back(image->pixels[x * 2 + 1 + image->resW*y * 2]->color);
			samples.push_back(image->pixels[x * 2 + image->resW*(y * 2 + 1)]->color);
			samples.push_back(image->pixels[x * 2 + 1 + image->resW*(y * 2 + 1)]->color);

			Pixel * dPixel = new Pixel();
			dPixel->color = average(samples);
			dPixels.push_back(dPixel);

			downsampledPixels = downsampledPixels + 4;
			int percentComplete = round((float(downsampledPixels) / pixelsToDownsample) * 100);

			if (percentComplete != lastPercent) {
				printf("Downsampling... %i%%\r", int(percentComplete));
				fflush(stdout);
				lastPercent = percentComplete;
			}
		}
	}

	Image * dImage = new Image(dWidth, dHeight, dPixels);

	return dImage;
}

glm::vec3 RayTracer::phong(Light * light, Ray * shadowRay, SceneGeometry * obj, glm::vec3 norm, glm::vec3 v) {
	glm::vec3 l = shadowRay->dir;
	glm::vec3 r = 2 * glm::dot(l, norm) * norm - l;

	float ldotn = glm::dot(l, norm);
	float rdotv = glm::dot(r, v);

	glm::vec3 spec = obj->speColor * pow(glm::max(0.0f, rdotv), obj->shiny);
	glm::vec3 dif = obj->difColor * abs(ldotn);

	glm::vec3 nothing = glm::vec3(0.0f);
	glm::vec3 color = light->color * (dif + spec);

	return color;
}
