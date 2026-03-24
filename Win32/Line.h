#pragma once

struct Point
{
    int x;
    int y;
};

struct Line
{
    Point p0;
    Point p1;
};

struct Polygon1
{
    std::vector<Point> points;
    bool closed = false;
};