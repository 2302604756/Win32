#pragma once
#include <vector>
#include "Line.h"
#include "GeometryPass.h"

class DrawSystem
{
public:
    void SetMode(DrawMode m);

    void OnClick(int x, int y);   // 鼠标点击
    void EndCurrent();            // 结束当前图形

    void Draw(GeometryPass& gpfx);

    DrawMode GetMode() const { return mode; }

    
private:
    DrawMode mode = DrawMode::Line;

    // 已完成图元
    std::vector<Line> lines;
    std::vector<Polygon1> polygons;
    std::vector<Line> continusLines;

    // 当前正在绘制
    bool hasFirstPoint = false;
    Point tempPoint;
    bool dragging = false;
    Point start;
    Line previewLine;   
    Polygon1 currentPoly;
};
