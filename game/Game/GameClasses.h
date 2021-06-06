#pragma once

#include "Enums.h"

//#include <memory>

class GameActor;
class Building;
class Constructor;
class Connectable;
class Militaty;
class Turret;

class GameData
{
private:
    //TODO: make this array two-dimensional
    TerrainType* mapTerrain = nullptr;
    Image* tileset;
    Texture2D tilesetTex[8];
    Texture2D terrainTexture;
    Texture2D expansionInsectsTexture;
    Animation expansionMachinesAnimation;
    int currentFrame = 0;
    std::map<ActorType, std::map<State, Animation>> unitAnimations;
    Color* palette;
    
    std::vector<GameActor*> unitsList;
    std::vector<Building*> expansionUnitsList_Insects;
    std::vector<Building*> expansionUnitsList_Machines;
    std::vector<Militaty*> militaryUnitsList;
    std::vector<Turret*> turretUnitsList;

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
    Side visionSide = Side::MACHINES;


    //тут же и будут хранится карты урона и т.п.

    //RESOURCES
    unsigned resourcesInsects = 0;
    unsigned resourcesMachines = 100;

    //оставить тут методы "приказов", а вызывать их извне в общем цикле?

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
    std::map<ActorType, std::map<std::string, int>> connectableAttributes;
    std::map<ActorType, std::map<std::string, int>> constructorsAttributes;
    std::map<ActorType, std::map<std::string, int>> turretsAttributes;

    Animation getUnitAnimation(ActorType type, State state) { return unitAnimations[type][state]; }

    //ECONOMICS
    //returns amount of resources, that can be used
    int trySpendResources(int amount, Side side);
    //actual spend of resources
    void spendResources(int amount, Side side);
    //num of creep-covered tiles
    unsigned int creepTilesCount = 0;
    //num of energy layer tiles
    unsigned int energisedTilesCount = 0;

    unsigned char** mapExpansionCreep = nullptr;
    unsigned char** mapExpansionEnergised = nullptr;

    //return num of adjoined expansion tiles 
    int numOfExpansionTileAdjoin(int x, int y, Side side);
    int numOfExpansionTileAdjoinFading(int x, int y, Side side);

    bool isTileExpanded(TileIndex tile, Side side);
    bool isTileInBounds(TileIndex tile);

    //recalculate state of expansion tiles
    void recalculateExpansion(Side side);
    //if some unit dies, recalculate targets for opposite side
    void recalculateMilitaryTargets(Side side);

    //VECTOR PATHFINDING
    //pre-calculated matrix with neighbors indices
    NeighborsIndex** neighborsIndices = nullptr;
    //have inside mapsVector, mapsHeat, mapsTerrainMod, mapsDamage
    std::map<ActorType, std::map<std::string, float**>> mapsPathfinding;
    std::map<ActorType, Vector2**> vectorFields;

    //mapVector - target mapVector to calculate, mapTerrainMod - speed modification by terrain type, mapDamage - modification by receiving/dealing damage 
    void calculateVectorPathfinding(TileIndex target, ActorType actorType);

    //reveals fog of war in circle with radius (in tiles) around TileIndex position
    void revealTerritory(TileIndex position, int radius, Side side);

    TerrainType getTerrainType(int x, int y); //{return this->mapTerrain[x][y]};
    bool closed = false;
    std::vector<TileIndex>tilesInsideCircle(Vector2 center, unsigned int radius);
    std::vector<TileIndex>tilesInsideCircleOrdered(TileIndex center, int radius);
    //std::vector<TileIndex>tilesInPerimeterCircle(TileIndex center, unsigned int radius);
    
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
    GameActor* getNearestSpecificActor(Vector2 position, std::vector<GameActor*> actors, GameActor* caller, ActorType type = ActorType::ACTOR_NULL);
    //returns true, if no obstacles on line of sight for this unit
    bool isOnLineOfSight(TileIndex pos1, TileIndex pos2, ActorType unitType);

    void addActor(ActorType type, Vector2 position, State state); //add actor on map, on full health or not - depends on "state" and debug mod on/off
    void removeActor(unsigned int ID);
    void Hit(GameActor* target, int damage, ActorType hitBy); //return true, if actor destroyed by hit

    void setTerrain(Terrain);
    bool isMapLoaded();
    void clearMap();

    TileIndex getInsectsDesirePosition() { return insectsDesirePosition; };
    
    //для вызова обновлений и отрисовки по всем актерам, вычисления экономических тайлов и т.п.
    void GameDraw(); 
    void GameUpdate();
    GameData();
    ~GameData();

    void saveToFile(std::string fileName);
    void loadFromFile(std::string fileName);
};

class GameActor {
protected:
    Animation sprite; //заменить на структуру анимации+максимального числа кадров? надо глянуть, как анимация реализована у других
    unsigned short currentFrame; //current animation 

    Vector2 position;
    TileIndex positionIndex;
    State state;
    
    int HP;
    bool selectable; //удалить? все равно просматривать данные необходимо, а приказы отдавать не тут/сменить на selected, чтобы в Draw рисовать рамку, а в GUI хранить указатель на выбранного актера
    int angle; //float?

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

    int inBattleCounter = 0;
    bool inBattle = false; //if in battle - cant be repaired

    //int buildCount; //just tune buildRate in cores and base
    unsigned int sightRange; //при создании неподвижного актера единожды разведывать туман войны, для military(и турелей в мобильном режиме) обновлять каждый тик (или каждый переход на новую клетку)

