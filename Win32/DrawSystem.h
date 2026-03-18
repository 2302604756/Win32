#pragma once
#include <vector>
#include "Line.h"
#include "Graphics.h"

class DrawSystem
{
public:
    void AddPoint(int x, int y);
    void Draw(Graphics& gfx);

private:
    std::vector<Point> points;
};
