#include "TerrainGenerator.h"
#include <omp.h>

//��������� � �������� ����� ����������� (�� �������!)

TerrainGenerator::TerrainGenerator()
{
	closed = false;

	//TODO: make preset maps (empty map, for path finding etc.)
	width = 512, height = 512, seed = 12345, octaves = 16;
	frequency = 16.;

	//TODO: replace memory allocation to regenerate/rerender
	terrainMap = new TerrainType[width * height];
								//	LAKE,	 SWAMP,	SAND,	PLAIN,	TREE,		STONE,	MOUNTAIN
	levels = new float[] { 0.,		0.35,	  0.4,	0.45,	0.5,		0.6,	0.70,		1. };
	palette = new Color[]{	DARKBLUE, BLUE,	YELLOW, GREEN,	DARKGREEN,	GRAY,	BLACK};
	levelNames = new const char*[] { "LAKE", "SWAMP",	"SAND",	"PLAIN", "TREE", "STONE", "MOUNTAIN" };

	noiseMap = new double[width * height];
}

void TerrainGenerator::DrawInterface()
{
	closed = GuiButton(Rectangle{ 20.f, 20.f, 100.f, 50.f }, "Back") || IsKeyPressed(KEY_BACKSPACE);

	if (generated)
	{
		//TODO: drawing according to screen size (not to exceed borders)
		DrawTexture(colorPreview, 50, 100, WHITE);
		DrawTexture(grayPreview, colorPreview.width+50, 100, WHITE);
	}
	else
	{
		RegenerateTerrain();
		RerenderTerrain();
	}

	//REPLACE IN CONSTRUCTOR PARAMETERS(??)
	Vector2 guiZeroPoint = { colorPreview.width * 2+150, 20 };

	//drawing levels sliders and retriving values from them
	Rectangle position;
	std::string buf;
	for (int i = 1; i < 7; i++) // 7 - `cause 8 terrain levels, 8-1
	{
		buf = levelNames[i] + std::string(": %.2f");
		position = { guiZeroPoint.x, guiZeroPoint.y + 35.f *i, 120, 20 };
		levels[i] = GuiSliderBar(
						position,
						TextFormat(buf.c_str(), levels[i-1]),
						TextFormat("%.2f", levels[i+1]),
						levels[i],
						levels[i - 1],
						levels[i+1]
						);
	}

	//level distribution visualisation
	//TODO: solve problem with mountain visual
	for (int i = 0; i < 8; i++)
	{
		position = { levels[i] * 240.f + guiZeroPoint.x-60, guiZeroPoint.y + 250, (levels[i + 1] - levels[i]) * 300, 30.f };

		DrawText(TextFormat("%.1f", levels[i]), position.x, position.y - 15, 14, palette[i]);
		DrawRectangleRec(position, palette[i]);
	}

	//numerable params
	GuiValueBox(Rectangle{ guiZeroPoint.x, guiZeroPoint.y + 390, 60, 30 }, "Generation seed:", &seed, 0, INT_MAX, true);
	octaves = GuiSliderBar(
				Rectangle{ guiZeroPoint.x, guiZeroPoint.y+ 430, 120, 20},
				"Octaves: 1",
				"16",
				octaves,
				1,
				16
				);

	frequency = GuiSliderBar(
		Rectangle{ guiZeroPoint.x, guiZeroPoint.y + 460, 120, 20 },
		"Frequency: 1.0",
		"64.0",
		frequency,
		0.1f,
		64.f
	);

	//TODO: add valueBoxes for width and height

	//Rerender button
	if (GuiButton(Rectangle{ guiZeroPoint.x, guiZeroPoint.y + 300, 150.f, 50.f }, "Rerender"))
		RerenderTerrain();

	//Regenerate button
	if (GuiButton(Rectangle{ guiZeroPoint.x, guiZeroPoint.y+ 500, 150.f, 50.f }, "Regenerate"))
	{
		RegenerateTerrain();
		RerenderTerrain();
	}

	//TODO: add random seed button (with "refresh" icon)

	//TODO: Add regenerate animation(??)

	//TODO: ADD HISTOGRAM BELOW MAP PREVIEWS
}

void TerrainGenerator::RegenerateTerrain()
{
	int index;
	const siv::PerlinNoise perlin((unsigned int)seed);
	
	//TODO: regenerate according to chosen terrain size (with pointers)

	fx = width / frequency;
	fy = height / frequency;

#pragma omp parallel for private(index)
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			index = width * y + x;
			noiseMap[index] = perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves);
		}
	}
}

void TerrainGenerator::RerenderTerrain()
{
	//updating color preview
	Color* colorPixels = new Color[width * height];
	unsigned char* grayPixels = new unsigned char[width * height];

#pragma omp parallel for
	for (int i = 0; i < width * height; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			grayPixels[i] = (unsigned char)(noiseMap[i] * 255.);
			if (noiseMap[i] <= levels[j+1])
			{
				colorPixels[i] = palette[j];
				terrainMap[i] = static_cast<TerrainType>(j); 
				break;
			}
		}
	}

	Image colorImage = {
		colorPixels,
		width,
		height,
		1,
		UNCOMPRESSED_R8G8B8A8
	};
	
	Image grayImage = {
		grayPixels,
		width,
		height,
		1,
		UNCOMPRESSED_GRAYSCALE
	};

	if (!generated)
	{
		colorPreview = LoadTextureFromImage(colorImage);
		grayPreview = LoadTextureFromImage(grayImage);
	}
	else
	{
		UpdateTexture(colorPreview, colorPixels);
		UpdateTexture(grayPreview, grayPixels);
	}

	generated = true;
	delete[] colorPixels;
	delete[] grayPixels;
}

TerrainGenerator::~TerrainGenerator()
{
	delete[] levels;
	delete[] noiseMap;
	delete[] palette;
}

Terrain TerrainGenerator::getMap()
{
	return Terrain{width, height, terrainMap};
}