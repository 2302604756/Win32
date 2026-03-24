#pragma once
#include "GeometryPass.h"
#include "Line.h"

class Rasterizer
{
public:
    static void DrawLine(GeometryPass& gpfx, const Line& line, unsigned int color);
    static void FillPolygon(GeometryPass& gpfx, const Polygon1& poly, unsigned int color);
};

