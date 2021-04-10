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

	float lakeLevel, mountainLevel; //�������� �� ���� ������ � ��������� (� ������ ������� � �������� �������� �������������� - 0. � 1.)


public:
	TerrainGenerator();
	void RegenerateTerrain(); // ������� ����� ����� ����� ����� ������� (��������������!)
	void RerenderTerrain(); // ���������� ��� ����� �������� ��������� (��������� ��������� ������ ��� ����������, ��������������?)
	void DrawInterface();
	~TerrainGenerator();
};