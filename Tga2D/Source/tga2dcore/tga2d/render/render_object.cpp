#include "stdafx.h"

#include <tga2d/render/render_object.h>
#include <tga2d/d3d/direct_3d.h>


using namespace Tga2D;


CRenderObject::CRenderObject(void)
{
	myIsScreenSpace = false;
	myBlendState = EBlendState::Alphablend;
	mySamplerFilter = ESamplerFilter::Bilinear;
	mySamplerAddressMode = ESamplerAddressMode::Clamp;
}

CRenderObjectLine::CRenderObjectLine()
	:myFromPosition(0.0f, 0.0f),myToPosition(0.0f, 0.0f),myColor(1, 0, 0, 1)
{

}

void CRenderObjectLine::Draw(CDirectEngine* aRenderer)
{
	aRenderer->DoDraw(this);
}

Tga2D::CRenderObjectCustom::CRenderObjectCustom()
{
	myPosition.Set(0, 0);
}

void Tga2D::CRenderObjectCustom::Draw(CDirectEngine* aRenderer)
{
	aRenderer->DoDraw(this);
}
