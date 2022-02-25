#include "stdafx.h"
#include <tga2d/primitives/line_primitive.h>
#include <tga2d/engine.h>
#include <tga2d/render/Renderer.h>

using namespace Tga2D;

CLinePrimitive::CLinePrimitive(void)
{
}


CLinePrimitive::~CLinePrimitive(void)
{
}

void Tga2D::CLinePrimitive::Render()
{
	CEngine::GetInstance()->GetRenderer().AddToRender(this);
}

void Tga2D::CLineMultiPrimitive::Render()
{
	CEngine::GetInstance()->GetRenderer().AddToRender(this);
}

void Tga2D::CLineMultiPrimitive::Draw(CDirectEngine* aRenderer)
{
	aRenderer->DoDraw(this);
}
