#pragma once

#include "Enums.h"

//#include <memory>

class GameActor;
class Building;

class GameData
{
private:
    //TODO: make this array two-dimensional
    TerrainType* mapTerrain = nullptr;
    Image* tileset;
    Texture2D tilesetTex[8];
    Texture2D terrainTexture;
    Texture2D creepTexture;
    Color* palette;
    std::vector<GameActor*> unitsList;
    std::vector<Building*> expansionUnitsList;

    int mapHeight; // num of tile rows
    int mapWidth; // num of tile columns
    
    Camera2D camera = { 0 }; 
    Vector2 mousePosition;
    Vector2 mouseWorldPosition;
    TileIndex mouseIndex;
    Vector2 screenSize;
    Vector2 mapSize; //mapsize in pixels
    Rectangle viewBorders;
    int renderBorders[4]; //indices: top horiz, left vert, bottom horiz, right vert

    ActorType wantToBuild = ActorType::ACTOR_NULL;
    bool wantToRemove = false;

    //INTERFACE
    bool showingCreepStates = false;

    //��� ����� �������� ������ �� ������� (����� �����-�� ��������� �� STL)
    //��� �� � ����� �������� ����� ����� � �.�.

    //���-�� ��������
    //��� �� � ������� ���������
    
    //void loadActorsAttributes();
    //�������� ��� ������ "��������", � �������� �� ����� � ����� �����?

    //fog of war maps
    std::map<Side, int**> mapsFogOfWar;
    TileIndex insectsDesirePosition = {0,0};

public:
    unsigned short timeCount; //for Update()
    unsigned int lastID = 0;

    unsigned int radius = 1;
    const float pixelsPerTile = 16.f;

    int getMaxWidth() { return mapWidth; }
    int getMaxHeight() { return mapHeight; }

    //attributes for GameActor constructor
    std::map<ActorType, std::map<std::string, int>> genericAttributes;
    std::map<ActorType, std::map<std::string, int>> buildingsAttributes;
    std::map<ActorType, std::map<std::string, int>> militaryAttributes;

    //ECONOMICS
    //num of creep-covered tiles
    unsigned int creepCount = 0;

    //VECTOR PATHFINDING
    NeighborsIndex** neighborsIndices = nullptr;
    //have inside mapsVector, mapsHeat, mapsTerrainMod, mapsDamage
    std::map<ActorType, std::map<std::string, float**>> mapsPathfinding;
    std::map<ActorType, Vector2**> vectorFields;

    //mapVector - target mapVector to calculate, mapTerrainMod - speed modification by terrain type, mapDamage - modification by receiving/dealing damage 
    void calculateVectorPathfinding(TileIndex target, ActorType actorType);

    //reveals fog of war in circle with radius (in tiles) around TileIndex position
    void revealTerritory(TileIndex position, int radius, Side side);

    unsigned char** mapExpansionCreep = nullptr;
    TerrainType getTerrainType(int x, int y); //{return this->mapTerrain[x][y]};
    bool closed = false;
    std::vector<TileIndex>tilesInsideCircle(Vector2 center, unsigned int radius);
    std::vector<TileIndex>tilesInsideCircleOrdered(TileIndex center, int radius);
    //std::vector<TileIndex>tilesInPerimeterCircle(TileIndex center, unsigned int radius);
    
    //return num of adjoined expansion tiles 
    int numOfExpansionTileAdjoin(int x, int y, Side side); 
    int numOfExpansionTileAdjoinFading(int x, int y, Side side);

    bool isTileExpanded(TileIndex tile, Side side);
    bool isTileInBounds(TileIndex tile);

    //recalculate state of expansion tiles
    void recalculateExpansion(Side side);
    
    //return TileIndex of Vector2
    TileIndex getTileIndex(Vector2);

    //returns vector of neightbors indices
    NeighborsIndex getNeighbors(int x, int y);
    //returns vector of neighbors tiles
    std::vector<TileIndex> getNeighborsAsVector(int x, int y);

    //returns pointer to actor in coordinates
    GameActor* getActorInTile(int x, int y);
    //returns vector of actors pointers in radius of circle
    std::vector<GameActor*> getActorsInRadius(Vector2 center, float radius);
    GameActor* getNearestSpecificActor(Vector2 position, std::vector<GameActor*> actors, ActorType type, GameActor* caller = nullptr);

    void addActor(ActorType type, Vector2 position, State state); //add actor on map, on full health or not - depends on "state" and debug mod on/off
    void removeActor(unsigned int ID);


    void setTerrain(Terrain);
    bool isMapLoaded();
    void clearMap();

    TileIndex getInsectsDesirePosition() { return insectsDesirePosition; };

