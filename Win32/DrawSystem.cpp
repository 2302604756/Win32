#include "DrawSystem.h"
#include "Rasterizer.h"

#include <iostream>

void DrawSystem::Draw(GeometryPass& gpfx)
{
    
    // ===== 画所有线 =====
    for (auto& l : lines)
    {
        Rasterizer::DrawLine(gpfx, l, 0xFFFFFF);
    }

    // ===== 画所有多边形 =====
    for (auto& poly : polygons)
    {
        //先填充
        Rasterizer::FillPolygon(gpfx, poly, 0x003300); // 深绿色填充

        //再画边
        for (size_t i = 1; i < poly.points.size(); i++)
        {
            Rasterizer::DrawLine(gpfx,
                { poly.points[i - 1], poly.points[i] },
                0x00FF00);
        }

        if (poly.closed)
        {
            Rasterizer::DrawLine(gpfx,
                { poly.points.back(), poly.points.front() },
                0x00FFFF);
        }
    }

    // ===== 当前正在画的多边形（预览）=====
    for (size_t i = 1; i < currentPoly.points.size(); i++)
    {
        Rasterizer::DrawLine(gpfx,
            { currentPoly.points[i - 1], currentPoly.points[i] },
            0xFF0000);
    }

    // ===== 画预览线 =====
    if (dragging)
    {
        Rasterizer::DrawLine(gpfx, previewLine, 0xFF0000);
    }
}
//模式切换
void DrawSystem::SetMode(DrawMode m)
{
    if (mode == m) return;

    mode = m;

    hasFirstPoint = false;
    currentPoly.points.clear();
}
//鼠标点击逻辑
void DrawSystem::OnClick(int x, int y)
{
    if (mode == DrawMode::Line)
    {
        if (!hasFirstPoint)
        {
            tempPoint = { x, y };
            hasFirstPoint = true;
        }
        else
        {
            lines.push_back({ tempPoint, { x, y } });
            hasFirstPoint = false;
        }
    }
    else if (mode == DrawMode::Polygon)
    {
        currentPoly.points.push_back({ x, y });
    }
}

//结束图元
void DrawSystem::EndCurrent()
{
    if (mode == DrawMode::Polygon)
    {
        if (currentPoly.points.size() >= 3)
        {
            currentPoly.closed = true;
            polygons.push_back(currentPoly);
        }

        currentPoly.points.clear();
    }
}



