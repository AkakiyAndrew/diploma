#pragma once
#include "raylib.h"
#include <string>
#include "Enums.h"
#include <algorithm>
#include <vector>

class GameData
{
private:
	//TODO: make this array two-dimensional
	TerrainType* mapTerrain = nullptr;
	Image* tileset;
	Texture2D tilesetTex[7];
	Texture2D terrainTexture;
	Color* palette;

	unsigned char** mapExpansionCreep = nullptr;

	int mapHeight; // num of tiles
	int mapWidth;
	const float pixelsPerTile = 16.f;
	
	Camera2D camera = { 0 }; 
	Vector2 mousePosition;
	Vector2 mouseWorldPosition;
	TileIndex mouseIndex;
	Vector2 screenSize;
	Vector2 mapSize; //mapsize in pixels
	Rectangle viewBorders;
	int renderBorders[4]; //indices: top horiz, left vert, bottom horiz, right vert

	ActorType wantToBuild = ActorType::ACTOR_NULL;
	

	//��� ����� �������� ������ �� ������� (����� �����-�� ��������� �� STL)
	//��� �� � ����� �������� ����� ����� � �.�.

	//���-�� ��������

	//��� �� � ������� ���������?
	std::vector<TileIndex>tilesCircleIntersecting(Vector2 center, unsigned int radius);
	
	void saveToFile(std::string fileName);
	void loadFromFile(std::string fileName);
	//void loadSettings();

	//�������� ��� ������ "��������", � �������� �� ����� � ����� �����?

public:
	bool closed = false;

	void addActor(ActorType type, Status state); //add actor on map, on full health or not - depends on "state" and debug mod on/off

	//��� ������ ���������� � ��������� �� ���� �������, ���������� ������������� ������ � �.�.
	void setTerrain(Terrain);
	bool isMapLoaded();
	void GameDraw(); 
	void GameUpdate();
	GameData();
	~GameData();
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
