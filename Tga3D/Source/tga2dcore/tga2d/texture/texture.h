/*
This class will store a texture bound to DX11
*/
#pragma once

#include <tga2d/graphics/TextureResource.h>

using Microsoft::WRL::ComPtr;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
namespace Tga2D
{
	class Texture : public TextureResource
	{
	public:
		Texture();
		~Texture();

	public:
		std::wstring myPath;
		uint64_t myID;
		Vector2f mySize;
		Vector2ui myImageSize;
		bool myIsFailedTexture;
		bool myIsReleased;
	};
}
