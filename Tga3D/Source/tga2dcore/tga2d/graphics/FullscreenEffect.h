#pragma once
#include <array>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

struct ID3D11VertexShader;
struct ID3D11PixelShader;

namespace Tga2D
{

class FullscreenEffect
{
public:
	FullscreenEffect();
	bool Init(const char* aPixelShaderPath);
	void Render();
private:
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
};

} // namespace Tga2D