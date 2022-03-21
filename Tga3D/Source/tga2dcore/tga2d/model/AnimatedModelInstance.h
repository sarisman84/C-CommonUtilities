#pragma once
#include <map>
#include <memory>
#include <tga2d/math/matrix4x4.h>
#include <tga2d/math/transform.h>
#include <tga2d/EngineDefines.h>
#include <tga2d/model/ModelStatus.h>
#include <tga2d/model/model.h>

namespace Tga2D
{
	class AnimatedModel;
	class Animation;
	class ModelShader;
	class TextureResource;
	class AnimatedModelInstance : public RenderObjectSharedData
	{
	public:
		AnimatedModelInstance();
		~AnimatedModelInstance();
		void Init(std::shared_ptr<Model> aModel);
		bool InitAnimations(std::vector<std::wstring>& aAnimationPath);
		int GetAnimationCount() const;
		void SetCurrentAnimationIndex(int aIndex);
		void SetCurrentAnimationTime(float aAnimationFrame);
		ModelStatus Update(float aDelta);
		void RegisterAnimationEventCallback(const std::string& aEvent, std::function<void()> aFunctionToCall);
		void Render(ModelShader& shader) const;

		const Transform& GetTransform() const { return myTransform; }
		void SetTransform(const Transform& someTransform);
		const bool HasAnimation() { return myAnimation ? true : false; }
		void SetRotation(Rotator someRotation);
		void SetLocation(Vector3f someLocation);
		void SetScale(Vector3f someScale);
		bool IsValid() { return myModel ? true : false; }
		void SetTexture(int meshIndex, int textureIndex, TextureResource* texture) { myTextures[meshIndex][textureIndex] = texture; }

	private:
		friend class ForwardRenderer;

		bool SetAnimIndexNoBlend(int index);
		bool SetAnimIndex(int index);
		void TriggerEvents(int aAnimationIndex, float aAtWhatTime);
		void BoneTransform(Matrix4x4f* Transforms);

		Transform myTransform;
		std::shared_ptr<Model> myModel = nullptr;
		std::shared_ptr<Animation> myAnimation = nullptr;
		const TextureResource* myTextures[MAX_MESHES_PER_MODEL][4] = {};
		Matrix4x4f myBones[MAX_ANIMATION_BONES];
		Matrix4x4f myBonesDE[MAX_ANIMATION_BONES];

		int myPrevAnimIndex = 0;
		int myAnimIndex;
		float myTotalAnimationTime[32];
		float myAnimationTimes[32];
		int myPrevEventTickTime[32];
		std::map<unsigned int, std::vector<std::function<void()>>> myEventCallbacks;
	};

}