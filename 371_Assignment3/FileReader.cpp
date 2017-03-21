#include <vector>
#include <string>
#include <fstream>

#include "FileReader.h"

std::vector<std::string> FileReader::fileLines = std::vector<std::string>();

std::vector<std::string> FileReader::readFile() {

	std::cout << "Reading file..." << std::endl;

	std::string input_file_path = "scene.txt";
	std::vector<std::string> input_file_lines;
	std::ifstream input_file_stream(input_file_path, std::ios::in);

	if (input_file_stream.is_open()) {
		std::string line = "";
		while (getline(input_file_stream, line))
			input_file_lines.push_back(line);
		input_file_stream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", input_file_path.c_str());
		getchar();
		exit(-1);
	}

	fileLines = input_file_lines;

	std::cout << "Finished reading file." << std::endl;

	return input_file_lines;

}

Scene * FileReader::buildScene() {

	std::cout << "Building scene from file data..." << std::endl;

	Scene* scene = new Scene();

	int currLine = 0;

	int numObjects = stoi(fileLines[currLine]);
	std::cout << numObjects << std::endl;

	// Generate each object
	for (int i = 0; i < numObjects; ++i) {

		currLine++;

		// Generate camera
		if (fileLines[currLine] == "camera") {

			std::cout << "camera" << std::endl;

			Camera* camera = new Camera();

			// Load camera position
			currLine++;
			float x, y, z;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			camera->pos.x = x;
			camera->pos.y = y;
			camera->pos.z = z;
			std::cout << camera->pos.x << " " << camera->pos.y << " " << camera->pos.z << std::endl;


			// Load camera field of view
			currLine++;
			float fov;
			sscanf(fileLines[currLine].c_str(), "%*s %f", &fov);
			camera->fov = fov;
			std::cout << camera->fov << std::endl;

			// Load camera focal length
			currLine++;
			float fl;
			sscanf(fileLines[currLine].c_str(), "%*s %f", &fl);
			camera->fl = fl;
			std::cout << camera->fl << std::endl;

			// Load camera aspect ratio
			currLine++;
			float ar;
			sscanf(fileLines[currLine].c_str(), "%*s %f", &ar);
			camera->ar = ar;
			std::cout << camera->ar << std::endl;

			// Add camera to scene
			scene->camera = camera;
		}

		// Generate plane
		else if (fileLines[currLine] == "plane") {

			std::cout << "plane" << std::endl;

			Plane * plane = new Plane();

			// Load plane normal
			currLine++;
			float x, y, z;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			plane->normal.x = x;
			plane->normal.y = y;
			plane->normal.z = z;
			std::cout << plane->normal.x << " " << plane->normal.y << " " << plane->normal.z << std::endl;

			// Load plane position
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			plane->point.x = x;
			plane->point.y = y;
			plane->point.z = z;
			std::cout << plane->point.x << " " << plane->point.y << " " << plane->point.z << std::endl;

			// Load plane ambient color
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			plane->ambColor.x = x;
			plane->ambColor.y = y;
			plane->ambColor.z = z;
			std::cout << plane->ambColor.x << " " << plane->ambColor.y << " " << plane->ambColor.z << std::endl;

			// Load plane diffuse color
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			plane->difColor.x = x;
			plane->difColor.y = y;
			plane->difColor.z = z;
			std::cout << plane->difColor.x << " " << plane->difColor.y << " " << plane->difColor.z << std::endl;

			// Load plane specular color
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			plane->speColor.x = x;
			plane->speColor.y = y;
			plane->speColor.z = z;
			std::cout << plane->speColor.x << " " << plane->speColor.y << " " << plane->speColor.z << std::endl;

			// Load plane shininess
			currLine++;
			float shiny;
			sscanf(fileLines[currLine].c_str(), "%*s %f", &shiny);
			plane->shiny = shiny;
			std::cout << plane->shiny << std::endl;

			// Add plane to scene
			scene->plane = plane;
		}

		// Generate triangle
		else if (fileLines[currLine] == "triangle") {

			std::cout << "triangle" << std::endl;

			Triangle * triangle = new Triangle();

			// Load triangle vertex 1
			currLine++;
			float x, y, z;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			triangle->v1.x = x;
			triangle->v1.y = y;
			triangle->v1.z = z;
			std::cout << triangle->v1.x << " " << triangle->v1.y << " " << triangle->v1.z << std::endl;

			// Load triangle vertex 2
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			triangle->v2.x = x;
			triangle->v2.y = y;
			triangle->v2.z = z;
			std::cout << triangle->v2.x << " " << triangle->v2.y << " " << triangle->v2.z << std::endl;

			// Load triangle vertex 3
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			triangle->v3.x = x;
			triangle->v3.y = y;
			triangle->v3.z = z;
			std::cout << triangle->v3.x << " " << triangle->v3.y << " " << triangle->v3.z << std::endl;

			// Load triangle ambient color
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			triangle->ambColor.x = x;
			triangle->ambColor.y = y;
			triangle->ambColor.z = z;
			std::cout << triangle->ambColor.x << " " << triangle->ambColor.y << " " << triangle->ambColor.z << std::endl;

			// Load triangle diffuse color
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			triangle->difColor.x = x;
			triangle->difColor.y = y;
			triangle->difColor.z = z;
			std::cout << triangle->difColor.x << " " << triangle->difColor.y << " " << triangle->difColor.z << std::endl;

			// Load triangle specular color
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			triangle->speColor.x = x;
			triangle->speColor.y = y;
			triangle->speColor.z = z;
			std::cout << triangle->speColor.x << " " << triangle->speColor.y << " " << triangle->speColor.z << std::endl;

			// Load triangle shininess
			currLine++;
			float shiny;
			sscanf(fileLines[currLine].c_str(), "%*s %f", &shiny);
			triangle->shiny = shiny;
			std::cout << triangle->shiny << std::endl;

			// Add triangle to scene
			scene->triangles.push_back(triangle);

		}

		// Generate sphere
		else if (fileLines[currLine] == "sphere") {

			std::cout << "sphere" << std::endl;

			Sphere * sphere = new Sphere();

			// Load sphere position
			currLine++;
			float x, y, z;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			sphere->pos.x = x;
			sphere->pos.y = y;
			sphere->pos.z = z;
			std::cout << sphere->pos.x << " " << sphere->pos.y << " " << sphere->pos.z << std::endl;

			// Load sphere radius
			currLine++;
			float r;
			sscanf(fileLines[currLine].c_str(), "%*s %f", &r);
			sphere->r = r;
			std::cout << sphere->r << std::endl;

			// Load sphere ambient color
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			sphere->ambColor.x = x;
			sphere->ambColor.y = y;
			sphere->ambColor.z = z;
			std::cout << sphere->ambColor.x << " " << sphere->ambColor.y << " " << sphere->ambColor.z << std::endl;

			// Load sphere diffuse color
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			sphere->difColor.x = x;
			sphere->difColor.y = y;
			sphere->difColor.z = z;
			std::cout << sphere->difColor.x << " " << sphere->difColor.y << " " << sphere->difColor.z << std::endl;

			// Load sphere specular color
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			sphere->speColor.x = x;
			sphere->speColor.y = y;
			sphere->speColor.z = z;
			std::cout << sphere->speColor.x << " " << sphere->speColor.y << " " << sphere->speColor.z << std::endl;

			// Load sphere shininess
			currLine++;
			float shiny;
			sscanf(fileLines[currLine].c_str(), "%*s %f", &shiny);
			sphere->shiny = shiny;
			std::cout << sphere->shiny << std::endl;

			// Add sphere to scene
			scene->spheres.push_back(sphere);


		}

		// Generate light
		else if (fileLines[currLine] == "light") {

			std::cout << "light" << std::endl;

			Light * light = new Light();

			// Load sphere position
			currLine++;
			float x, y, z;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			light->pos.x = x;
			light->pos.y = y;
			light->pos.z = z;
			std::cout << light->pos.x << " " << light->pos.y << " " << light->pos.z << std::endl;

			// Load sphere color
			currLine++;
			sscanf(fileLines[currLine].c_str(), "%*s %f %f %f", &x, &y, &z);
			light->color.x = x;
			light->color.y = y;
			light->color.z = z;
			std::cout << light->color.x << " " << light->color.y << " " << light->color.z << std::endl;

			// Add light to scene
			scene->lights.push_back(light);

		}
	}

	std::cout << "Finished building scene." << std::endl;

	return scene;

}
