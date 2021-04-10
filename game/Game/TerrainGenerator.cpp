#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator()
{
	lakeLevel = 0.25;
	mountainLevel = 0.75;
}

void TerrainGenerator::DrawInterface()
{
	lakeLevel = GuiSliderBar(Rectangle{ 600, 40, 120, 20 }, TextFormat("%f", 0.), TextFormat("%f", mountainLevel), lakeLevel, 0., mountainLevel);
	mountainLevel = GuiSliderBar(Rectangle{ 600, 70, 120, 20}, TextFormat("%f", lakeLevel), TextFormat("%f", 1.), mountainLevel, lakeLevel, 1.);
}
