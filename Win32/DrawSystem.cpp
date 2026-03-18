#include "DrawSystem.h"
#include "Rasterizer.h"

void DrawSystem::AddPoint(int x, int y)
{
    points.push_back({ x, y });
}

void DrawSystem::Draw(Graphics& gfx)
{
    for (size_t i = 1; i < points.size(); i++)
    {
        Line l{ points[i - 1], points[i] };
        Rasterizer::DrawLine(gfx, l, 0xFFFFFF);
    }
}