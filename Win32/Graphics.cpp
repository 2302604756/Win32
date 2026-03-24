/*
    应该把graphics作为基础组件管理类，仅负责管理device context swapchain 以及rtv
    对外留有获取这些资源的接口以及刷新函数来方便我们创建绑定资源和交换屏幕渲染
*/


#include "Graphics.h"
#include <D3Dcompiler.h>
#include <stdexcept>
#include <iostream>


Graphics::Graphics(HWND hwnd, int width, int height)
    : width(width), height(height)
{
   
    //创建swapchain和device
    InitD3D(hwnd);
    //创建RenderTarget
    CreateRenderTarget();

    //我们创建两个三角形覆盖了屏幕，在用原来的逻辑修改buffer

}

//生成device和swapchain
void Graphics::InitD3D(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        swapChain.GetAddressOf(),
        device.GetAddressOf(),
        nullptr,
        context.GetAddressOf()
    );
    std::cout << hr << std::endl;
    if (FAILED(hr)) {
        std::cout << hr << std::endl;
        throw std::runtime_error("CreateSwapChain failed");
    }
}

//创建RenderTarget
void Graphics::CreateRenderTarget()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        (void**)backBuffer.GetAddressOf());

    device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtv.GetAddressOf());

    context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp = {};
    vp.Width = (float)width;
    vp.Height = (float)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    context->RSSetViewports(1, &vp);
}

void Graphics::Present() {
    swapChain->Present(1, 0);
}

void Graphics::Clear(float r, float g, float b)
{
    context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr); // 🔥补这一句
    float color[4] = { r,g,b,1 };
    context->ClearRenderTargetView(rtv.Get(), color);
}


void Graphics::Resize(int w, int h)
{
    if (swapChain) {
        width = w;
        height = h;

       

        rtv.Reset();
        swapChain->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);

        CreateRenderTarget();
    }
}

void Graphics::SetRenderTarget()
{
    context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
}





