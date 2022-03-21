#include <string>
#include <map>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/LogStream.hpp"
#include <fstream>
#include <tga2d/EngineDefines.h>
#include <tga2d/animation/Math/MathFunc.h>
#include <tga2d/util/StringCast.h>

#define NUM_BONES_PER_VERTEX 4

inline bool operator <(const aiString& aLhs, const aiString& aRhs)
{
	return strcmp(aLhs.data, aRhs.data) < 0;
}

namespace Tga2D
{
	struct BoneInfoAnim
	{
		aiMatrix4x4 BoneOffset;
		aiMatrix4x4 FinalTransformation;
	};

	class AnimationController
	{
	public:
		struct Event
		{
			aiMatrix4x4 myNodeTransformation;
			bool isSet = false;
			bool isTriggerFrame = false;
		};
		struct AnimEvent
		{
			std::map<int, Event> myEvents;
		};

		std::wstring m_ModelPath;
		aiVector3D _rotation;

		std::vector<Assimp::Importer*> _importers;
		std::vector<const aiScene*> _scenes;

		aiMatrix4x4 m_GlobalInverseTransform;
		std::map<aiString, uint> m_BoneMapping;
		std::vector<BoneInfoAnim> m_BoneInfo;
		std::vector<BoneInfoAnim> m_BoneInfoEvent;
		uint myNumBones;
		aiMatrix4x4** myTransforms[32];
		AnimEvent* myEventFrames[32];
		unsigned int myTransformSize[32];

		std::map<aiString, int> m_EventMapping;
		int m_EventCount = 0;

		void SetRotation(const aiVector3D& r) { _rotation = r; }
		aiVector3D GetRotation() { return _rotation; }

		~AnimationController()
		{
			Release();
		}

		void Release()
		{
			for (uint i = 0; i < _importers.size(); ++i)
			{
				delete _importers[i];
			}
			_importers.clear();

			for (int i=0; i< _scenes.size(); i++)
			{
				for (uint j = 0; j < myTransformSize[i]; j++)
				{
					delete[] myTransforms[i][j];
				}
				delete[]myTransforms[i];
			}	
		}

		AnimationController(const wchar_t* modelpath)
			: myNumBones(0)			
			, m_ModelPath(modelpath)
			, _rotation(0.f)

		{
			if constexpr (ANIMATION_PRECISION > 4)
			{
				INFO_TIP("%s", "Animation precision is set very high, this will impact loadtimes and memory. Please consider lower as this impacts every animation ingame");
			}
		}

		void ReadAsEvent(const aiString& aName, const aiScene* scene, float AnimationTime, const aiMatrix4x4& ParentTransform, aiNode* pNode, int stopAnimLevel)
		{
			pNode;
			ParentTransform;
			float time(AnimationTime);
			const aiNodeAnim* pNodeAnim = FindNodeAnim(scene->mAnimations[0], aName);
			aiMatrix4x4 NodeTransformation;
			if (pNodeAnim)
			{

				aiVector3D Translation;
				{
				//	float time2(stopAnimLevel <= 0 ? AnimationTime : 0.f);
					CalcPosition(Translation, AnimationTime, pNodeAnim);

				}
				aiMatrix4x4 TranslationM;
				aiMatrix4x4::Translation(Translation, TranslationM);

				// Combine the above transformations
				NodeTransformation = TranslationM;
			}
			stopAnimLevel--;

			if (m_EventMapping.find(aName) == m_EventMapping.end())
				m_EventMapping[aName] = m_EventCount++;

			int eventId = m_EventMapping[aName];

			Event& origin = myEventFrames[_importers.size()-1][0].myEvents[eventId];
			if (time == 0 && !origin.isSet)
			{
				origin.isSet = true;
				myEventFrames[_importers.size() - 1][(unsigned int)time].myEvents[eventId].myNodeTransformation = NodeTransformation;
			}

			if (origin.myNodeTransformation != NodeTransformation)
			{
				myEventFrames[_importers.size() - 1][(unsigned int)time].myEvents[eventId].isTriggerFrame = true;
			}
		}

		void ReadNodeHeirarchyEvents(const aiScene* scene, float AnimationTime, aiNode* pNode, const aiMatrix4x4& ParentTransform, int stopAnimLevel)
		{
			aiString NodeName(pNode->mName);
			aiMatrix4x4 NodeTransformation(pNode->mTransformation);

			bool ignoreChild = false;
			if (strstr(NodeName.data, "event") != 0)
			{
				ReadAsEvent(NodeName, scene, AnimationTime, ParentTransform, pNode, stopAnimLevel);
				ignoreChild = true;

			}
			aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;
			for (uint i = 0; i < pNode->mNumChildren; i++)
			{
				ReadNodeHeirarchyEvents(scene, AnimationTime, pNode->mChildren[i], GlobalTransformation, stopAnimLevel);
			}
		}

