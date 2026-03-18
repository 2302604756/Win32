#pragma once
#include "Graphics.h"
#include "Line.h"

class Rasterizer
{
public:
    static void DrawLine(Graphics& gfx, const Line& line, unsigned int color);
};
