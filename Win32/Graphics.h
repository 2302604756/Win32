#pragma once
#include <vector>
#include <windows.h>

class Graphics
{
public:
    Graphics(HWND hwnd, int width, int height);

    void Clear(float r, float g, float b);
    void PutPixel(int x, int y, unsigned int color);
    void Present();

    int GetWidth() const;
    int GetHeight() const;

    void Resize(int width, int height);
private:
    HWND hwnd;
    int width;
    int height;

    std::vector<unsigned int> buffer; // CPU framebuffer
};