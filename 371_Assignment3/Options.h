#pragma once



class Options {

public:

	int sceneNum;
	int outputHeight;
	int antialiasing;
	int softShadows;

	void chooseOptions();

private:
	int getUserInputInteger();
	
};