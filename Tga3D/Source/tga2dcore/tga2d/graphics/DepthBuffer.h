#pragma once

#include <Tga2d/Math/Vector.h>
#include <Tga2d/Graphics/TextureResource.h>
#include <wrl/client.h>
#include <dxgiformat.h>
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct D3D11_VIEWPORT;

namespace Tga2D
{

class DepthBuffer : public TextureResource
{
	ComPtr<ID3D11DepthStencilView> myDepth = 0;
	D3D11_VIEWPORT myViewport = {};

public:

	static DepthBuffer Create(Vector2ui aSize, DXGI_FORMAT aFormat = DXGI_FORMAT_D32_FLOAT);

	void Clear(float aClearDepthValue = 1.0f, uint8_t aClearStencilValue = 0);
	ID3D11DepthStencilView* GetDepthStencilView() { return myDepth.Get(); };
};

} // namespace Tga2D