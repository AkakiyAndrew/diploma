#pragma once

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
	//œŒƒ”Ã¿“‹ œ–Œ —»—“≈Ã” ¿“¿ » » —÷≈œ»“‹ — ¿Õ»Ã¿÷»≈…
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
	LAKE,
	SWAMP,
	SAND,
	PLAIN,
	TREE,
	STONE,
	MOUNTAIN,
	ASH,
};

enum class GameState
{
	MAIN_MENU,
	TERRAIN_GENERATOR,
	GAMEPLAY,
	OPTIONS //??
};

struct Terrain
{
	int width;
	int height;
	TerrainType* map;
};