#define NOMINMAX
#include "Rasterizer.h"
#include <cmath>
#include <algorithm>

void Rasterizer::DrawLine(GeometryPass& gpfx, const Line& line, unsigned int color)
{
    int x0 = line.p0.x;
    int y0 = line.p0.y;
    int x1 = line.p1.x;
    int y1 = line.p1.y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int err = dx - dy;

    while (true)
    {
        gpfx.PutPixel(x0, y0, color);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}



void Rasterizer::FillPolygon(GeometryPass& gpfx, const Polygon1& poly, unsigned int color)
{
    if (poly.points.size() < 3) return;

    // 找 y 范围
    int minY = poly.points[0].y;
    int maxY = poly.points[0].y;

    for (auto& p : poly.points)
    {
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
    }

    // 扫描每一行
    for (int y = minY; y <= maxY; y++)
    {
        std::vector<int> nodes;

        // 找交点
        size_t count = poly.points.size();
        for (size_t i = 0; i < count; i++)
        {
            Point p1 = poly.points[i];
            Point p2 = poly.points[(i + 1) % count];

            // 判断是否跨越扫描线
            if ((p1.y < y && p2.y >= y) ||
                (p2.y < y && p1.y >= y))
            {
                int x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                nodes.push_back(x);
            }
        }

        // 排序
        std::sort(nodes.begin(), nodes.end());

        // 两两填充
        for (size_t i = 0; i + 1 < nodes.size(); i += 2)
        {
            for (int x = nodes[i]; x < nodes[i + 1]; x++)
            {
                gpfx.PutPixel(x, y, color);
            }
        }
    }
}