    virtual void Draw() = 0;
    virtual void Update() = 0; 
    virtual void Destroy() = 0; //создать обломки/ошметки на карте, возможно даже не виртуальный, внутри использовать деструктор и/или удалять из вектора-хранилища данных игры
    virtual ~GameActor() 
    {        
    }

    //when spawning new unit, use GameData unit settings and ActorType for seting mapHP, size, etc. State defines currentHP and, maybe, something else
    GameActor(GameData* ptr, ActorType type, Vector2 pos, State state)
        :game(ptr), type(type), position(pos), state(state)
    {
        size = ptr->genericAttributes[type]["size"];
        maxHP = ptr->genericAttributes[type]["maxHP"];
        cost = ptr->genericAttributes[type]["cost"];
        sightRange = ptr->genericAttributes[type]["sightRange"];
        positionIndex = ptr->getTileIndex(pos);

        switch (state)
        {
        case State::ONLINE:
        case State::OFFLINE:
        case State::GOES:
            HP = maxHP;
            break;

        case State::UNDER_CONSTRUCTION:
            HP = 0;
            break;
        default:
            break;
        }

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

        sprite = game->getUnitAnimation(type, state);
    }

    //draw bar with HP
    void drawHP() 
    {
        if (HP != maxHP)
        {
            DrawRectangle(position.x - size/2, position.y + size, size, 2, RED);
            if(HP!=0)
                DrawRectangle(position.x - size/2, position.y + size, size * ((float)HP / (float)maxHP), 2, GREEN);
        }
    };
    void isInBattleCheck() 
    {
        inBattleCounter--;
        if (inBattleCounter <= 0)
        {
            inBattleCounter = 0;
            inBattle = false;
        }
    }
    State getState() { return state; }
    int getHP() { return this->HP; }
    void setHP(int amount) { this->HP = amount; }
    Vector2 getPosition() { return this->position; }
    TileIndex getPositionIndex() { return this->positionIndex; }
    
    //for building construction, repair and insects units and buildings regeneration
    int RestoreHP(int amount)
    {
        int restored;

        if ((this->HP + amount) > this->maxHP)
        {
            restored = this->maxHP - this->HP;
            this->HP = this->maxHP;
        }
        else
        {
            restored = amount;
            this->HP += amount;
        }
            
        return restored;
    }
};

class Building: public GameActor {

private:
    int expansionRange;
    int expansionTime;
    bool expanded; //true, if all tiles around this building is filled by expansion
    std::vector<TileIndex> expansionIndices;

public:
    Building(GameData* ptr, ActorType type, Vector2 pos, State state);
    void markAreaFade();
    void markAreaExpand();
    void Expand();
    virtual ~Building() = 0;
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
    //TODO: idea, common Draw() method for constructors

private:
    int buildPower;
    int buildRange;
    GameActor* target = nullptr;

protected:
    std::vector<Connectable*>connectedUnits;

public:
    Constructor(GameData* ptr, ActorType type, Vector2 pos, State state);
    ~Constructor();

    //returns true, if attaching unit is not a parent of this constructor, to prevent locking loops
    bool RequestAttachment(Connectable* unit);
    void UnAttach(Connectable* unit);
    void BuildOrRepair();
    void SeekForTarget();
    void DrawBuildingRay();
};

class Connectable
{
private:
    int connectRange;
    GameData* gameConn;
    
protected:
    Constructor* parent;
    //return true, if connection succesful
    bool TryConnect(Vector2 position, int ID);

public:
    //recursive checking, is this unit have chain connection to Base
    Connectable(GameData *ptr, ActorType type);
    ~Connectable();

    //use in destructor, or when parent destructed
    virtual void Disconect();
};

class Core : public Constructor, public Connectable
{
public:
    Core(GameData* ptr, ActorType type, Vector2 pos, State state);
    void Disconect();
    void Update();
    void Draw();
    void Destroy();
};

class Base : public Constructor
{
public:
    Base(GameData* ptr, ActorType type, Vector2 pos, State state);
    void Update();
    void Draw();
    void Destroy();
};

class Militaty : public GameActor
{
protected:
    int seekRange;
    int attackRange;
    int speed;
    int damage;
    int cooldownDuration; //how much ticks need to recharge
    int cooldownRemain; //amount of ticks, remains to reload
    int rotationSpeed;
    
    Vector2 velocityVector;

    GameActor* target;
    ActorType targetPriority;

    Militaty(GameData* ptr, ActorType type, Vector2 pos, State state);
    ~Militaty();
    
    virtual void Attack() = 0;
    void Reload(); //IN TURRETS USE ONLY WHEN charge>=energyPerShot!
    void Targeting(); //turning actor in direction of enemy
    void DrawReloadBar();

public:
    void SeekForEnemy();
};

class Insect : public Militaty
{
    void Move();
public:
    Insect(GameData* ptr, ActorType type, Vector2 pos, State state);
    void Attack();
    void Update();
    void Draw();
    void Destroy();
};

class Turret : public Militaty, public Connectable
{
protected:
    Animation chasisSprite;
    int chasisCurrentFrame;
    int chasisAngle;

    bool isMounted = true;
    int maxCharge;
    int charge; //amount of accumulated energy
    int chargeRate; //how much energy takes per second
    int energyPerShot;
    std::vector<TileIndex> waypoints;
    int modeProgressCounter = 0; //for changing mode
    int attackProgressCounter = 0; //for counting attack

    void Move();
    void Recharge();
    void DrawChargeBar();
    void Attack();

public:
    void ChangeMode();
    //common for all of turrets (make only Attack() uniq):
    void Update();
    void Draw();
    void Destroy();
    Turret(GameData* ptr, ActorType type, Vector2 pos, State state);
    //~Turret();
};