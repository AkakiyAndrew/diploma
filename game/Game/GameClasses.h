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

    void addActor(ActorType type, State state); //add actor on map, on full health or not - depends on "state" and debug mod on/off

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
    const Texture2D* sprite; //�������� �� ��������� ��������+������������� ����� ������? ���� �������, ��� �������� ����������� � ������
    unsigned int animationFrame; //current animation 

    int progress; //for construction? or just use state in Update
    Vector2 position;
    State state;
    int HP;
    
    bool selectable; //�������? ��� ����� ������������� ������ ����������, � ������� �������� �� ���/������� �� selected, ����� � Draw �������� �����, � � GUI ������� ��������� �� ���������� ������

public:
    const GameData* game;
    int ID; //private?
    const Side side;
    std::string name;

    const int maxHP;
    const int size;
    const int armor;
    const int cost;

    const int buildCount;
    const int sightRange; //��� �������� ������������ ������ �������� ����������� ����� �����, ��� military(� ������� � ��������� ������) ��������� ������ ��� (��� ������ ������� �� ����� ������)
    
    virtual void Draw() = 0;
    virtual void Update() = 0; 
    virtual void Destroy() = 0; //������� �������/������� �� �����, �������� ���� �� �����������, ������ ������������ ���������� �/��� ������� �� �������-��������� ������ ����

    int getHP() { return this->HP; }
    Vector2 getPosition() { return this->position; }
    void Hit(int damage)
    {
        damage -= armor;
        this->HP -= damage;
        if (this->HP <= 0)
            this->Destroy();
    }
    
    //for building construction, repair (and insects units and buildings regeneration?)
    void RestoreHP(int amount)     {
        if ((this->HP + amount) > this->maxHP)
            this->HP = this->maxHP;
        else
            this->HP += amount;
    } 
    

    //GameActor(ActorType type) //��������� ������ ���(�����, ��� � ������?), � ���������� ��������� ��������, ������ �� �������� ����, ����� ���?
    //{

    //}

};

class Building: public GameActor {

private:
    int expansionRange;

public:
    void Expand();
        //��� ��������: ���������, ���� �� � ������� expansionRange ��������� ����� �� ����� (�������� ����� ���������)
        //�������� �� ��� 2-4 �����, ������� ���� ���� ��������:
        //��������� �� ������� ������� ���������, �������� �������� �� ���� ������ �� ����� � ������� �� ����� �� ������

        //��������� �����: ��� � n*m ������ ��������� ��� ����� ����� �� ������� ����� ��������� �����, ���� ��� - ��������� 
};

class Constructor : public Building
{
    GameActor* target = nullptr;

    int buildRate;
    int constructionRange;
    int buildRange;

    void BuildOrRepair()
    {
        if (target != nullptr)
        {
            //if have non-null pointer - just repair
            //repair
        }
        //seeking for target to repair must be in Update()!
    }

};
