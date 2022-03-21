#include "stdafx.h"
#include <tga2d/primitives/CustomShape.h>
#include <tga2d/engine.h>
#include <tga2d/render/renderer.h>
#include <tga2d/texture/TextureManager.h>

using namespace Tga2D;
CustomShape::CustomShape()
{
	myIndex = 0;
	myIsDirty = false;
	myTexture = nullptr;
	myBlendState = BlendState::AlphaBlend;
	myHasTexture = false;
	SetSize({ 1, 1 });
}

CustomShape::~CustomShape()
{
}

void CustomShape::Reset()
{
	myPoints.clear();
}

int CustomShape::AddPoint(Vector2f aPoint, Color aColor, Vector2f aUV)
{
	SCustomPoint point;
	point.myPosition = aPoint;
	point.myColor = aColor;
	point.myIndex = myIndex;
	point.myUV = aUV;
	myPoints.push_back(point);
	if (myPoints.size()%3 == 0)
	{
		myIndex++;
	}
	
	myIsDirty = true;
	return point.myIndex;
}

// Do this second
void CustomShape::BuildShape()
{
#ifdef _DEBUG
	if (myPoints.size() %3 != 0)
	{
		INFO_PRINT("%s", "CustomShape::BuildShape(): building shape with non even devided by 3 points, invalid! We need it to build triangles!");
		return;
	}

	if (myHasTexture && myTexture)
	{

		bool noUVsSet = true;
		for (SCustomPoint& point : myPoints)
		{
			if (point.myUV.x != 0 && point.myUV.y != 0)
			{
				noUVsSet = false;
			}
		}
		if (noUVsSet)
		{
			ERROR_PRINT("%s", "Warning! A custom shape that have a texture set have no valid UV coordinates, the shape will most likley not be shown. Set valid UV coordinates,")
		}
	}
#endif

	if (!myTexture)
	{
		myTexture = Engine::GetInstance()->GetTextureManager().GetWhiteSquareTexture();
	}

	myIsDirty = false;
}

void Tga2D::CustomShape::RemovePoint(int aIndex)
{
	for (size_t i = myPoints.size() - 1; i > 0; i--)
	{
		if (myPoints[i].myIndex == aIndex)
		{
			myPoints.erase(myPoints.begin() + i);
			myIsDirty = true;
		}
	}
}

void Tga2D::CustomShape::Render()
{
	if (myIsDirty)
	{
		ERROR_PRINT("%s", "CustomShape::Render() shape is dirty, please call CustomShape::BuildShape() to verify shape");
		return;
	}
	Engine::GetInstance()->GetRenderer().AddToRender(this);
}

void Tga2D::CustomShape::SetTexture(const wchar_t* aPath)
{
	myTexture = Engine::GetInstance()->GetTextureManager().GetTexture(aPath);
	myHasTexture = false;
	if (myTexture && !myTexture->myIsFailedTexture)
	{
		myHasTexture = true;
	}
}