		void ReadNodeHeirarchy(const aiScene* scene, float AnimationTime, aiNode* pNode, const aiMatrix4x4& ParentTransform, int stopAnimLevel)
		{
			float time(AnimationTime);
			aiString NodeName(pNode->mName);
			aiMatrix4x4 NodeTransformation(pNode->mTransformation);
			const aiNodeAnim* pNodeAnim = FindNodeAnim(scene->mAnimations[0], NodeName);
			if (pNodeAnim)
			{
				// Interpolate scaling and generate scaling transformation matrix
				aiVector3D Scaling;
				CalcInterpolatedScaling(Scaling, time, pNodeAnim);
				aiMatrix4x4 ScalingM;
				aiMatrix4x4::Scaling(Scaling, ScalingM);

				// Interpolate rotation and generate rotation transformation matrix
				aiQuaternion RotationQ;
				CalcInterpolatedRotation(RotationQ, time, pNodeAnim);
				aiMatrix4x4 RotationM;
				InitM4FromM3(RotationM, RotationQ.GetMatrix());

				// Interpolate translation and generate translation transformation matrix
				aiVector3D Translation;
				{
					float time2(stopAnimLevel <= 0 ? AnimationTime : 0.f);
					CalcInterpolatedPosition(Translation, time2, pNodeAnim);

				}
				aiMatrix4x4 TranslationM;
				aiMatrix4x4::Translation(Translation, TranslationM);

				// Combine the above transformations
				NodeTransformation = TranslationM * RotationM * ScalingM;
			}
			stopAnimLevel--;

			aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;
			if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
			{
				uint BoneIndex = m_BoneMapping[NodeName];
				m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
			}

			for (uint i = 0; i < pNode->mNumChildren; i++)
			{
				ReadNodeHeirarchy(scene, AnimationTime, pNode->mChildren[i], GlobalTransformation, stopAnimLevel);
			}
		}

		void LoadBones( const aiMesh* pMesh)
		{
			for (uint i = 0; i < pMesh->mNumBones; i++) {
				uint BoneIndex = 0;
				aiString BoneName(pMesh->mBones[i]->mName);

				if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
					BoneIndex = myNumBones;
					myNumBones++;
					BoneInfoAnim bi;
					m_BoneInfo.push_back(bi);
				}
				else {
					BoneIndex = m_BoneMapping[BoneName];
				}

				m_BoneMapping[BoneName] = BoneIndex;
				m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;

			}

		}

		void BakeAnimation(int aSceneIndex)
		{
			aiMatrix4x4 Identity;
			InitIdentityM4(Identity);
			float duration = (float)_scenes[aSceneIndex]->mAnimations[0]->mDuration * ANIMATION_PRECISION;
			myTransforms[aSceneIndex] = new aiMatrix4x4*[(int)duration];

			myTransformSize[aSceneIndex] = (int)duration;
			for (int i = 0; i < (int)duration; i++)
			{
				myTransforms[aSceneIndex][i] = new aiMatrix4x4[myNumBones];
			}
			for (int i = 0; i < duration; i++)
			{
				float currentTime = (((float)i / duration) * duration) / ANIMATION_PRECISION;
				ReadNodeHeirarchy(_scenes[aSceneIndex], currentTime, _scenes[aSceneIndex]->mRootNode, Identity, 1);

				for (uint k = 0; k < myNumBones; k++)
				{
					myTransforms[aSceneIndex][i][k] = m_BoneInfo[k].FinalTransformation;
				}
			}

			float realDuration = (float)_scenes[aSceneIndex]->mAnimations[0]->mDuration;
			myEventFrames[aSceneIndex] = new AnimEvent[(int)realDuration];
			for (int i = 0; i < realDuration; i++)
			{
				float currentTime = (((float)i / realDuration) * realDuration);
				ReadNodeHeirarchyEvents(_scenes[aSceneIndex], currentTime, _scenes[aSceneIndex]->mRootNode, Identity, 1);
			}
		}

		bool Import3DFromFile(const std::wstring& pFile = L"")
		{
			if (pFile.length())
				m_ModelPath = pFile;

			// Check if file exists
			std::ifstream fin(m_ModelPath.c_str());
			if (!fin.fail())
			{
				fin.close();
			}
			else
			{
				MessageBoxA(NULL, ("Couldn't open file: " + string_cast<std::string>(m_ModelPath)).c_str(), "ERROR", MB_OK | MB_ICONEXCLAMATION);
				return false;
			}

			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(string_cast<std::string>(m_ModelPath), aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded);

			bool ret = false;
			// If the import failed, report it
			if (scene)
			{
				m_GlobalInverseTransform = scene->mRootNode->mTransformation;
				m_GlobalInverseTransform.Inverse();

				for (uint i = 0; i < scene->mNumMeshes; ++i)
				{
					LoadBones(scene->mMeshes[i]);
				}

				// Now we can access the file's contents.
				logInfo("Import of " + string_cast<std::string>(m_ModelPath) + " succeeded.");
			}
			else 
			{
				logInfo(importer.GetErrorString());
			}

			// We're done. Everything will be cleaned up by the importer destructor
			return ret;
		}

		bool Add3DAnimFromFile(const std::wstring& fileName)
		{
			// Check if file exists
			std::ifstream fin(fileName.c_str());
			if (!fin.fail())
			{
				fin.close();
			}
			else
			{
				MessageBoxA(NULL, ("Couldn't open file: " + string_cast<std::string>(fileName)).c_str(), "ERROR", MB_OK | MB_ICONEXCLAMATION);
				return false;
			}

			uint sceneIndex = static_cast<uint>(_importers.size());
			_importers.push_back(new Assimp::Importer);
			_scenes.push_back(_importers[sceneIndex]->ReadFile(string_cast<std::string>(fileName), aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded));
			// If the import failed, report it
			if (_scenes[sceneIndex])
			{
				logInfo(_importers[sceneIndex]->GetErrorString());
				m_GlobalInverseTransform = _scenes[sceneIndex]->mRootNode->mTransformation;
				m_GlobalInverseTransform.Inverse();

				if (_scenes[sceneIndex]->mNumAnimations > 0)
					BakeAnimation(sceneIndex);
			}
			else 
			{
				logInfo(_importers[sceneIndex]->GetErrorString());
			}

			return true;
		}

		uint GetMaxIndex()
		{
			return static_cast<uint>(_scenes.size());
		}

	
	};
}
