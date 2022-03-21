#include "stdafx.h"
#include <tga2d/graphics/DX11.h>
#include <tga2d/math/Transform.h>
#include <tga2d/model/Model.h>
#include <tga2d/shaders/ModelShader.h>

using namespace Tga2D;

void Model::Init(MeshData* aMeshDatas, int aMeshCount, bool isAnimated, std::wstring Path)
{
	assert(aMeshCount <= MAX_MESHES_PER_MODEL);
	if (aMeshCount > MAX_MESHES_PER_MODEL)
		aMeshCount = MAX_MESHES_PER_MODEL;

	for (int i = 0; i < aMeshCount; i++)
		myMeshDatas[i] = aMeshDatas[i];

	myMeshCount = aMeshCount;
	myPath = Path;
	myIsAnimated = isAnimated;
}
