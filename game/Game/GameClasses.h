#pragma once
#include "raylib.h"
#include <string>
#include "Enums.h"
#include <algorithm>
#include <vector>
//#include <memory>

class GameActor;

class GameData
{
private:
    //TODO: make this array two-dimensional
    TerrainType* mapTerrain = nullptr;
    Image* tileset;
    Texture2D tilesetTex[8];
    Texture2D terrainTexture;
    Color* palette;
    std::vector<GameActor*> unitsList;

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
    

    //тут будут хранится ссылки на актеров (взять какой-то контейнер из STL)
    //тут же и будут хранится карты урона и т.п.

    //кол-во ресурсов

    //тут же и хранить настройки?
    
    //void loadSettings();

    //оставить тут методы "приказов", а вызывать их извне в общем цикле?

public:
    unsigned short timeCount; //for Update()

    unsigned int radius = 1;
    const float pixelsPerTile = 16.f;

    unsigned char** mapExpansionCreep = nullptr;
    TerrainType getTerrainType(int x, int y); //{return this->mapTerrain[x][y]};
    bool closed = false;
    std::vector<TileIndex>tilesInsideCircle(Vector2 center, unsigned int radius);
    std::vector<TileIndex>tilesInsideCircleOrdered(TileIndex center, int radius);
    std::vector<TileIndex>tilesInPerimeterCircle(TileIndex center, unsigned int radius);
    void addActor(ActorType type, Vector2 position, State state); //add actor on map, on full health or not - depends on "state" and debug mod on/off

    //для вызова обновлений и отрисовки по всем актерам, вычисления экономических тайлов и т.п.
    void setTerrain(Terrain);
    bool isMapLoaded();
    void GameDraw(); 
    void GameUpdate();
    GameData();
    ~GameData();

    void saveToFile(std::string fileName);
    void loadFromFile(std::string fileName);
};

class GameActor {
protected:
    Texture2D* sprite; //заменить на структуру анимации+максимального числа кадров? надо глянуть, как анимация реализована у других
    unsigned short animationFrame; //current animation 

    int progress; //for construction? or just use state in Update
    Vector2 position;
    State state;
    int HP;
    
    bool selectable; //удалить? все равно просматривать данные необходимо, а приказы отдавать не тут/сменить на selected, чтобы в Draw рисовать рамку, а в GUI хранить указатель на выбранного актера

public:
    GameData* game;
    const ActorType type;
    int ID; //private?
    Side side;
    std::string name;

    //make consts?
    int maxHP;
    int size;
    int armor;
    int cost;

    //int buildCount; //just tune buildRate in cores and base
    int sightRange; //при создании неподвижного актера единожды разведывать туман войны, для military(и турелей в мобильном режиме) обновлять каждый тик (или каждый переход на новую клетку)
    
    virtual void Draw() = 0;
    virtual void Update() = 0; 
    virtual void Destroy() = 0; //создать обломки/ошметки на карте, возможно даже не виртуальный, внутри использовать деструктор и/или удалять из вектора-хранилища данных игры

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

    //when spawning new unit, use GameData unit settings and ActorType for seting mapHP, size, etc. State defines currentHP and, maybe, something else
    GameActor(GameData *ptr, ActorType type, Vector2 pos, State state)
        :game(ptr), type(type), position(pos), state(state)
    {
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
            size = 8;
            side = Side::INSECTS;
            break;
        }
    }
};

class Building: public GameActor {

private:
    TileIndex positionIndex;
    int expansionRange;
    bool expanded; //true, if all tiles around this building is filled by expansion
    std::vector<TileIndex> expansionIndices;

public:
    Building(GameData* ptr, ActorType type, Vector2 pos, State state);
    void Expand();

    //для опухолей: проверяет, есть ли в области expansionRange свободное место от слизи (учитывая карту экспансии)
    //спавнить за раз 2-4 тайла, каждому свой цикл проверок:
    //проходить по спирали радиуса экспансии, проверяя является ли тайл пустым от слизи и имеется ли сосед со слизью

    //убываение слизи: раз в n*m циклов проверять все тайлы слизи на наличие рядом источника слизи, если нет - проверять 
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