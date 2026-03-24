#pragma once


#include <d3d11.h>
#include <wrl/client.h>

class ImGuiManager
{
public:
    ImGuiManager() = default;
    ~ImGuiManager() = default;

    void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
    void Shutdown();

    void BeginFrame();
    void EndFrame();

private:
    bool initialized = false;
};