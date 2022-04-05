#pragma once
#include <string>
#include <vector>
#include <map>

class ObjectDataRaw
{
public:
	float myRotX;
	float myRotY;
	float myRotZ;
	float myPosX;
	float myPosY;
	float myPosZ;

	float myScaleX;
	float myScaleY;
	float myScaleZ;
};

class ObjectDataBin : public ObjectDataRaw
{
public:
	int myModelIndex;
};
class ObjectData : public ObjectDataRaw
{
public:
	std::string myRelativePath;
};


namespace Tga2D {
	// @todo: stuff above should probably just be removed
	class ModelInstance;

	class UnityLoader
	{
	public:
		UnityLoader();
		void LoadModels(std::string aModelPathFile);
		std::vector<ObjectData> LoadGameObjectsBinary(const std::string& aGameObjectFile);
		void LoadGameObjectsASCII(const std::string& aGameObjectFile, std::vector<std::shared_ptr<ModelInstance>> &someModelInstances);
	private:

		std::map<int, std::string> myModelPaths;
		bool myModelPathsFetched = false;
	};
}

