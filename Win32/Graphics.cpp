#include "Graphics.h"

Graphics::Graphics(HWND hwnd, int width, int height)
    : hwnd(hwnd), width(width), height(height)
{
    buffer.resize(width * height);
}

void Graphics::Clear(float r, float g, float b)
{
    unsigned int color =
        ((int)(r * 255) << 16) |
        ((int)(g * 255) << 8) |
        ((int)(b * 255));

    std::fill(buffer.begin(), buffer.end(), color);
}

void Graphics::PutPixel(int x, int y, unsigned int color)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    buffer[y * width + x] = color;
}

void Graphics::Present()
{
    HDC hdc = GetDC(hwnd);

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    StretchDIBits(
        hdc,
        0, 0, width, height,
        0, 0, width, height,
        buffer.data(),
        &bmi,
        DIB_RGB_COLORS,
        SRCCOPY
    );

    ReleaseDC(hwnd, hdc);
}

void Graphics::Resize(int w, int h)
{
    // ResizeBuffers
}