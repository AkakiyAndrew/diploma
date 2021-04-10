#pragma once

#include "PerlinNoise.h"
#include "GameClasses.h"
#include "raylib.h"
#include "raygui.h"

class TerrainGenerator
{
private:
	bool closed;
	int width, height, seed, octave;

	float lakeLevel, mountainLevel;


public:
	TerrainGenerator();
	void DrawInterface();
};