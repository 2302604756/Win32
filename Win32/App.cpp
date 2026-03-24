#include "App.h"
#include "ImGuiManager.h"
#include <iostream>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "GeometryPass.h"

App::App()
    :wnd(*this, 800, 600, L"My Engine"),
    gfx(wnd.GetHWND(), wnd.GetWidth(), wnd.GetHeight()),
    gpfx(gfx)
{
    imgui.Init(wnd.GetHWND(), gfx.GetDevice().Get(), gfx.GetContext().Get());
}


bool App::IsRunning() const
{
    return running;
}

void App::ProcessMessages() 
{
    if (!wnd.ProcessMessages())
    {
        running = false;
    }
        
}

void App::Update()
{
    if (input.mouseClicked)
    {
        // 🔥 如果 UI 在用鼠标，直接忽略
        if (!ImGui::GetIO().WantCaptureMouse)
        {
            draw.OnClick(input.GetMouseX(), input.GetMouseY());
        }
    }
}

void App::Render()
{
    gfx.Clear(0.5f, 0.0f, 0.0f);

    
 // 🔥 必须
    draw.Draw(gpfx);
    gpfx.Draw();
    // ===== ImGui Begin =====
    imgui.BeginFrame();

    // ===== UI 控制 =====
    ImGui::Begin("Draw Tool");

    if (ImGui::RadioButton("Line", draw.GetMode() == DrawMode::Line))
        draw.SetMode(DrawMode::Line);

    if (ImGui::RadioButton("Polygon", draw.GetMode() == DrawMode::Polygon))
        draw.SetMode(DrawMode::Polygon);

    if (ImGui::Button("End Polygon"))
        draw.EndCurrent();

    ImGui::Text("Mouse: %d, %d", input.GetMouseX(), input.GetMouseY());

    ImGui::End();

    // ===== 绘制 =====
    

    // ===== ImGui Render =====
    imgui.EndFrame();

    gfx.Present();
}

void App::OnResize(int w, int h)
{
    gfx.Resize(w, h);
    gpfx.Resize(gfx);
}