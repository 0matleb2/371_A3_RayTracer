#pragma once



class Options {

public:

	int sceneNum;
	int outputHeight;
	int antialiasing;
	int softShadows;

	float pixelW;
	float pixelH;

	void chooseOptions();

private:
	int getUserInputInteger();
	
};