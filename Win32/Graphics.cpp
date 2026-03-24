#include "Graphics.h"
#include <D3Dcompiler.h>
#include <stdexcept>
#include <iostream>


Graphics::Graphics(HWND hwnd, int width, int height)
    : width(width), height(height)
{
    buffer.resize(width * height);

    //创建swapchain和device
    InitD3D(hwnd);
    //创建RenderTarget
    CreateRenderTarget();
    CreateTexture();
    //我们创建两个三角形覆盖了屏幕，在用原来的逻辑修改buffer
    CreateFullscreenQuad();
}

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

void Graphics::CreateTexture()
{
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
    device->CreateShaderResourceView(texture.Get(), nullptr, srv.GetAddressOf());

    // sampler
    D3D11_SAMPLER_DESC samp = {};
    samp.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samp.AddressU = samp.AddressV = samp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    device->CreateSamplerState(&samp, sampler.GetAddressOf());
}

struct Vertex
{
    float x, y;
    float u, v;
};

void Graphics::CreateFullscreenQuad()
{
    Vertex verts[] =
    {
        { -1, -1, 0, 1 },
        { -1,  1, 0, 0 },
        {  1, -1, 1, 1 },
        {  1,  1, 1, 0 },
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(verts);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = verts;

    device->CreateBuffer(&bd, &data, vertexBuffer.GetAddressOf());

    //  Shader（最小版本）
    const char* vsSrc =
        "struct VSIn { float2 pos : POSITION; float2 uv : TEXCOORD; };"
        "struct PSIn { float4 pos : SV_POSITION; float2 uv : TEXCOORD; };"
        "PSIn main(VSIn input){"
        "PSIn o;"
        "o.pos = float4(input.pos,0,1);"
        "o.uv = input.uv;"
        "return o;}";

    const char* psSrc =
        "Texture2D tex0 : register(t0);"
        "SamplerState samp0 : register(s0);"
        "float4 main(float4 pos:SV_POSITION,float2 uv:TEXCOORD):SV_TARGET{"
        "return tex0.Sample(samp0,uv);}";

    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, psBlob;

    D3DCompile(vsSrc, strlen(vsSrc), nullptr, nullptr, nullptr,
        "main", "vs_4_0", 0, 0, vsBlob.GetAddressOf(), nullptr);

    D3DCompile(psSrc, strlen(psSrc), nullptr, nullptr, nullptr,
        "main", "ps_4_0", 0, 0, psBlob.GetAddressOf(), nullptr);

    device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        vs.GetAddressOf());

    device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        ps.GetAddressOf());

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,8,D3D11_INPUT_PER_VERTEX_DATA,0},
    };

    device->CreateInputLayout(
        layout, 2,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        inputLayout.GetAddressOf());
}

void Graphics::Present()
{
    D3D11_MAPPED_SUBRESOURCE ms;
    context->Map(texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);

    //memcpy(ms.pData, buffer.data(), width * height * 4);

    for (int y = 0; y < height; y++)
    {
        memcpy(
            (BYTE*)ms.pData + y * ms.RowPitch,
            buffer.data() + y * width,
            width * 4
        );
    }

    context->Unmap(texture.Get(), 0);

    float clear[4] = { 0,0,0,1 };
    context->ClearRenderTargetView(rtv.Get(), clear);

    UINT stride = sizeof(float) * 4;
    UINT offset = 0;

    context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    context->IASetInputLayout(inputLayout.Get());

    context->VSSetShader(vs.Get(), nullptr, 0);
    context->PSSetShader(ps.Get(), nullptr, 0);

    context->PSSetShaderResources(0, 1, srv.GetAddressOf());
    context->PSSetSamplers(0, 1, sampler.GetAddressOf());

    context->Draw(4, 0);

    swapChain->Present(1, 0);
}

void Graphics::Clear(float r, float g, float b)
{
    
    float color[4] = { 0,1,1,1 };
    context->ClearRenderTargetView(rtv.Get(), color);

}

void Graphics::Resize(int w, int h)
{
    if (swapChain) {
        width = w;
        height = h;

        buffer.assign(w * h, 0); // 🔥 必须！

        rtv.Reset();
        swapChain->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);

        CreateRenderTarget();
        CreateTexture();
    }
}

void Graphics::PutPixel(int x, int y, unsigned int color)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    buffer[y * width  + x ] = color;
    
    
}

