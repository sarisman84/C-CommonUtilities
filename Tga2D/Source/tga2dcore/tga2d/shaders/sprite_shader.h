#pragma once
#include <tga2d/math/matrix44.h>
#include <tga2d/shaders/shader.h>

#define MAX_SHADER_DATA 8
#define MAX_SHADER_TEXTURES 2

using Microsoft::WRL::ComPtr;

namespace Tga2D
{
	class CDirectEngine;
	class CEngine;
	class CTexture;
	struct SSpriteSharedData;

	enum EShaderDataID
	{
		EShaderDataID_1,
		EShaderDataID_2,
		EShaderDataID_3,
		EShaderDataID_4,
		EShaderDataID_5,
		EShaderDataID_6,
		EShaderDataID_7,
		EShaderDataID_8,
	};
	enum EShaderTextureSlot
	{
		EShaderTextureSlot_1 = 4, // Below 4 is the standard texture slots, keep above it! (: register( t4 ); in the shader)
		EShaderTextureSlot_2 = 5,
		EShaderTextureSlot_3 = 6,
		EShaderTextureSlot_4 = 7,
	};
	enum EShaderDataBufferIndex
	{
		EShaderDataBufferIndex_1 = 5, // Below 5 is the standard buffer slots, keep above it! (: register(b5) in shader)
		EShaderDataBufferIndex_2 = 6,
		EShaderDataBufferIndex_3 = 7,
		EShaderDataBufferIndex_4 = 8,
	};

	class CSpriteShader : private CShader
	{
		friend class CSpriteDrawer;
	public:
		CSpriteShader();
		bool Init(const char* aVertex = "shaders/instanced_sprite_shader_vs.fx", const char* aPixel = "shaders/sprite_shader_ps.fx");


		void SetDataBufferIndex(EShaderDataBufferIndex aBufferRegisterIndex);
		// Add a float4 to a shader, this must be in the shade file matching 1:1! The Id is the index of the float4 in the shader (top down starting from 0)
		void SetShaderdataFloat4(Tga2D::Vector4f someData, EShaderDataID aID);

		// Set a texture to the shader, the index: 0, 1, 2, 3 are reserved. Keep the index above this and register it with the same id in the shader
		void SetTextureAtRegister(Tga2D::CTexture* aTexture, EShaderTextureSlot aRegisterIndex);

		bool PrepareRender(const SSpriteSharedData& aSharedData);

	protected:
		bool CreateInputLayout(ID3D10Blob* aVS) override;

	private:
		Tga2D::Vector4f myCustomData[MAX_SHADER_DATA];
		int myCurrentDataIndex;
		unsigned char myCurrentTextureCount;

		ComPtr<ID3D11Buffer> myCustomBuffer;
		unsigned char myBufferIndex;

		struct SBoundTexture
		{
			SBoundTexture() {}
			SBoundTexture(Tga2D::CTexture* aTex, unsigned char aIndex)
			{
				myTexture = aTex;
				myIndex = aIndex;
			}
			Tga2D::CTexture* myTexture = nullptr;
			unsigned char myIndex = 0;
		};
		SBoundTexture myBoundTextures[MAX_SHADER_TEXTURES];
	};
}