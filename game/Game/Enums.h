#pragma once

#include "raylib.h"
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <omp.h>
#include <math.h>

enum class Side
{
	MACHINES,
	INSECTS
};

enum class State
{
	ONLINE,
	OFFLINE,
	UNDER_CONSTRUCTION,
	ATTACKING,
	GOES,
	IDLE, //remove?
	CHANGING_MODE, //for turrets mount/dismount
	
				   
	//œŒƒ”Ã¿“‹ œ–Œ —»—“≈Ã” ¿“¿ » » —÷≈œ»“‹ — ¿Õ»Ã¿÷»≈…
};

enum class ActorType
{
	//for wantToBuild
	ACTOR_NULL,

	//for Turret animation
	TURRET_CHASIS,

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
	bool operator==(TileIndex rhs) { return (x == rhs.x) && (y == rhs.y); };
};

struct NeighborsIndex
{
	TileIndex left = {-1,-1};
	TileIndex up = { -1,-1 };
	TileIndex right = { -1,-1 };
	TileIndex down = { -1,-1 };

	TileIndex upLeft = { -1,-1 };
	TileIndex upRight = { -1,-1 };
	TileIndex downLeft = { -1,-1 };
	TileIndex downRight = { -1,-1 };
};

struct Animation
{
	Texture2D* frames;

	int framesAmount;
	int width = 0;
	int height = 0;
};

inline int Vector2Angle(Vector2 v1, Vector2 v2)
{
	int result = atan2f(v2.y - v1.y, v2.x - v1.x) * (180.0f / PI);
	if (result < 0) result += 360.0f;
	return result;
}

inline float Vector2Distance(Vector2 v1, Vector2 v2)
{
	float result = sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
	return result;
}

inline float Vector2Length(Vector2 v)
{
	float result = sqrtf((v.x * v.x) + (v.y * v.y));
	return result;
}

inline int degreeBetweenVectors(Vector2 v1, Vector2 v2)
{
	double angle = atan2(v1.x * v2.x + v1.y * v2.y, v1.x * v2.y - v1.y * v2.x);
	
	return (angle*180.0)/PI;
}

//TODO: make ENUM for attributes for faster geting from std::maps