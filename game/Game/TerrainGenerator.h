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
	double frequency, fx, fy;

	Color* palette;

	float* levels;

	double* heightMap;

public:
	TerrainGenerator(void);
	void RegenerateTerrain(); // ������� ����� ����� ����� ����� ������� (��������������!)
	void RerenderTerrain(); // ���������� ��� ����� �������� ��������� (��������� ��������� ������ ��� ����������, ��������������?), �������� ������ ���� ������ ���� ������
	void DrawInterface();
	~TerrainGenerator();
};