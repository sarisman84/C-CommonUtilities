#pragma once
#include <tga2d/math/Vector2.h>

namespace Tga2D
{

	enum class EShader
	{
		Normal,
		FullScreen,
		Count
	};

	enum class EBlendState
	{
		Disabled,
		Alphablend,
		Additiveblend,
		Count
	};

	enum class ESamplerFilter
	{
		Point,
		Bilinear,
		Trilinear,
		Count
	};

	enum class ESamplerAddressMode
	{
		Clamp,
		Wrap,
		Mirror,
		Count
	};

	enum EShaderMap
	{
		NORMAL_MAP,
		MAP_MAX,
	};

#pragma warning( push )
#pragma warning( disable : 26495)

	struct SColor
	{
		SColor() {}
		SColor(float aR, float aG, float aB, float aA)
			:myR(aR)
			, myG(aG)
			, myB(aB)
			, myA(aA)
		{}
		float myR, myG, myB, myA;    // color
	};

#pragma warning( pop )

	struct SVertex
	{
		float X, Y, Z;      // position
		float texX, texY;    // tex
	};

	struct SVertexInstanced
	{
		float X, Y, Z, W;      // position
		unsigned int myVertexIndex, unused1, unused2, unused3;
		float texX, texY;    // tex
	};

	struct SSimpleVertex
	{
		float X, Y, Z;      // position
		float myColorR, myColorG, myColorB, myColorA;
		float myU, myV;
	};

	struct STextureRext
	{
		float myStartX;
		float myStartY;
		float myEndX;
		float myEndY;
	};

	struct SRenderObjectSharedData
	{
		EBlendState myBlendState = EBlendState::Alphablend;
		ESamplerFilter mySamplerFilter = ESamplerFilter::Bilinear;
		ESamplerAddressMode mySamplerAddressMode = ESamplerAddressMode::Clamp;
	};
}