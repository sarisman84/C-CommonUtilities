/*
This class handles all the textures created, please use this when you create a texture
*/

#pragma once
#include "Texture.h"
#include <dxgiformat.h>
#include <vector>
#include <tga2d/loaders/tgaloader.h>

struct ID3D11ShaderResourceView;
namespace Tga2D
{
	class CDirectEngine;
	class CTextureManager
	{
	public:
		CTextureManager(void);
		~CTextureManager(void);
		void Init();
		CTexture* GetTexture(const char* aTexturePath, bool aForceReload = false);
		std::unique_ptr<CRenderTarget> CreateRenderTarget(const VECTOR2UI& aSize);
		
		CTexture* GetWhiteSquareTexture() { return myWhiteSquareTexture.get(); }
		static VECTOR2F GetTextureSize(struct ID3D11ShaderResourceView* aResourceView, bool aNormalize = true);

		CTexture * CreateTextureFromTarga(Tga32::Image * aImage);

		void ReleaseTexture(CTexture* aTexture);

		void Update();

		/* Requires DX11 includes */
		ID3D11ShaderResourceView* GetDefaultNormalMapResource() const { return myDefaultNormalMapResource.Get(); }
		/* Requires DX11 includes */
		ID3D11ShaderResourceView* GetNoiseTexture() const { return myNoiseResource.Get(); }
	private:
		DXGI_FORMAT GetTextureFormat(struct ID3D11ShaderResourceView* aResourceView) const;
		std::vector<std::unique_ptr<CTexture>> myResourceViews;
		void GeneratePerlinNoiseTexture(unsigned short aWidth, unsigned short aHeight);
		void CreateErrorSquareTexture();
		ComPtr<ID3D11ShaderResourceView> CreateWhiteSquareTexture();
		void CreateDefaultNormalmapTexture();
		void OnTextureChanged(std::string aFile);
		
		ComPtr<ID3D11Texture2D> myNoiseTexture;
		ComPtr<ID3D11ShaderResourceView> myNoiseResource;
		ComPtr<ID3D11ShaderResourceView> myFailedResource;
		ComPtr<ID3D11ShaderResourceView> myDefaultNormalMapResource;
		CDirectEngine* myDirect3D;
		std::unique_ptr<CTexture> myWhiteSquareTexture;
	};
}