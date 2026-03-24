#pragma once
#include "Graphics.h"

class GeometryPass {
public:
    GeometryPass(Graphics& gfx);
	void Bind();
	void Draw();
    void PutPixel(int x, int y, unsigned int color);
    void CreateTexture();
    void CreateFullscreenQuad();
    void Resize(Graphics& gfx);
    
private:

    int width;
    int height;
    //CPU上纹理
    std::vector<unsigned int> buffer;

    //DX11资源
    ID3D11Device* device;
    ID3D11DeviceContext* context;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
};
