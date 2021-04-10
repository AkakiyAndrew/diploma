#pragma once

#include "PerlinNoise.h"
#include "GameClasses.h"
#include "Enums.h"
#include "raylib.h"
#include "raygui.h"

class TerrainGenerator
{
private:
	bool closed, generated;
	int width, height, seed, octave;

	float levels[4] = { 0., 0.25, 0.75, 1. };

	float lakeLevel, mountainLevel; //заменить на один массив с пределами (с учетом нижнего и верхнего пределов соответственно - 0. и 1.)


public:
	TerrainGenerator();
	void RegenerateTerrain(); // создать новую карту высот шумом Перлина (распараллелить!)
	void RerenderTerrain(); // вызывается при смене значения ползунков (проверить насколько быстро это происходит, распараллелить?)
	void DrawInterface();
	~TerrainGenerator();
};