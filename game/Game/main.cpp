#include "PerlinNoise.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "raygui.h"
#include "raylib.h"

//int main(void)
//{
//    // Initialization
//    //--------------------------------------------------------------------------------------
//    const int screenWidth = 800;
//    const int screenHeight = 600;
//
//    const int cornerX = 200;
//    const int cornerY = 200;
//
//    const int pixelsPerCell = 32;
//    const int height = 10;
//    const int width = 10;
//
//    int cellByX, cellByY;
//
//    bool obstacles[height][width] = {};
//
//    bool buildMode = false;
//    Vector2 corner = { cornerX, cornerY };
//    Vector2 rectangleSize = { height * pixelsPerCell, width * pixelsPerCell };
//
//    InitWindow(screenWidth, screenHeight, "raylib [core] simple map");
//
//    /*Camera2D camera = { 0 };
//    camera.target = { screenWidth / 2, screenHeight / 2 };
//    camera.offset = { screenWidth / 2, screenHeight / 2 };
//    camera.rotation = 0.0f;
//    camera.zoom = 1.0f;*/
//    
//    //пределы перемещения камеры: не более ~50 пикселей от камеры до края игрового поля
//    
//    //функция, выдающая "прямоугольник" с координатами углов клетки, в ответ на произвольные координаты юнита/постройки
//
//    Vector2 cursor = GetMousePosition();
//
//
//    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
//    //--------------------------------------------------------------------------------------
//
//    // Main game loop
//    while (!WindowShouldClose())        // Detect window close button or ESC key
//    {
//        BeginDrawing();
//        
//        // Background
//        ClearBackground(DARKGRAY);
//        DrawRectangleV(corner, rectangleSize, RAYWHITE);
//        for (int i = 1; i < height; i++)
//        {
//            DrawLine(cornerX, cornerY + 32 * i, cornerX + rectangleSize.x, cornerY + 32 * i, GREEN);
//        }
//
//        for (int j = 1; j < width; j++)
//        {
//            DrawLine(cornerX+pixelsPerCell*j, cornerY, cornerX + pixelsPerCell * j, cornerY+rectangleSize.y, GREEN);
//        }
//
//        //drawing non-empty cells
//        for (int i = 0; i < height; i++)
//        {
//            for (int j = 0; j < width; j++)
//            {
//                if (obstacles[i][j])
//                {
//                    DrawRectangle(i * pixelsPerCell + cornerX, j * pixelsPerCell + corner.y, 32, 32, BLACK);
//                }
//            }
//        }
//
//        if (IsKeyPressed(KEY_E))
//        {
//            if (buildMode) buildMode = false;
//            else buildMode = true;
//        }
//
//        if (buildMode)
//        {
//            // drawing cursor
//            cursor = GetMousePosition();
//            if (cursor.x > cornerX && cursor.x< cornerX + rectangleSize.x && cursor.y> cornerY && cursor.y < cornerY + rectangleSize.y)
//            {
//                cellByX = (cursor.x - cornerX) / pixelsPerCell;
//                cellByY = (cursor.y - cornerY) / pixelsPerCell;
//
//                if (obstacles[cellByX][cellByY])
//                {
//                    //if build can't be placed
//                    DrawRectangle(cellByX * pixelsPerCell + cornerX, cellByY * pixelsPerCell + corner.y, 32, 32, RED);
//                }
//                else
//                {
//                    //if there's no obstacle
//                    DrawRectangle(cellByX * pixelsPerCell + cornerX, cellByY * pixelsPerCell + corner.y, 32, 32, GREEN);
//                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
//                    {
//                        obstacles[cellByX][cellByY] = true;
//                        buildMode = false;
//                    }
//                }
//            }
//            DrawText("BUILD MODE ON", 540, 10, 20, RED);
//        }
//        else
//        {
//            DrawText("BUILD MODE OFF", 540, 10, 20, RED);
//        }
//        
//        /*
//        DrawRectangle(cursor.x, cursor.y, 32, 32, BLUE);*/
//
//        EndDrawing();
//    }
//
//    // De-Initialization
//    //--------------------------------------------------------------------------------------
//    CloseWindow();        // Close window and OpenGL context
//    //--------------------------------------------------------------------------------------
//
//    return 0;
//}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib [core] map generation");

    SetTargetFPS(60);

    const int cornerX = 200;
    const int cornerY = 200;

    const int pixelsPerCell = 32;
    const int height = 512;
    const int width = 512;

    int cellByX, cellByY;

    bool obstacles[height][width] = {};

    bool buildMode = false;
  /*  Vector2 corner = { cornerX, cornerY };
    Vector2 rectangleSize = { height * pixelsPerCell, width * pixelsPerCell };*/

    unsigned char* pixels = new unsigned char[height * width];

    double frequency = 8;
    int octaves = 8;

    const siv::PerlinNoise perlin(12345);
    const double fx = width / frequency;
    const double fy = height / frequency;
    
    int index = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            index = width * y + x;
            pixels[index] = static_cast<unsigned char>(perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves) *255.);
        }
    }

    Image preview = {
        pixels,
        width,
        height,
        UNCOMPRESSED_GRAYSCALE,
        1
    };
    //UNCOMPRESSED_R8G8B8
    Texture2D texture = LoadTextureFromImage(preview);


    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        BeginDrawing();

        // Background
        ClearBackground(DARKGRAY);
        
        DrawTexture(texture, 0, 0, WHITE);

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


