#pragma once
#include "Window.h"
#include "Graphics.h"
#include "Input.h"
#include "DrawSystem.h"

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
    Window wnd;
    Graphics gfx;
    Input input;
    DrawSystem draw;
    bool running = true;
};