    //��� ������ ���������� � ��������� �� ���� �������, ���������� ������������� ������ � �.�.
    void GameDraw(); 
    void GameUpdate();
    GameData();
    ~GameData();

    void saveToFile(std::string fileName);
    void loadFromFile(std::string fileName);
};

class GameActor {
protected:
    Texture2D* sprite; //�������� �� ��������� ��������+������������� ����� ������? ���� �������, ��� �������� ����������� � ������
    unsigned short animationFrame; //current animation 

    Vector2 position;
    TileIndex positionIndex;
    State state;
    
    int HP;
    int progress; //for construction? or just use state in Update
    bool selectable; //�������? ��� ����� ������������� ������ ����������, � ������� �������� �� ���/������� �� selected, ����� � Draw �������� �����, � � GUI ������� ��������� �� ���������� ������

public:
    GameData* game;
    const ActorType type;
    unsigned int ID; //private?
    Side side;
    std::string name;

    //make consts?
    int maxHP;
    int size;
    int armor;
    int cost;

    //int buildCount; //just tune buildRate in cores and base
    unsigned int sightRange; //��� �������� ������������ ������ �������� ����������� ����� �����, ��� military(� ������� � ��������� ������) ��������� ������ ��� (��� ������ ������� �� ����� ������)

    virtual void Draw() = 0;
    virtual void Update() = 0; 
    virtual void Destroy() = 0; //������� �������/������� �� �����, �������� ���� �� �����������, ������ ������������ ���������� �/��� ������� �� �������-��������� ������ ����
    virtual ~GameActor() 
    {        
    }

    int getHP() { return this->HP; }
    Vector2 getPosition() { return this->position; }
    void Hit(int damage, ActorType hitBy)
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

    //when spawning new unit, use GameData unit settings and ActorType for seting mapHP, size, etc. State defines currentHP and, maybe, something else
    GameActor(GameData *ptr, ActorType type, Vector2 pos, State state)
        :game(ptr), type(type), position(pos), state(state)
    {
        size = ptr->genericAttributes[type]["size"];
        maxHP = ptr->genericAttributes[type]["maxHP"];
        cost = ptr->genericAttributes[type]["cost"];
        sightRange = ptr->genericAttributes[type]["sightRange"];
        positionIndex = ptr->getTileIndex(pos);

        switch (type)
        {
        case ActorType::LIGHT_TURRET:
            //add seting properties here
        case ActorType::HEAVY_TURRET:
            //and here
        case ActorType::AIRDEFENSE_TURRET:
            //and everywhere
        case ActorType::CORE:
        case ActorType::BASE:
            side = Side::MACHINES;
            break;
        case ActorType::LIGHT_INSECT:
        case ActorType::HEAVY_INSECT:
        case ActorType::FLYING_INSECT:
        case ActorType::HIVE:
        case ActorType::TUMOR:

            side = Side::INSECTS;
            break;
        }
        this->ID = this->game->lastID;
        this->game->lastID++;

        ptr->revealTerritory(positionIndex, sightRange, side);
    }
};

class Building: public GameActor {

private:
    int expansionRange;
    bool expanded; //true, if all tiles around this building is filled by expansion
    std::vector<TileIndex> expansionIndices;

public:
    Building(GameData* ptr, ActorType type, Vector2 pos, State state);
    void markExpandArea();
    void Expand();
    virtual ~Building() = 0;
    //��� ��������: ���������, ���� �� � ������� expansionRange ��������� ����� �� ����� (�������� ����� ���������)
    //�������� �� ��� 2-4 �����, ������� ���� ���� ��������:
    //��������� �� ������� ������� ���������, �������� �������� �� ���� ������ �� ����� � ������� �� ����� �� ������

    //��������� �����: ��� � n*m ������ ��������� ��� ����� ����� �� ������� ����� ��������� �����, ���� ��� - ��������� 
};

class Tumor : public Building
{
public:
    void Update();
    void Draw();
    void Destroy();
    Tumor(GameData* ptr, ActorType type, Vector2 pos, State state);
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

class Core : public Constructor
{
    void Update();
    void Draw();
};

class Militaty : public GameActor
{
protected:
    int seekRange;
    int attackRange;
    int speed;
    int damage;
    int reloadCount;
    int rotationSpeed;
    int angle;
    Vector2 velocityVector;

    //int type
    GameActor* target;
    ActorType targetPriority;

    Militaty(GameData* ptr, ActorType type, Vector2 pos, State state);
    ~Militaty();

    void Attack();
    void SeekForEnemy();
    void Move();
    void Reload();
    void Targeting();
};

class LightInsect : public Militaty
{
public:
    LightInsect(GameData* ptr, ActorType type, Vector2 pos, State state);
    void Update();
    void Draw();
    void Destroy();
};