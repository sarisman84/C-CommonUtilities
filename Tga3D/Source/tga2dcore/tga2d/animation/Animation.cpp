#include "stdafx.h"
#include "Animation.h"
#include <tga2d/animation/AnimationController.h>
#include <tga2d/engine.h>

#pragma comment (lib, "assimp-vc142-mt.lib")

using namespace Tga2D;
Tga2D::Animation::~Animation()
{}

void Animation::Init(const wchar_t* aRig, std::vector<std::wstring>& somePathsToAnimations)
{
	controller = std::make_unique<AnimationController>(aRig);
	controller->Import3DFromFile(aRig);
	for (std::wstring s : somePathsToAnimations)
	{
		controller->Add3DAnimFromFile(s);
	}
	
}

unsigned int Tga2D::Animation::GetNumberOfBones() const
{
	if (!controller)
	{
		return 0;
	}
	return controller->myNumBones;
}

uint Tga2D::Animation::GetAnimationCount() const
{
	if (!controller) 
	{
		return 0;
	}
	return controller->GetMaxIndex();
}