//
//# pragma pack (push, 1)
//struct BMPHeader
//{
//	std::uint16_t bfType;
//	std::uint32_t bfSize;
//	std::uint16_t bfReserved1;
//	std::uint16_t bfReserved2;
//	std::uint32_t bfOffBits;
//	std::uint32_t biSize;
//	std::int32_t  biWidth;
//	std::int32_t  biHeight;
//	std::uint16_t biPlanes;
//	std::uint16_t biBitCount;
//	std::uint32_t biCompression;
//	std::uint32_t biSizeImage;
//	std::int32_t  biXPelsPerMeter;
//	std::int32_t  biYPelsPerMeter;
//	std::uint32_t biClrUsed;
//	std::uint32_t biClrImportant;
//};
//
//static_assert(sizeof(BMPHeader) == 54);
//# pragma pack (pop)
//
//struct RGB
//{
//	double r = 0.0;
//	double g = 0.0;
//	double b = 0.0;
//	constexpr RGB() = default;
//	explicit constexpr RGB(double _rgb) noexcept
//		: r(_rgb), g(_rgb), b(_rgb) {}
//	constexpr RGB(double _r, double _g, double _b) noexcept
//		: r(_r), g(_g), b(_b) {}
//};
//
//class Image
//{
//private:
//
//	std::vector<RGB> m_data;
//
//	std::int32_t m_width = 0, m_height = 0;
//
//	bool inBounds(std::int32_t y, std::int32_t x) const noexcept
//	{
//		return (0 <= y) && (y < m_height) && (0 <= x) && (x < m_width);
//	}
//
//	static constexpr std::uint8_t ToUint8(double x) noexcept
//	{
//		return x >= 1.0 ? 255 : x <= 0.0 ? 0 : static_cast<std::uint8_t>(x * 255.0 + 0.5);
//	}
//
//public:
//	
//	Image() = default;
//
//	Image(std::size_t width, std::size_t height)
//		: m_data(width* height)
//		, m_width(static_cast<std::int32_t>(width))
//		, m_height(static_cast<std::int32_t>(height)) {}
//
//	void set(std::int32_t x, std::int32_t y, const RGB& color)
//	{
//		if (!inBounds(y, x))
//		{
//			return;
//		}
//
//		m_data[static_cast<std::size_t>(y) * m_width + x] = color;
//	}
//
//	std::int32_t width() const
//	{
//		return m_width;
//	}
//
//	std::int32_t height() const
//	{
//		return m_height;
//	}
//
//	bool saveBMP(const std::string& path)
//	{
//		const std::int32_t  rowSize = m_width * 3 + m_width % 4;
//		const std::uint32_t bmpsize = rowSize * m_height;
//		const BMPHeader header =
//		{
//			0x4d42,
//			static_cast<std::uint32_t>(bmpsize + sizeof(BMPHeader)),
//			0,
//			0,
//			sizeof(BMPHeader),
//			40,
//			m_width,
//			m_height,
//			1,
//			24,
//			0,
//			bmpsize,
//			0,
//			0,
//			0,
//			0
//		};
//
//		if (std::ofstream ofs{ path, std::ios_base::binary })
//		{
//			ofs.write(reinterpret_cast<const char*>(&header), sizeof(header));
//
//			std::vector<std::uint8_t> line(rowSize);
//
//			for (std::int32_t y = m_height - 1; -1 < y; --y)
//			{
//				size_t pos = 0;
//
//				for (std::int32_t x = 0; x < m_width; ++x)
//				{
//					const RGB& col = m_data[static_cast<std::size_t>(y) * m_width + x];
//					line[pos++] = ToUint8(col.b);
//					line[pos++] = ToUint8(col.g);
//					line[pos++] = ToUint8(col.r);
//				}
//
//				ofs.write(reinterpret_cast<const char*>(line.data()), line.size());
//			}
//
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//};
//
//int main()
//{
//
//	Image image(512, 512);
//
//	std::cout << "---------------------------------\n";
//	std::cout << "* frequency [0.1 .. 8.0 .. 64.0] \n";
//	std::cout << "* octaves   [1 .. 8 .. 16]       \n";
//	std::cout << "* seed      [0 .. 2^32-1]        \n";
//	std::cout << "---------------------------------\n";
//
//	while()
//	{
//		double frequency;
//		std::cout << "double frequency = ";
//		std::cin >> frequency;
//		frequency = std::clamp(frequency, 0.1, 64.0);
//
//		std::int32_t octaves;
//		std::cout << "int32 octaves    = ";
//		std::cin >> octaves;
//		octaves = std::clamp(octaves, 1, 16);
//
//		std::uint32_t seed;
//		std::cout << "uint32 seed      = ";
//		std::cin >> seed;
//
//		const siv::PerlinNoise perlin(seed);
//		const double fx = image.width() / frequency;
//		const double fy = image.height() / frequency;
//
//		for (std::int32_t y = 0; y < image.height(); ++y)
//		{
//			for (std::int32_t x = 0; x < image.width(); ++x)
//			{
//				const RGB color(perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves));
//				image.set(x, y, color);
//			}
//		}
//
//		std::stringstream ss;
//		ss << 'f' << frequency << 'o' << octaves << '_' << seed << ".bmp";
//
//		if (image.saveBMP(ss.str()))
//		{
//			std::cout << "...saved \"" << ss.str() << "\"\n";
//		}
//		else
//		{
//			std::cout << "...failed\n";
//		}
//		
//		char c;
//		std::cout << "continue? [y/n] >";
//		std::cin >> c;
//		if (c != 'y') break;
//		std::cout << '\n';
//	}
//}

