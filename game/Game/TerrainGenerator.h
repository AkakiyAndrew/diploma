#pragma once

#include "PerlinNoise.h"
#include "GameClasses.h"
#include "Enums.h"
#include "raylib.h"
#include "raygui.h"

class TerrainGenerator
{
private:
	bool closed, generated, rendered;
	int width, height, octaves;
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
	TerrainGenerator(void);
	void RegenerateTerrain(); // ������� ����� ����� ����� ����� ������� (��������������!)
	void RerenderTerrain(); // ���������� ��� ����� �������� ��������� (��������� ��������� ������ ��� ����������, ��������������?), �������� ������ ���� ������ ���� ������
	void DrawInterface();
	~TerrainGenerator();
};