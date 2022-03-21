#pragma once
#include <memory>
#include <Tga2D/Math/Matrix4x4.h>
#include <Tga2D/Math/Transform.h>
#include <Tga2D/EngineDefines.h>

namespace Tga2D
{

class Model;
class ModelShader;
class ModelInstance : public RenderObjectSharedData
{
public:
	void Init(std::shared_ptr<Model> aModel);

	Model* GetModel() const;

	const Transform& GetTransform() const { return myTransform; }
	void SetTransform(const Transform& someTransform);

	void SetRotation(Rotator someRotation);
	void SetLocation(Vector3f someLocation);
	void SetScale(Vector3f someScale);
	void SetTexture(int meshIndex, int textureIndex, TextureResource* texture) { myTextures[meshIndex][textureIndex] = texture; }

	const TextureResource* const* GetTextures(int meshIndex) const { return myTextures[meshIndex]; }
	bool IsValid() { return myModel ? true : false; }
	void Render(ModelShader& shader) const;
private:

	std::shared_ptr<Model> myModel{};
	const TextureResource* myTextures[MAX_MESHES_PER_MODEL][4] = {};
	Transform myTransform{};
};

} // namespace Tga2D