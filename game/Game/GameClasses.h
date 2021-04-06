#pragma once
#include "raylib.h"
#include <string>

enum class Side
{
	MACHINES,
	INSECTS
};

enum class Status
{
	ONLINE,
	OFFLINE,
	CONSTRUCTION,
	BUILDING,
	ATTACKING,
	GOES,
	ATTACKING, //�������� ��� ������� ����� � ������� � ���������
};

enum class ActorType
{
	//Machines turrets
	LIGHT_TURRET,
	HEAVY_TURRET,
	AIRDEFENSE_TURRET,

	//Insects units
	LIGHT_INSECT,
	HEAVY_INSECT,
	FLYING_INSECT,

	//Machines buildings
	CORE,
	BASE,

	//Insects buildings
	HIVE,
	TUMOR
};

enum class TerrainType
{
	MOUNTAIN,
	SWAMP,
	TREE,
	LAKE,
	PLAIN,
	STONE,
	SAND,
	ASH,
};

class GameData
{
	TerrainType* mapTerrain;
	int mapHeight;
	int mapWidth;

	//��� ����� �������� ������ �� ������� (����� �����-�� ��������� �� STL)
	//��� �� � ����� �������� ����� ����� � �.�.

	//���-�� ��������

	//��� �� � ������� ���������?

	void generateMap(int height, int width);
	void saveToFile(std::string fileName);
	void loadFromFile(std::string fileName);
	//void loadSettings();

	//�������� ��� ������ "��������", � �������� �� ����� � ����� �����?

	//��� ������ ���������� � ��������� �� ���� �������, ���������� ������������� ������ � �.�.
	void GameDraw(); 
	void GameUpdate();

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

	GameActor(ActorType type) //��������� ������ ���(�����, ��� � ������?), � ���������� ��������� ��������, ������ �� �������� ����, ����� ���?
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
