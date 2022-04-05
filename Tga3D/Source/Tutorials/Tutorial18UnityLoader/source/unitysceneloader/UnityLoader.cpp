#include <stdafx.h>

#include "UnityLoader.h"

#include <tga2d/Model/ModelFactory.h> // ?? used?
#include <tga2d/Model/ModelInstance.h>

#include <functional>
#include <fstream>
#include <streambuf>
#include <assert.h>

#include <json.hpp>
#include <array>

using namespace Tga2D;

UnityLoader::UnityLoader(){}

void UnityLoader::LoadModels(std::string aModelPathFile)
{
	std::ifstream modelFile(aModelPathFile);
	std::string s;
	int index = -1;
	while (std::getline(modelFile, s))
	{
		index++;
		std::replace(s.begin(), s.end(), '/', '\\');
		myModelPaths[index] = s;
	}

	myModelPathsFetched = true;
}

// Much faster than ASCII load, but unreadable files, better with ASCII for debugging
std::vector<ObjectData> UnityLoader::LoadGameObjectsBinary(const std::string& aGameObjectFile)
{
	std::ifstream t(aGameObjectFile, std::ios::binary);
	assert(t.is_open());

	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	unsigned int count = 0;
	memcpy(&count, &str[0], sizeof(int));

	assert(count != 0);
	char* ptr = &str[0];
	ptr += sizeof(int);

	ObjectDataBin* data = new ObjectDataBin[count];
	memcpy(data, ptr, sizeof(ObjectDataBin) * count);

	std::vector<ObjectData> returnedData;
	for (size_t i=0; i< count; i++)
	{
		returnedData.push_back(ObjectData());
		ObjectData& object = returnedData.back();

		memcpy(&object, &data[i], sizeof(ObjectDataRaw)); // Memcpy all the essential data
		object.myRelativePath = myModelPaths[data[i].myModelIndex];
	}
	delete[] data;
	t.close();
	return returnedData;
}

// Use only for debugging, terribly slow, stringcomparison hell
void UnityLoader::LoadGameObjectsASCII(
	const std::string& aGameObjectFile, 
	std::vector<std::shared_ptr<ModelInstance>>& someModelInstances
) {
	std::vector<ObjectData> loadedObjects;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	// temporary scope where nlohmann is used, since it's only interesting while reading ASCII I limited it to this specific scope
	{
		std::ifstream t(aGameObjectFile);
		assert(t.is_open());

		using namespace nlohmann;
		json j = json::parse(t);
		t.close();
		Tga2D::ModelFactory modelFactory;

		for (auto& o : j["dataList"]) {
			std::string s = o["myPath"];
			if (s.length() == 0) continue;

			std::wstring wsTmp(s.begin(), s.end());
			// \Assets\Assets\Platforms
			wsTmp = L"../Source/Tutorials/Tutorial18UnityLoader/data/" + wsTmp;

			std::shared_ptr<ModelInstance> obj = std::make_shared<ModelInstance>(modelFactory.GetModel(wsTmp));
			Vector3f pos = { o["myPosX"], o["myPosY"], o["myPosZ"] };

			obj->SetLocation(pos * 100.0f);
			//obj->SetRotation({ o["myRotX"], o["myRotY"], o["myRotZ"] });
			obj->SetScale({ o["myScaleX"] * -1.0f, o["myScaleY"], o["myScaleZ"] * -1.0f });

			// std::replace(object.myRelativePath.begin(), object.myRelativePath.end(), '/', '\\');

			someModelInstances.emplace_back(obj);
		}
	}
}

