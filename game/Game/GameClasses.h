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
    

    //тут будут хранится ссылки на актеров (взять какой-то контейнер из STL)
    //тут же и будут хранится карты урона и т.п.

    //кол-во ресурсов

    //тут же и хранить настройки?
    std::vector<TileIndex>tilesCircleIntersecting(Vector2 center, unsigned int radius);
    
    void saveToFile(std::string fileName);
    void loadFromFile(std::string fileName);
    //void loadSettings();

    //оставить тут методы "приказов", а вызывать их извне в общем цикле?

public:
    bool closed = false;

    void addActor(ActorType type, State state); //add actor on map, on full health or not - depends on "state" and debug mod on/off

    //для вызова обновлений и отрисовки по всем актерам, вычисления экономических тайлов и т.п.
    void setTerrain(Terrain);
    bool isMapLoaded();
    void GameDraw(); 
    void GameUpdate();
    GameData();
    ~GameData();
};

class GameActor {
protected:
    const Texture2D* sprite; //заменить на структуру анимации+максимального числа кадров? надо глянуть, как анимация реализована у других
    unsigned int animationFrame; //current animation 

    int progress; //for construction? or just use state in Update
    Vector2 position;
    State state;
    int HP;
    
    bool selectable; //удалить? все равно просматривать данные необходимо, а приказы отдавать не тут/сменить на selected, чтобы в Draw рисовать рамку, а в GUI хранить указатель на выбранного актера

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
    const int sightRange; //при создании неподвижного актера единожды разведывать туман войны, для military(и турелей в мобильном режиме) обновлять каждый тик (или каждый переход на новую клетку)
    
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
    

    //GameActor(ActorType type) //принимать просто тип(может, еще и статус?), и выставлять стартовые значения, взятые из настроек игры, прямо тут?
    //{

    //}

};

class Building: public GameActor {

private:
    int expansionRange;

public:
    void Expand();
        //для опухолей: проверяет, есть ли в области expansionRange свободное место от слизи (учитывая карту экспансии)
        //спавнить за раз 2-4 тайла, каждому свой цикл проверок:
        //проходить по спирали радиуса экспансии, проверяя является ли тайл пустым от слизи и имеется ли сосед со слизью

        //убываение слизи: раз в n*m циклов проверять все тайлы слизи на наличие рядом источника слизи, если нет - проверять 
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
