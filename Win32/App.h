#pragma once
#include "Window.h"
#include "Graphics.h"
#include "Input.h"
#include "DrawSystem.h"

#include "ImGuiManager.h"


class App
{
public:
    App();
    bool IsRunning() const;

    void ProcessMessages();
    void Update();
    void Render();

    // 给 Window 调用
    void OnResize(int w, int h);

public:
    //pass
    
    Window wnd;
    Graphics gfx;
    GeometryPass gpfx;
    Input input;
    DrawSystem draw;
    bool running = true;
    //用于控制绘制什么
    DrawMode mode = DrawMode::Line;
    ImGuiManager imgui;
};