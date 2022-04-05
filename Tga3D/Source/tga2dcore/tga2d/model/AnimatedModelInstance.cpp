#include "stdafx.h"
#include "AnimatedModelInstance.h"
#include <tga2d/model/ModelFactory.h>
#include <tga2d/animation/Animation.h>
#include <tga2d/animation/AnimationController.h>
#include <tga2d/drawers/DebugDrawer.h>
#include <tga2d/shaders/ModelShader.h>

using namespace Tga2D;
AnimatedModelInstance::AnimatedModelInstance()
{}

AnimatedModelInstance::~AnimatedModelInstance() {}


void AnimatedModelInstance::Init(std::shared_ptr<Model> aModel)
{
	myModel = aModel;
}

void AnimatedModelInstance::SetTransform(const Transform& someTransform)
{
	myTransform = someTransform;
}

void AnimatedModelInstance::SetRotation(Rotator someRotation)
{
	// Really should unroll rotations as well somewhere
	// so we can use -180 to 180 instead of 0 to 360.
	myTransform.SetRotation(someRotation);
}

void AnimatedModelInstance::SetLocation(Vector3f someLocation)
{
	myTransform.SetPosition(someLocation);
}

void AnimatedModelInstance::SetScale(Vector3f someScale)
{
	myTransform.SetScale(someScale);
}

bool AnimatedModelInstance::InitAnimations(std::vector<std::wstring>& someAnimationPaths)
{
	if (!myAnimation)
	{
		myAnimation = std::make_shared<Animation>();	
	}

	myAnimation->Init(myModel->GetPath().c_str(), someAnimationPaths);
	return true;
}

int AnimatedModelInstance::GetAnimationCount() const
{
	if (!myAnimation)
	{
		return 0;
	}
	return myAnimation->GetAnimationCount();
}

ModelStatus AnimatedModelInstance::Update(float aDelta)
{
	ModelStatus status = ModelStatus::Nothing;
	if (myAnimation && myAnimIndex != -1)
	{
		memset(myBones, 0, sizeof(myBones));
		
		if (myAnimIndex != -1)
		{
			AnimationController* controller = myAnimation->controller.get();

			float TicksPerSecond = (float)controller->_scenes[myAnimIndex]->mAnimations[0]->mTicksPerSecond != 0 ? (float)controller->_scenes[myAnimIndex]->mAnimations[0]->mTicksPerSecond : 25.0f;
			myTotalAnimationTime[myAnimIndex] += aDelta * TicksPerSecond;
			float realDuration = (float)controller->_scenes[myAnimIndex]->mAnimations[0]->mDuration;
			if (myTotalAnimationTime[myAnimIndex] >= realDuration)
			{
				myTotalAnimationTime[myAnimIndex] = 0.0f;
				status = ModelStatus::Animation_End;
			}
			myAnimationTimes[myAnimIndex] += aDelta;
		}

		BoneTransform(&myBones[0]);
	}
	else
	{
		// if there is no animation to play, reset to default transforms
		for (int i = 0; i < MAX_ANIMATION_BONES; i++)
			myBones[i] = Matrix4x4f();
	}

	return status;
}

void Tga2D::AnimatedModelInstance::SetCurrentAnimationIndex(int aIndex)
{
	if (!myAnimation)
	{
		ERROR_PRINT("%s", "SetCurrentAnimationIndex: no animation is loaded");
		return;
	}
	SetAnimIndexNoBlend(aIndex);
}

void Tga2D::AnimatedModelInstance::SetCurrentAnimationTime(float aAnimationFrame)
{
	if (!myAnimation)
	{
		ERROR_PRINT("%s", "SetCurrentAnimationIndex: no animation is loaded");
		return;
	}
	myAnimationTimes[myAnimIndex] = aAnimationFrame;

}

void Tga2D::AnimatedModelInstance::RegisterAnimationEventCallback(const std::string& aEvent, std::function<void()> aFunctionToCall)
{
	aiString eventString(aEvent);
	myEventCallbacks[myAnimation->controller->m_EventMapping[eventString]].push_back(aFunctionToCall);
}

void AnimatedModelInstance::Render(ModelShader& shader) const
{
	const Model::MeshData* meshDatas = myModel->GetMeshDatas();
	int meshCount = myModel->GetMeshCount();

	for (int j = 0; j < meshCount; j++)
	{
		shader.Render(*this, myTextures[j], meshDatas[j], myTransform.GetMatrix(), myBones);
	}
}

bool AnimatedModelInstance::SetAnimIndexNoBlend(int index)
{
	if (index >= GetAnimationCount())
	{
		myPrevAnimIndex = myAnimIndex;
		myAnimIndex = -1;
		return false;
	}

	if (myAnimIndex == index)
		return true;

	myPrevAnimIndex = myAnimIndex;
	myAnimIndex = index;
	myAnimationTimes[myAnimIndex] = 0;
	return true;
}

bool AnimatedModelInstance::SetAnimIndex(int index)
{
	if (index >= GetAnimationCount())
	{
		myPrevAnimIndex = myAnimIndex;
		myAnimIndex = -1;
		return false;
	}

	if (myAnimIndex == index)
		return true;

	myPrevAnimIndex = myAnimIndex;
	myAnimIndex = index;
	return true;
}

void AnimatedModelInstance::TriggerEvents(int aAnimationIndex, float aAtWhatTime)
{
	int eventFrameIndex = (int)aAtWhatTime;
	if (myPrevEventTickTime[aAnimationIndex] == eventFrameIndex)
		return;

	myPrevEventTickTime[aAnimationIndex] = eventFrameIndex;

	for (auto& it : myAnimation->controller->myEventFrames[aAnimationIndex][eventFrameIndex].myEvents)
	{
		if (it.second.isTriggerFrame)
		{
			for (auto& listener : myEventCallbacks[it.first])
			{
				listener();
			}
		}
	}
}

void AnimatedModelInstance::BoneTransform(Matrix4x4f* Transforms)
{
	AnimationController* controller = myAnimation->controller.get();

	aiMatrix4x4 Identity;
	InitIdentityM4(Identity);

	float TicksPerSecond = (float)controller->_scenes[myAnimIndex]->mAnimations[0]->mTicksPerSecond != 0 ? (float)controller->_scenes[myAnimIndex]->mAnimations[0]->mTicksPerSecond : 25.0f;
	float TimeInTicks = myAnimationTimes[myAnimIndex] * TicksPerSecond;

	float AnimationTime = fmod(TimeInTicks, (float)controller->_scenes[myAnimIndex]->mAnimations[0]->mDuration);

	TriggerEvents(myAnimIndex, AnimationTime);

	AnimationTime *= ANIMATION_PRECISION;
	AnimationTime = std::min(AnimationTime, (float)(controller->myTransformSize[myAnimIndex] - 1));
	unsigned int animTimeInt = static_cast<unsigned int>(AnimationTime);
	assert(animTimeInt <= controller->myTransformSize[myAnimIndex]);
	animTimeInt;

	memcpy(Transforms, &controller->myTransforms[myAnimIndex][static_cast<int>(AnimationTime)][0], sizeof(aiMatrix4x4) * controller->myNumBones);
}
