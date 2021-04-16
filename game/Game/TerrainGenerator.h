#pragma once

#include "PerlinNoise.h"
#include "GameClasses.h"
#include "Enums.h"
#include "raylib.h"
#include "raygui.h"

class TerrainGenerator
{
private:
	bool generated, rendered;
	unsigned int width, height, octaves;
	int seed;
	double frequency, fx, fy;

	const char** levelNames;
	Color* palette;
	float* levels;
	double* noiseMap;
	TerrainType* terrainMap;
	Texture2D colorPreview;
	Texture2D grayPreview;

public:
	bool closed;

	TerrainGenerator(void);
	void RegenerateTerrain(); // создать новую карту высот шумом Перлина (распараллелить!)
	void RerenderTerrain(); // вызывается при смене значения ползунков (проверить насколько быстро это происходит, распараллелить?), вызывать только если кнопка мыши отжата
	void DrawInterface();
	~TerrainGenerator();

	Terrain getMap();
};