#include "stdafx.h"

#include <tga2d/render/RenderObject.h>
#include <tga2d/graphics/GraphicsEngine.h>


using namespace Tga2D;


RenderObject::RenderObject(void)
{
	myIsScreenSpace = false;
	myBlendState = BlendState::AlphaBlend;
	mySamplerFilter = SamplerFilter::Bilinear;
	mySamplerAddressMode = SamplerAddressMode::Clamp;
}

RenderObjectLine::RenderObjectLine()
	:myFromPosition(0.0f, 0.0f),myToPosition(0.0f, 0.0f),myColor(1, 0, 0, 1)
{

}

void RenderObjectLine::Draw(GraphicsEngine* aRenderer)
{
	aRenderer->DoDraw(this);
}

Tga2D::RenderObjectCustom::RenderObjectCustom()
{
	myPosition.Set(0, 0);
}

void Tga2D::RenderObjectCustom::Draw(GraphicsEngine* aRenderer)
{
	aRenderer->DoDraw(this);
}
