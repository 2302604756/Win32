#include "App.h"

App::App()
    : wnd(*this, 800, 600, L"My Engine"),
    gfx(wnd.GetHWND(), wnd.GetWidth(), wnd.GetHeight())
{
}

bool App::IsRunning() const
{
    return running;
}

void App::ProcessMessages()
{
    if (!wnd.ProcessMessages())
        running = false;
}

void App::Update()
{
    if (input.KeyIsPressed(VK_LBUTTON))
    {
        draw.AddPoint(input.GetMouseX(), input.GetMouseY());
    }
}

void App::Render()
{
    gfx.Clear(0.0f, 0.0f, 0.0f);
    draw.Draw(gfx);
    gfx.Present();
}

void App::OnResize(int w, int h)
{
    gfx.Resize(w, h);
}