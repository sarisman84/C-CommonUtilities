#pragma once
#include <map>
#include <string>
#include <vector>
#include <tga2d/graphics/Vertex.h>
#include <tga2d/model/model.h>
#include <tga2d/model/AnimatedModelInstance.h>

struct ID3D11Device;

namespace Tga2D
{

class Texture;
class AnimatedModel;

struct MeshData
{
	std::vector<Vertex> myVertices;
	std::vector<unsigned int> myIndices;
	BoxSphereBounds myBounds;
};

class ModelInstance;
class Model;

class ModelFactory
{
	bool InitUnitCube();
	bool InitUnitPlane();
	bool InitPrimitives();
	
public:
	ModelFactory();
	~ModelFactory();
	static ModelFactory& GetInstance() { if (!myInstance) { myInstance = new ModelFactory(); } return *myInstance; }
	static void DestroyInstance() { if (myInstance) { delete myInstance; myInstance = nullptr; } }

	AnimatedModelInstance GetAnimatedModel(const std::wstring& someFilePath);
	ModelInstance GetModel(const std::wstring& someFilePath);
	ModelInstance GetUnitCube();
	ModelInstance GetUnitPlane();
	bool ModelHasMesh(const std::wstring& someFilePath);
protected:
	std::shared_ptr<Model> GetModelInternal(const std::wstring& someFilePath);
	std::shared_ptr<Model> LoadModel(const std::wstring& someFilePath);
	
private:	
	std::map<std::wstring, Model*> myLoadedModels;
	std::vector<std::shared_ptr<Model>> myModels;	
	static ModelFactory* myInstance;	
};

} // namespace Tga2D
