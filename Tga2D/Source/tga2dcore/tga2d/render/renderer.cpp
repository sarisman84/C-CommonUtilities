#include "stdafx.h"

#include <tga2d/render/renderer.h>
#include <tga2d/engine.h>
#include <tga2d/d3d/direct_3d.h>
#include <tga2d/render/render_object.h>
#include <tga2d/d3d/direct_3d.h>
#include <tga2d/texture/texture.h>

using namespace Tga2D;

CRenderer::CRenderer()
	: myDXEngine(&CEngine::GetInstance()->GetDirect3D())
{}

CRenderer::~CRenderer()
{
	myRenderedSpritesOfSameType.clear();
}

void CRenderer::AddToRender( CRenderObject* aObject )
{
	myDXEngine->Draw(aObject);

}

#define RENDER_WARN_COUNT 20
void Tga2D::CRenderer::Update()
{
	if (CEngine::GetInstance()->IsDebugFeatureOn(EDebugFeature::OptimizeWarnings))
	{
		for (OptimisationMap::iterator iter = myRenderedSpritesOfSameType.begin(); iter != myRenderedSpritesOfSameType.end(); iter++)
		{
			if (iter->second.myCount > RENDER_WARN_COUNT)
			{
				if (!iter->second.myHasWarned)
				{
					INFO_TIP("%s%s%s%i%s%s%i%s%s", "Optimisation warning! Texture: ", iter->second.myPath.c_str(), " was rendered more than ", RENDER_WARN_COUNT, " this frame", "(", iter->second.myCount, ")!", " Consider making it a spritebath");
				}
				iter->second.myHasWarned = true;
				
			}
			iter->second.myCount = 0;
		}
	}
}

