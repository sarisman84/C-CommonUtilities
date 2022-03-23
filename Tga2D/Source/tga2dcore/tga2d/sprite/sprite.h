/*
Use this class to create and show a sprite
*/

#pragma once
#include <tga2d/render/render_common.h>
#include <tga2d/math/Color.h>
namespace Tga2D
{
	class CTexture;
	class CSpriteShader;

	struct SSpriteSharedData : SRenderObjectSharedData
	{
		const CTexture* myTexture = nullptr;
		CTexture* myMaps[MAP_MAX] = { nullptr };
		CSpriteShader* myCustomShader = nullptr;
	};

	struct SSpriteInstanceData
	{
		VECTOR2F myPosition = { 0.0f, 0.0f };
		VECTOR2F myPivot = { 0.5f, 0.5f };
		VECTOR2F mySize = { 1.0f, 1.0f };
		VECTOR2F mySizeMultiplier = { 1.0f, 1.0f };
		VECTOR2F myUV = { 0.0f, 0.0f };
		VECTOR2F myUVScale = { 1.0f, 1.0f };
		CColor myColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		STextureRext myTextureRect = { 0.0f, 0.0f, 1.0f, 1.0f };;
		float myRotation = 0.f;
		bool myIsHidden = false;
	};
}