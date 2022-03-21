/*
This class handles all the textures created, please use this when you create a texture
*/

#pragma once
#include "Texture.h"
#include <dxgiformat.h>
#include <vector>
#include <tga2d/loaders/tgaloader.h>

struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;
namespace Tga2D
{
	class GraphicsEngine;
	class TextureManager
	{
	public:
		TextureManager(void);
		~TextureManager(void);
		void Init();
		Texture* GetTexture(const wchar_t* aTexturePath, bool aForceSRGB = true, bool aForceReload = false);
		Texture* TryGetTexture(const wchar_t* aTexturePath, bool aForceSRGB = true);

		Texture* GetWhiteSquareTexture() { return myWhiteSquareTexture.get(); }
		static Vector2f GetTextureSize(struct ID3D11ShaderResourceView* aResourceView, bool aNormalize = true);

		Texture * CreateTextureFromTarga(Tga32::Image * aImage);

		void ReleaseTexture(Texture* aTexture);

		void Update();

		/* Requires DX11 includes */
		ID3D11ShaderResourceView* GetDefaultNormalMapResource() const { return myDefaultNormalMapResource.Get(); }
		/* Requires DX11 includes */
		ID3D11ShaderResourceView* GetNoiseTexture() const { return myNoiseResource.Get(); }
	private:
		DXGI_FORMAT GetTextureFormat(struct ID3D11ShaderResourceView* aResourceView) const;
		std::vector<std::unique_ptr<Texture>> myResourceViews;
		void GeneratePerlinNoiseTexture(unsigned short aWidth, unsigned short aHeight);
		void CreateErrorSquareTexture();
		ComPtr<ID3D11ShaderResourceView> CreateWhiteSquareTexture();
		void CreateDefaultNormalmapTexture();
		void OnTextureChanged(std::wstring aFile);
		
		ComPtr<ID3D11Texture2D> myNoiseTexture;
		ComPtr<ID3D11ShaderResourceView> myNoiseResource;
		ComPtr<ID3D11ShaderResourceView> myFailedResource;
		ComPtr<ID3D11ShaderResourceView> myDefaultNormalMapResource;
		std::unique_ptr<Texture> myWhiteSquareTexture;
	};
}