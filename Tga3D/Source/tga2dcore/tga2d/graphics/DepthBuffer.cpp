#include "stdafx.h"
#include "DepthBuffer.h"

#include <tga2d/graphics/DX11.h>

using namespace Tga2D;

void DepthBuffer::Clear(float aClearDepthValue /* = 1.0f */, uint8_t aClearStencilValue /* = 0 */)
{
	DX11::Context->ClearDepthStencilView(myDepth.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, aClearDepthValue, aClearStencilValue);
}

DepthBuffer DepthBuffer::Create(Vector2ui aSize, DXGI_FORMAT aFormat)
{
	/*
	 * It's possible to create this as R32_TYPELESS and then create the DSV with
	 * a specified type of D32_FLOAT in the desc. It's also then possible to create
	 * a SRV specifying the type as R32_FLOAT to be able to read the depth.
	 */

	HRESULT result;
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = aSize.X;
	desc.Height = aSize.Y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = aFormat;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Texture2D* texture;
	result = DX11::Device->CreateTexture2D(&desc, nullptr, &texture);
	assert(SUCCEEDED(result));

	ID3D11DepthStencilView* DSV;
	result = DX11::Device->CreateDepthStencilView(texture, nullptr, &DSV);
	assert(SUCCEEDED(result));

	DepthBuffer textureResult;
	textureResult.myDepth = DSV;
	DSV->Release();
	textureResult.myViewport = {
			0,
			0,
			static_cast<float>(aSize.X),
			static_cast<float>(aSize.Y),
			0,
			1
	};
	return textureResult;
}
