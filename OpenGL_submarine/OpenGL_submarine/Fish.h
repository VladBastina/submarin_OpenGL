#pragma once

#include "Submarine.h"

class Fish
{
private:
	Model fish;
	Shader* fishShader;

public:
	Fish();

	void Render(SubmarineCamera* camera,float aspectRatio);

};

