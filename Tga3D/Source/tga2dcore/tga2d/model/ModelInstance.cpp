#include "stdafx.h"
#include <tga2d/model/ModelInstance.h>
#include <tga2d/model/Model.h>
#include <tga2d/shaders/ModelShader.h>

using namespace Tga2D;

void ModelInstance::Init(std::shared_ptr<Model> aModel)
{
	myModel = aModel;
}

Model* ModelInstance::GetModel() const
{
	return myModel.get();
}

void ModelInstance::SetTransform(const Transform& someTransform)
{
	myTransform = someTransform;
}

void ModelInstance::SetRotation(Rotator someRotation)
{
	// Really should unroll rotations as well somewhere
	// so we can use -180 to 180 instead of 0 to 360.
	myTransform.SetRotation(someRotation);
}

void ModelInstance::SetLocation(Vector3f someLocation)
{
	myTransform.SetPosition(someLocation);
}

void ModelInstance::SetScale(Vector3f someScale)
{
	myTransform.SetScale(someScale);
}

void ModelInstance::Render(ModelShader& shader) const
{
	const Model::MeshData* meshDatas = myModel->GetMeshDatas();
	int meshCount = myModel->GetMeshCount();

	for (int j = 0; j < meshCount; j++)
	{
		shader.Render(*this, myTextures[j], meshDatas[j], myTransform.GetMatrix());
	}
}

