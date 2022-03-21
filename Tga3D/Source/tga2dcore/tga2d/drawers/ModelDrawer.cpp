#include "stdafx.h"

#include <tga2d/drawers/ModelDrawer.h>
#include <tga2d/shaders/ModelShader.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/DX11.h>
#include <tga2d/engine.h>
#include <tga2d/math/Matrix2x2.h>
#include <tga2d/model/AnimatedModelInstance.h>
#include <tga2d/model/ModelInstance.h>

using namespace Tga2D;

constexpr size_t BATCH_SIZE = 1024;

ModelDrawer::ModelDrawer()
{
}

ModelDrawer::~ModelDrawer()
{
}

bool ModelDrawer::Init()
{
	myDefaultShader = std::make_unique<ModelShader>(Engine::GetInstance());
	if (!myDefaultShader->Init())
	{
		return false;
	}

	myDefaultAnimatedModelShader = std::make_unique<ModelShader>(Engine::GetInstance());
	if (!myDefaultAnimatedModelShader->Init(L"shaders/animated_model_shader_VS.cso", L"shaders/model_shader_PS.cso"))
	{
		return false;
	}

	return true;
}

void ModelDrawer::Draw(const AnimatedModelInstance& modelInstance)
{
	modelInstance.Render(*myDefaultAnimatedModelShader);
}

void ModelDrawer::Draw(const ModelInstance& modelInstance)
{
	modelInstance.Render(*myDefaultShader);
}