#pragma once



class Options {

public:

	int sceneNum;
	int outputHeight;
	int antialiasing;

	float pixelW;
	float pixelH;

	void chooseOptions();

private:
	int getUserInputInteger();
	
};