#pragma once

#include "raylib.h"
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <omp.h>

enum class Side
{
	MACHINES,
	INSECTS
};

enum class State
{
	ONLINE,
	OFFLINE,
	CONSTRUCTION,
	BUILDING,
	ATTACKING,
	GOES,
	IDLE,
	//œŒƒ”Ã¿“‹ œ–Œ —»—“≈Ã” ¿“¿ » » —÷≈œ»“‹ — ¿Õ»Ã¿÷»≈…
};

enum class ActorType
{
	//for wantToBuild
	ACTOR_NULL,

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

enum ExpandState
{
	UNAVAILABLE,
	AVAILABLE,
	EXPANDED,
	EXPANDED_WITHOUT_SOURCE,
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

struct TileIndex
{
	int x;
	int y;

	operator Vector2() { return Vector2{ static_cast<float>(x), static_cast<float>(y) }; };
};

//struct NeighborsIndex
//{
//	TileIndex left = {-1,-1};
//	TileIndex up = { -1,-1 };
//	TileIndex right = { -1,-1 };
//	TileIndex down = { -1,-1 };
//};