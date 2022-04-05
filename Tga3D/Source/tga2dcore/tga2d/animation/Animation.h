#pragma once
#include <vector>
#include <string>
#include <tga2d/model/ModelStatus.h>
#include <memory>
#include <tga2d/animation/Math/MathFunc.h>
namespace Tga2D
{
	class AnimationController;
	class Animation
	{
	public:
		~Animation();
		void Init(const wchar_t* aRig, std::vector<std::wstring>& somePathsToAnimations);

		unsigned int GetNumberOfBones() const;

		uint GetAnimationCount() const;

		class SceneAnimator* myAnimator;
		class SceneAnimator* myBindPose = nullptr;
		std::vector<int> myActiveAnimations;
		float myAnimTime = 0;
		int myAnimSpeed = 60;

		std::unique_ptr<AnimationController> controller;
	};
}

