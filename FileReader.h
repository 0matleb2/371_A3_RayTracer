#pragma once

#include <iostream>
#include <string>
#include <stdio.h>

#include "Scene.h"

class FileReader {

public:
	
	static std::vector<std::string> fileLines;
	static std::string loadedFilePath;

	static std::vector<std::string> readFile(int sceneNum);

	static Scene* buildScene();


};