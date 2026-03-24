#include "Rasterizer.h"
#include <cmath>

void Rasterizer::DrawLine(Graphics& gfx, const Line& line, unsigned int color)
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
        gfx.PutPixel(x0, y0, color);

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

