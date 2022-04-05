#include "stdafx.h"
#include <tga2d/primitives/LinePrimitive.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/engine.h>
#include <tga2d/render/Renderer.h>

using namespace Tga2D;

LinePrimitive::LinePrimitive(void)
{
}


LinePrimitive::~LinePrimitive(void)
{
}

void Tga2D::LinePrimitive::Render()
{
	Engine::GetInstance()->GetRenderer().AddToRender(this);
}

void Tga2D::LineMultiPrimitive::Render()
{
	Engine::GetInstance()->GetRenderer().AddToRender(this);
}

void Tga2D::LineMultiPrimitive::Draw(GraphicsEngine* aRenderer)
{
	aRenderer->DoDraw(this);
}
