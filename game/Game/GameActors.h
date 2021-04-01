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

protected:
	int currentHP;
	int maxHP;
	int size;
	int armor;
	int cost;
	int buildCount;
	int progress;
	int ID;
	int sightRange; //��� �������� ������������ ������ �������� ����������� ����� �����, ��� military(� ������� � ��������� ������) ��������� ������ ��� (��� ������ ������� �� ����� ������)
	Vector2 position;
	Side side;
	std::string name;
	Status status;
	bool selectable; //�������? ��� ����� ������������� ������ ����������, � ������� �������� �� ���/������� �� selected, ����� � Draw �������� �����, � � GUI ������� ��������� �� ���������� ������
	Texture2D* sprite; //�������� �� ��������� ��������+�������� �����? ���� �������, ��� �������� ����������� � ������

public:
	 //???, ������� ������� ��������? ��, �������, � Draw ������������ ��������� �� ������ � ��� �����, � ������������ �������� �������� ��������� �� ���� 
	virtual void Draw() = 0;
	virtual void Update() = 0; 
	virtual void Destroy() = 0; //������� �������/������� �� �����, �������� ���� �� �����������, ������ ������������ ����������

	GameActor(Type type) //��������� ������ ���(�����, ��� � ������?), � ���������� ��������� ��������, ������ �� �������� ����, ����� ���?
	{

	}

};



class Building: public GameActor {

private:
	int expansionRange;

public:
	void Expand()
	{
		//��� ��������: ���������, ���� �� � ������� expansionRange ��������� ����� �� ����� (�������� ����� ���������)
		//�������� �� ��� 2-4 �����, ������� ���� ���� ��������:
		//��������� �� ������� ������� ���������, �������� �������� �� ���� ������ �� ����� � ������� �� ����� �� ������

		//��������� �����: ��� � n*m ������ ��������� ��� ����� ����� �� ������� ����� ��������� �����, ���� ��� - ��������� 

	}

};
