#pragma once
#include <vector>
#include "Line.h"

struct Polygon
{
    std::vector<Point> points;
    bool closed = false;
};