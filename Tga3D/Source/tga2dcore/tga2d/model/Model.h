#pragma once
#include <d3d11.h>
#include <string>
#include <vector>

#include <Tga2d/Math/Vector.h>

namespace Tga2D
{

class ModelShader;
struct RenderObjectSharedData;
class TextureResource;

struct BoxSphereBounds
{
	// The radius of the Sphere
	float Radius;
	// The extents of the Box
	Vector3f BoxExtents;
	// The local-space center of the shape.
	Vector3f Center;
};

class Model
{
public:
	struct MeshData
	{
		std::string myName;
		std::string myMaterialName;
		UINT myNumberOfVertices;
		UINT myNumberOfIndices;
		UINT myStride;
		UINT myOffset;
		ID3D11Buffer* myVertexBuffer;
		ID3D11Buffer* myIndexBuffer;
		BoxSphereBounds myBounds;
	};

	void Init(MeshData* aMeshDatas, int aMeshCount, bool isAnimated, std::wstring Path);

	const char* GetMaterialName(int meshIndex) { return myMeshDatas[meshIndex].myMaterialName.c_str(); }
	const char* GetMeshName(int meshIndex) { return myMeshDatas[meshIndex].myName.c_str(); }

	int GetMeshCount() const {return myMeshCount;}
	const MeshData* GetMeshDatas() { return myMeshDatas; };
	const std::wstring& GetPath() { return myPath; }
	bool GetIsAnimated() { return myIsAnimated; }
private:
	bool myIsAnimated = false;
	int myMeshCount = 0;
	MeshData myMeshDatas[MAX_MESHES_PER_MODEL] = {};
	std::wstring myPath;
};

} // namespace Tga2D