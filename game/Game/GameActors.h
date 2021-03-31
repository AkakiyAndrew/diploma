#pragma once
#include "raylib.h"
#include <string>

enum Side
{
	Machines,
	Insects
};

enum Status
{
	Online,
	Offline,
	Construction,
	Building,
	Attacking,
	Goes,
	Attacking, //�������� ��� ������� ����� � ������� � ���������
};

enum Type 
{
	//Machines turrets
	LightTurret,
	HeavyTurret,
	AirDefenseTurret,

	//Insects units
	LightInsect,
	HeavyInsect,
	FlyingInsect,

	//Machines buildings
	Core,
	Base,

	//Insects buildings
	Hive,
	Tumor
};

class GameActor {

private:
	int HP;
	int size;
	int armor;
	int cost;
	int buildCount;
	int progress;
	int ID;
	int sightRange;
	Vector2 position;
	Side side;
	std::string name;
	Status status;
	bool selectable; //�������? ��� ����� ������������� ������ ����������, � ������� �������� �� ���

public:
	static Texture2D *sprite; //???, ������� ������� ��������? ��, �������, � Draw ������������ ������� �� ������ � ��� �����, � ������������ �������� �������� ��������� �� ���� 



};