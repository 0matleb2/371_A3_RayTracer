#include "Options.h"

#include <iostream>
#include <string>


void Options::chooseOptions() {
	
	std::string s;





	bool choosingScene = true;
	while (choosingScene) {
		std::cout << "Which scene(s) would you like to render? (1-17 or 0 to render all scenes)" << std::endl;
		std::cout << "Choose an option:  ";

		int i = getUserInputInteger();
		if (i >= 0 && i <= 17) {
			sceneNum = i;
			choosingScene = false;
		}

		std::cout << std::endl;
	}



	bool choosingSize = true;
	while (choosingSize) {
		std::cout << "Which size image(s) do you want to render? (height)" << std::endl;
		std::cout << "[1] Miniscule,\t25px" << std::endl;
		std::cout << "[2] Tiny,\t\t50px" << std::endl;
		std::cout << "[3] Small,\t\t100px" << std::endl;
		std::cout << "[4] Medium,\t\t200px" << std::endl;
		std::cout << "[5] Large,\t\t400px" << std::endl;
		std::cout << "[6] Huge,\t\t800px" << std::endl;
		std::cout << "[7] Enormous,\t\t1600px" << std::endl;
		std::cout << "Choose an option:  ";

		int i = getUserInputInteger();
		if (i >= 1 && i <= 7) {
			outputHeight = 25 * (int)pow(2, i-1);
			choosingSize = false;
		}

		std::cout << std::endl;
	}


	bool choosingAntialiasing = true;
	while (choosingAntialiasing) {
		std::cout << "What level of antialiasing do you want? (0-3)" << std::endl;
		std::cout << "Choose an option:  ";

		int i = getUserInputInteger();
		if (i >= 0 && i <= 3) {
			antialiasing = i;
			choosingAntialiasing = false;
		}

		std::cout << std::endl;
	}

}



int Options::getUserInputInteger() {
	while (true) {
		int tmp;
		std::cin >> tmp;
		if (!std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			fflush(stdin);
			return tmp;
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		fflush(stdin);
		std::cout << "Invalid input!" << std::endl;
	}
}