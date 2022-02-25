#pragma once
#include <tga2d/math/vector2.h>
#include <tga2d/math/color.h>
#define SPRITE_BATCH_COUNT 1024
namespace Tga2D
{
	enum class EConstantBufferSlot
	{
		Matrix = 0,
		Common = 1,
		Light = 2,
		Object = 3,
		ObjectCustom = 4,
		Count,
	};
	enum SEffect
	{
		SEffect_None,
	};
	struct SSpriteShaderInstanceData
	{
		SSpriteShaderInstanceData()
		{
			myPosition.Set(0, 0);
		}
		VECTOR2F myPosition;
		Vector4f myColor;
		Vector4f myRotationAndSize;
		Vector4f myUV;
		Vector4f myPivot;
		Vector4f myUVRect;
	};

}