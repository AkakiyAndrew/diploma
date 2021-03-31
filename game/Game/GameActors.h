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
	Attacking, //ПОДУМАТЬ ПРО СИСТЕМУ АТАКИ И СЦЕПИТЬ С АНИМАЦИЕЙ
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
	bool selectable; //удалить? все равно просматривать данные необходимо, а приказы отдавать не тут

public:
	static Texture2D *sprite; //???, вынести спрайты отдельно? да, вынести, в Draw использовать указать на спрайт и его юзать, в конструкторе получать значение указателя по типу 



};