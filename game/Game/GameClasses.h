#pragma once
#include "raylib.h"
#include <string>
#include "Enums.h"

class GameData
{
private:
	TerrainType* mapTerrain = nullptr;
	Image* tileset;
	Texture2D tilesetTex[7];

	int mapHeight; // num of tiles
	int mapWidth;
	const float pixelsPerTile = 16.f;
	

	Camera2D camera = { 0 }; 
	Vector2 cursor;
	Vector2 screenSize;
	Vector2 mapSize; //mapsize in pixels
	Texture2D terrainTexture;
	

	//тут будут хранится ссылки на актеров (взять какой-то контейнер из STL)
	//тут же и будут хранится карты урона и т.п.

	//кол-во ресурсов

	//тут же и хранить настройки?

	
	void saveToFile(std::string fileName);
	void loadFromFile(std::string fileName);
	//void loadSettings();

	//оставить тут методы "приказов", а вызывать их извне в общем цикле?

public:
	bool closed = false;

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
	int currentHP;
	int maxHP;
	int size;
	int armor;
	int cost;
	int buildCount;
	int progress;
	int ID;
	int sightRange; //при создании неподвижного актера единожды разведывать туман войны, для military(и турелей в мобильном режиме) обновлять каждый тик (или каждый переход на новую клетку)
	Vector2 position;
	Side side;
	std::string name;
	Status status;
	bool selectable; //удалить? все равно просматривать данные необходимо, а приказы отдавать не тут/сменить на selected, чтобы в Draw рисовать рамку, а в GUI хранить указатель на выбранного актера
	Texture2D* sprite; //заменить на структуру анимации+текущего кадра? надо глянуть, как анимация реализована у других

public:
	 //???, вынести спрайты отдельно? да, вынести, в Draw использовать указатель на спрайт и его юзать, в конструкторе получать значение указателя по типу 
	virtual void Draw() = 0;
	virtual void Update() = 0; 
	virtual void Destroy() = 0; //создать обломки/ошметки на карте, возможно даже не виртуальный, внутри использовать деструктор

	GameActor(ActorType type) //принимать просто тип(может, еще и статус?), и выставлять стартовые значения, взятые из настроек игры, прямо тут?
	{

	}

};



class Building: public GameActor {

private:
	int expansionRange;

public:
	void Expand()
	{
		//для опухолей: проверяет, есть ли в области expansionRange свободное место от слизи (учитывая карту экспансии)
		//спавнить за раз 2-4 тайла, каждому свой цикл проверок:
		//проходить по спирали радиуса экспансии, проверяя является ли тайл пустым от слизи и имеется ли сосед со слизью

		//убываение слизи: раз в n*m циклов проверять все тайлы слизи на наличие рядом источника слизи, если нет - проверять 

	}

};
