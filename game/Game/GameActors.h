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

	GameActor(Type type) //принимать просто тип(может, еще и статус?), и выставлять стартовые значения, взятые из настроек игры, прямо тут?
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
