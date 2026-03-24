#pragma once
#include <vector>
#include <d3d11.h>
#include <wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

enum class DrawMode
{
    Line,
    Polygon

};



class Graphics
{
public:
    Graphics(HWND hwnd, int width, int height);

    void Clear(float r, float g, float b);
    void PutPixel(int x, int y, unsigned int color);
    void Present();
    void Resize(int w, int h);
    void SetRenderTarget();
    void Draw();

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const { return device; }
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() const { return context; }
   

private:
    void InitD3D(HWND hwnd);
    void CreateRenderTarget();
    void CreateTexture();
    void CreateFullscreenQuad();

private:
    int width;
    int height;

    std::vector<unsigned int> buffer;

    // ComPtr 管理
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
};