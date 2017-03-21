#pragma once

#include <iostream>
#include <string>
#include <stdio.h>

#include "Scene.h"

class FileReader {

public:
	
	static std::vector<std::string> fileLines;

	static std::vector<std::string> readFile();

	static Scene* buildScene();


};