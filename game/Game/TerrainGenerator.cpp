#include "TerrainGenerator.h"

//»—œ–¿¬»“‹ » ƒŒ¡¿¬»“‹ ÕŒ¬€≈  ŒÃÃ≈Õ“¿–»» (Ì‡ ËÌ„ÎË¯Â!)

TerrainGenerator::TerrainGenerator()
{
	width = 512, height = 512, seed = 12345, octaves = 16;
	frequency = 16.;
								//	LAKE,	 SWAMP,	SAND,	PLAIN,	TREE,		STONE,	MOUNTAIN
	levels = new float[] { 0.,		0.35,	  0.4,	0.45,	0.5,		0.6,	0.70,		1. };
	palette = new Color[]{	DARKBLUE, BLUE,	YELLOW, GREEN,	DARKGREEN,	GRAY,	BLACK};
	levelNames = new const char*[] { "LAKE", "SWAMP",	"SAND",	"PLAIN", "TREE", "STONE", "MOUNTAIN" };

	noiseMap = new double[width * height];
}

void TerrainGenerator::DrawInterface()
{
	if (generated)
	{
		
		DrawTexture(colorPreview, 20, 20, WHITE);
		DrawTexture(grayPreview, colorPreview.width+50, 20, WHITE);
	}
	else
	{
		RegenerateTerrain();
		RerenderTerrain();
	}

	//GuiLoadStyleDefault();

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

	//Rerender button
	if (GuiButton(Rectangle{ guiZeroPoint.x, guiZeroPoint.y + 300, 150.f, 50.f }, "Rerender"))
		RerenderTerrain();

	//Regenerate button
	if (GuiButton(Rectangle{ guiZeroPoint.x, guiZeroPoint.y+ 500, 150.f, 50.f }, "Regenerate"))
	{
		RegenerateTerrain();
		RerenderTerrain();
	}

	//TODO: ADD HISTOGRAM BELOW MAP PREVIEWS
}

void TerrainGenerator::RegenerateTerrain()
{
	int index;
	const siv::PerlinNoise perlin((unsigned int)seed);
	
	fx = width / frequency;
	fy = height / frequency;

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

	for (int i = 0; i < width * height; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			unsigned char buf = (unsigned char)(noiseMap[i] * 255.);
			grayPixels[i] = buf;
			if (noiseMap[i] <= levels[j+1])
			{
				colorPixels[i] = palette[j];
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