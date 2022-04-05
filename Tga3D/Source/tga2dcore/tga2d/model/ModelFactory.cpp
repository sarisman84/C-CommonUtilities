#include "stdafx.h"
#include <assimp/cimport.h>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "ModelFactory.h"

#include <fstream>

#include <tga2d/graphics/DX11.h>
#include <tga2d/util/StringCast.h>
#include <tga2d/model/Model.h>
#include <tga2d/model/ModelInstance.h>
#include <tga2d/graphics/Vertex.h>
#include <tga2d/math/matrix4x4.h>
#include <tga2d/texture/texture.h>
#include <tga2d/texture/TextureManager.h>

#include <DDSTextureLoader/DDSTextureLoader11.h>

using namespace Tga2D;

ModelFactory* ModelFactory::myInstance = nullptr;
#pragma comment (lib, "assimp-vc142-mt.lib")

#define TEXTURE_SET_0 0
#define TEXTURE_SET_1 1
#define TEXTURE_SET_2 2
#define TEXTURE_SET_3 3

#define VERTEX_COLOR_SET_0 0
#define VERTEX_COLOR_SET_1 1
#define VERTEX_COLOR_SET_2 2
#define VERTEX_COLOR_SET_3 3

#define NUM_BONES_PER_VERTEX 4

using namespace Tga2D;

Matrix4x4f ConvertToEngineMatrix33(const aiMatrix3x3& AssimpMatrix)
{
    Matrix4x4f mat;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            mat(1 + i, 1 + j) = AssimpMatrix[i][j];
        }
    }

    return mat;
}

// constructor from Assimp matrix
Matrix4x4f ConvertToEngineMatrix4x4f(const aiMatrix4x4& AssimpMatrix)
{
    Matrix4x4f mat;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mat(1 + i, 1 + j) = AssimpMatrix[i][j];
        }
    }
    return mat;
}

inline bool operator <(const aiString& aLhs, const aiString& aRhs)
{
    return strcmp(aLhs.data, aRhs.data) < 0;
}

struct VertexBoneData
{
    unsigned int IDs[NUM_BONES_PER_VERTEX];
    float Weights[NUM_BONES_PER_VERTEX];

    VertexBoneData()
    {
        Reset();
    };

    void Reset()
    {
        memset(IDs, 0, sizeof(IDs));
        memset(Weights, 0, sizeof(Weights));
    }

    void AddBoneData(unsigned int BoneID, float Weight)
    {
        for (unsigned int i = 0; i < sizeof(IDs) / sizeof(IDs[0]); i++) {
            if (Weights[i] == 0.0) {
                IDs[i] = BoneID;
                Weights[i] = Weight;
                return;
            }
        }

        // should never get here - more bones than we have space for
        //assert(0);
    }
};

bool LoadVertices(aiMesh* fbxMesh, MeshData& someMeshData)
{
    someMeshData.myVertices.resize(fbxMesh->mNumVertices);

    std::vector<VertexBoneData> collectedBoneData;
    if (fbxMesh->HasBones())
    {
        collectedBoneData.resize(fbxMesh->mNumVertices);

        unsigned int BoneIndex = 0;
        std::map<aiString, unsigned int> boneMapping;
        int numBones = 0;
        for (unsigned int i = 0; i < fbxMesh->mNumBones; i++)
        {
            aiString& boneName = fbxMesh->mBones[i]->mName;
            if (boneMapping.find(boneName) == boneMapping.end())
            {
                // Allocate an index for a new bone
                BoneIndex = numBones;
                numBones++;

                Matrix4x4f NodeTransformation = ConvertToEngineMatrix4x4f(fbxMesh->mBones[i]->mOffsetMatrix);

                boneMapping[boneName] = BoneIndex;
            }
            else {
                BoneIndex = boneMapping[boneName];
            }

            for (unsigned int j = 0; j < fbxMesh->mBones[i]->mNumWeights; j++)
            {
                unsigned int VertexID = fbxMesh->mBones[i]->mWeights[j].mVertexId;
                float Weight = fbxMesh->mBones[i]->mWeights[j].mWeight;
                collectedBoneData[VertexID].AddBoneData(BoneIndex, Weight);
            }
        }
    }

    Vector3f minExtents = Vector3f::Zero;
    Vector3f maxExtents = Vector3f::Zero;

    for (unsigned int i = 0; i < fbxMesh->mNumVertices; i++)
    {
        someMeshData.myVertices[i] = {};

        if (fbxMesh->HasPositions())
        {
            aiVector3D& aiPosition = fbxMesh->mVertices[i];
            Vector4f position = Vector4f(aiPosition.x, aiPosition.y, aiPosition.z, 1);
            someMeshData.myVertices[i].Position = position;

            if (position.X > maxExtents.X)
                maxExtents.X = position.X;
            if (position.Y > maxExtents.Y)
                maxExtents.Y = position.Y;
            if (position.Z > maxExtents.Z)
                maxExtents.Z = position.Z;

            if (position.X < minExtents.X)
                minExtents.X = position.X;
            if (position.Y < minExtents.Y)
                minExtents.Y = position.Y;
            if (position.Z < minExtents.Z)
                minExtents.Z = position.Z;
        }
        else
        {
            ERROR_PRINT("%s", "Model must at least have vertex positions, please consult the artists about their export process");
        }

        if (fbxMesh->HasBones())
        {
            VertexBoneData& boneData = collectedBoneData[i];

            someMeshData.myVertices[i].Bones = (Vector4f((float)boneData.IDs[0], (float)boneData.IDs[1], (float)boneData.IDs[2], (float)boneData.IDs[3]));
            someMeshData.myVertices[i].Weights = (Vector4f(boneData.Weights[0], boneData.Weights[1], boneData.Weights[2], boneData.Weights[3]));
        }

        if (fbxMesh->HasTextureCoords(TEXTURE_SET_0))
        {
            someMeshData.myVertices[i].UVs[0] = (Vector2f(fbxMesh->mTextureCoords[TEXTURE_SET_0][i].x, fbxMesh->mTextureCoords[TEXTURE_SET_0][i].y));
        }

        if (fbxMesh->HasVertexColors(VERTEX_COLOR_SET_0))
        {
            someMeshData.myVertices[i].VertexColors[0] = (Vector4f(fbxMesh->mColors[VERTEX_COLOR_SET_0][i].r, fbxMesh->mColors[VERTEX_COLOR_SET_0][i].g, fbxMesh->mColors[VERTEX_COLOR_SET_0][i].b, fbxMesh->mColors[VERTEX_COLOR_SET_0][i].a));
        }

        if (fbxMesh->HasNormals())
        {
            aiVector3D& normal = fbxMesh->mNormals[i];
            someMeshData.myVertices[i].Normal = Vector3f(normal.x, normal.y, normal.z);
        }

        if (fbxMesh->HasTangentsAndBitangents())
        {
            aiVector3D& tangent = fbxMesh->mTangents[i];
            someMeshData.myVertices[i].Tangent = Vector3f(tangent.x, tangent.y, tangent.z);

            aiVector3D& binormal = fbxMesh->mBitangents[i];
            someMeshData.myVertices[i].Binormal = Vector3f(binormal.x, binormal.y, binormal.z);
        }
    }

    const Vector3f extentsCenter = 0.5f * (minExtents + maxExtents);
    const Vector3f boxExtents = 0.5f * (maxExtents - minExtents);
    const float myBoxSphereRadius = FMath::Max(boxExtents.X, FMath::Max(boxExtents.Y, boxExtents.Z));
    someMeshData.myBounds = { myBoxSphereRadius, boxExtents, extentsCenter };

    return true;
}

bool ModelFactory::InitUnitCube()
{
    // First we make a cube.

	// Watch the winding! DX defaults to Clockwise.
	// Assume the winding as if you're viewing the face head on.
	// +Y up, +X right, +Z Forward
    std::vector<Vertex> mdlVertices = {

    	// Front
        {
        	50.0f, -50.0f, 50.0f,
            0, 0, 1,
            1, 0, 0,
            0, -1, 0,
        	1, 1, 1, 1,
        	0, 1
        },
        {
        	50.0f, 50.0f, 50.0f,
            0, 0, 1,
            1, 0, 0,
            0, -1, 0,
        	1, 1, 1, 1,
        	0, 0
        },
        {
        	-50.0f, 50.0f, 50.0f,
            0, 0, 1,
            1, 0, 0,
            0, -1, 0,
        	1, 1, 1, 1,
        	1, 0
        },
        {
        	-50.0f, -50.0f, 50.0f,
            0, 0, 1,
            1, 0, 0,
            0, -1, 0,
        	1, 1, 1, 1,
        	1, 1
        },

    	// Left
        {
        	-50.0f, -50.0f, 50.0f,
            -1, 0, 0,
            0, 0, 1,
            0, -1, 0,
        	1, 0, 0, 1,
        	0, 1
        },
        {
        	-50.0f, 50.0f, 50.0f,
            -1, 0, 0,
            0, 0, 1,
            0, -1, 0,
        	1, 0, 0, 1,
        	0, 0
        },       
        {
        	-50.0f, 50.0f, -50.0f,
            -1, 0, 0,
            0, 0, 1,
            0, -1, 0,
        	1, 0, 0, 1,
        	1, 0
        },
        {
        	-50.0f, -50.0f, -50.0f,
            -1, 0, 0,
            0, 0, 1,
            0, -1, 0,
        	1, 0, 0, 1,
        	1, 1
        },      

    	// Back
        {
        	-50.0f, -50.0f, -50.0f,
            0, 0, -1,
            1, 0, 0,
            0, -1, 0,
        	0, 1, 0, 1,
        	0, 1
        },
        {
        	-50.0f, 50.0f, -50.0f,
            0, 0, -1,
            1, 0, 0,
            0, -1, 0,
        	0, 1, 0, 1,
        	0, 0
        },      
        {
        	50.0f, 50.0f, -50.0f,
            0, 0, -1,
            1, 0, 0,
            0, -1, 0,
        	0, 1, 0, 1,
        	1, 0
        },
    	{
    		50.0f, -50.0f, -50.0f,
            0, 0, -1,
            1, 0, 0,
            0, -1, 0,
    		0, 1, 0, 1,
    		1, 1
    	},       

    	// Right
        {
        	50.0f, -50.0f, -50.0f,
            1, 0, 0,
            0, 0, -1,
            0, -1, 0,
        	0, 0, 1, 1,
        	0, 1
        },
        {
        	50.0f, 50.0f, -50.0f,
            1, 0, 0,
            0, 0, -1,
            0, -1, 0,
        	0, 0, 1, 1,
        	0, 0
        },
        {
        	50.0f, 50.0f, 50.0f,
            1, 0, 0,
            0, 0, -1,
            0, -1, 0,
        	0, 0, 1, 1
        	, 1, 0
        },
        {
        	50.0f, -50.0f, 50.0f,
            1, 0, 0,
            0, 0, -1,
            0, -1, 0,
        	0, 0, 1, 1,
        	1, 1
        },       

    	// Top
        {
        	50.0f, 50.0f, 50.0f,
        	0, 1, 0,
        	1, 0, 0,
        	0, 0, 1,
        	1, 1, 0, 1,
        	0, 1
        },
        {
        	50.0f, 50.0f, -50.0f,
        	0, 1, 0,
        	1, 0, 0,
        	0, 0, 1,
        	1, 1, 0, 1,
        	0, 0
        },
        {
        	-50.0f, 50.0f, -50.0f,
        	0, 1, 0,
        	1, 0, 0,
        	0, 0, 1,
        	1, 1, 0, 1,
        	1, 0
        },
        {
        	-50.0f, 50.0f, 50.0f,
        	0, 1, 0,
        	1, 0, 0,
        	0, 0, 1,
        	1, 1, 0, 1,
        	1, 1
        },

    	// Bottom
        {
        	-50.0f, -50.0f, 50.0f,
        	0, -1, 0,
        	1, 0, 0,
        	0, 0, 1,
        	1, 0, 1, 1,
        	0, 1
        },
        {
        	-50.0f, -50.0f, -50.0f,
        	0, -1, 0,
        	1, 0, 0,
        	0, 0, 1,
        	1, 0, 1, 1,
        	0, 0
        },
        {
        	50.0f, -50.0f, -50.0f,
        	0, -1, 0,
        	1, 0, 0,
        	0, 0, 1,
        	1, 0, 1, 1,
        	1, 0
        },
        {
        	50.0f, -50.0f, 50.0f,
        	0, -1, 0,
        	1, 0, 0,
        	0, 0, 1,
        	1, 0, 1, 1
        	, 1, 1
        },
    };

    std::vector<unsigned int> mdlIndices =
    {
    	0, 1, 2,        /* |/ */
    	0, 2, 3,        /* /| */
    	4, 5, 6,        /* |/ */
    	4, 6, 7,        /* /| */
    	8, 9, 10,       /* |/ */
    	8, 10, 11,      /* /| */
    	12, 13, 14,     /* |/ */
    	12, 14, 15,     /* /| */
    	16, 17, 18,     /* |/ */
    	16, 18, 19,     /* /| */
    	20, 21, 22,     /* |/ */
    	20, 22, 23      /* /| */
    };

    //const Vector3f extentsCenter = 0.5f * (minExtents + maxExtents);
    //const Vector3f boxExtents = 0.5f * (maxExtents - minExtents);
    //const float myBoxSphereRadius = FMath::Max(boxExtents.X, FMath::Max(boxExtents.Y, boxExtents.Z));

    HRESULT result;

    D3D11_BUFFER_DESC vertexBufferDesc{};
    vertexBufferDesc.ByteWidth = static_cast<UINT>(mdlVertices.size()) * static_cast<UINT>(sizeof(Vertex));
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
    vertexSubresourceData.pSysMem = &mdlVertices[0];

    ID3D11Buffer* vertexBuffer;
    result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc{};
    indexBufferDesc.ByteWidth = static_cast<UINT>(mdlIndices.size()) * static_cast<UINT>(sizeof(unsigned int));
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexSubresourceData{};
    indexSubresourceData.pSysMem = &mdlIndices[0];

    ID3D11Buffer* indexBuffer;
    result = DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    std::shared_ptr<Model> model = std::make_shared<Model>();

    Model::MeshData meshData = {};
    meshData.myNumberOfVertices = static_cast<UINT>(mdlVertices.size());
    meshData.myNumberOfIndices = static_cast<UINT>(mdlIndices.size());
    meshData.myStride = sizeof(Vertex);
    meshData.myOffset = 0;
    meshData.myVertexBuffer = vertexBuffer;
    meshData.myIndexBuffer = indexBuffer;
    //meshData.myBounds = { myBoxSphereRadius, boxExtents, extentsCenter };
    model->Init(&meshData, 1, false, L"Cube");
    myModels.push_back(model);
    myLoadedModels.insert(std::pair<std::wstring, Model*>(L"Cube", model.get()));

    return true;
}

bool ModelFactory::InitUnitPlane()
{
    std::vector<Vertex> mdlVertices;
    std::vector<unsigned int> mdlIndices;

    mdlVertices.push_back({
	    -50.0f, 0.0f, 50.0f,
	    0, 1, 0,
	    1, 0, 0,
	    0, 0, 1,
	    1, 1, 1, 1,
	    0, 0
        });

    mdlVertices.push_back({
	    50.0f, 0.0f, 50.0f,
        0, 1, 0,
        1, 0, 0,
        0, 0, 1,
	    1, 1, 1, 1,
	    1, 0
        });

    mdlVertices.push_back({
		50.0f, 0.0f, -50.0f,
        0, 1, 0,
        1, 0, 0,
        0, 0, 1,
		1, 1, 1, 1,
		1, 1
		});

    mdlVertices.push_back({
	    -50.0f, 0.0f, -50.0f,
        0, 1, 0,
        1, 0, 0,
        0, 0, 1,
	    1, 1, 1, 1,
	    0, 1
        });


    mdlIndices = { 0, 1, 2, 0, 2, 3 };

    //const Vector3f extentsCenter = 0.5f * (minExtents + maxExtents);
    //const Vector3f boxExtents = 0.5f * (maxExtents - minExtents);
    //const float myBoxSphereRadius = FMath::Max(boxExtents.X, FMath::Max(boxExtents.Y, boxExtents.Z));

    HRESULT result;

    D3D11_BUFFER_DESC vertexBufferDesc{};
    vertexBufferDesc.ByteWidth = static_cast<UINT>(mdlVertices.size()) * static_cast<UINT>(sizeof(Vertex));
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
    vertexSubresourceData.pSysMem = &mdlVertices[0];

    ID3D11Buffer* vertexBuffer;
    result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc{};
    indexBufferDesc.ByteWidth = static_cast<UINT>(mdlIndices.size()) * static_cast<UINT>(sizeof(unsigned int));
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexSubresourceData{};
    indexSubresourceData.pSysMem = &mdlIndices[0];

    ID3D11Buffer* indexBuffer;
    result = DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
    if (FAILED(result))
    {
        return false;
    }
	
    std::shared_ptr<Model> model = std::make_shared<Model>();

    Model::MeshData meshData = {};
    meshData.myNumberOfVertices = static_cast<UINT>(mdlVertices.size());
    meshData.myNumberOfIndices = static_cast<UINT>(mdlIndices.size());
    meshData.myStride = sizeof(Vertex);
    meshData.myOffset = 0;
    meshData.myVertexBuffer = vertexBuffer;
    meshData.myIndexBuffer = indexBuffer;

    //meshData.myBounds = { myBoxSphereRadius, boxExtents, extentsCenter };
    model->Init(&meshData, 1, false, L"Plane");
    myModels.push_back(model);
    myLoadedModels.insert(std::pair<std::wstring, Model*>(L"Plane", model.get()));
	
    return true;
}

bool ModelFactory::InitPrimitives()
{
    if (!InitUnitCube())
        return false;

    if (!InitUnitPlane())
        return false;

    return true;
}

ModelFactory::ModelFactory()
{
    InitPrimitives();
	myInstance = this;
}

ModelFactory::~ModelFactory()
{
	myInstance = nullptr;
}

template<typename ModelInstanceType>
void AssignMaterials(const std::wstring& someFilePath, Model* Mdl, ModelInstanceType& MI)
{
    std::wstring baseFileName = someFilePath;
    baseFileName.erase(baseFileName.end() - 4, baseFileName.end());
    std::wstring path = someFilePath.substr(0, someFilePath.find_last_of(L"/\\") + 1);

    for (int i = 0; i < Mdl->GetMeshCount(); i++)
    {
        Tga2D::Engine& engine = *Tga2D::Engine::GetInstance();

        std::wstring materialFileName = path + string_cast<std::wstring>(Mdl->GetMaterialName(i));

        TextureResource* albedoTexture = nullptr;

        if (albedoTexture == nullptr)
        {
            std::wstring fnAlbedo = materialFileName + L"_C.dds";
            albedoTexture = engine.GetTextureManager().TryGetTexture(fnAlbedo.c_str());
        }

        if (albedoTexture == nullptr)
        {
            std::wstring fnAlbedo = materialFileName + L"_D.dds";
            albedoTexture = engine.GetTextureManager().TryGetTexture(fnAlbedo.c_str());
        }

        if (albedoTexture == nullptr)
        {
            std::wstring fnAlbedo = baseFileName + L"_C.dds";
            albedoTexture = engine.GetTextureManager().TryGetTexture(fnAlbedo.c_str());
        }

        if (albedoTexture == nullptr)
        {
            std::wstring fnAlbedo = baseFileName + L"_D.dds";
            albedoTexture = engine.GetTextureManager().TryGetTexture(fnAlbedo.c_str());
        }

        if (albedoTexture == nullptr)
            albedoTexture = engine.GetTextureManager().GetTexture(L"Textures/T_Default_BC.dds");

        MI.SetTexture(i, 0, albedoTexture);

        TextureResource* normalTexture = nullptr;

        if (normalTexture == nullptr)
        {
            std::wstring fnNormal = materialFileName + L"_N.dds";
            normalTexture = engine.GetTextureManager().TryGetTexture(fnNormal.c_str(), false);
        }

        if (normalTexture == nullptr)
        {
            std::wstring fnNormal = baseFileName + L"_N.dds";
            normalTexture = engine.GetTextureManager().TryGetTexture(fnNormal.c_str(), false);
        }

        if (normalTexture == nullptr)
            normalTexture = engine.GetTextureManager().GetTexture(L"Textures/T_Default_N.dds");

        MI.SetTexture(i, 1, normalTexture);

        TextureResource* materialTexture = nullptr;

        if (materialTexture == nullptr)
        {
            std::wstring fnMaterial = materialFileName + L"_M.dds";
            materialTexture = engine.GetTextureManager().TryGetTexture(fnMaterial.c_str(), false);
        }

        if (materialTexture == nullptr)
        {
            std::wstring fnMaterial = baseFileName + L"_M.dds";
            materialTexture = engine.GetTextureManager().TryGetTexture(fnMaterial.c_str(), false);
        }

        if (materialTexture == nullptr)
            materialTexture = engine.GetTextureManager().GetTexture(L"Textures/T_Default_M.dds");

        MI.SetTexture(i, 2, materialTexture);
    }
}

ModelInstance ModelFactory::GetModel(const std::wstring& someFilePath)
{
	// This needs to be moved to separate memory structures at some point.
    ModelInstance MI;

    std::shared_ptr<Model> Mdl = GetModelInternal(someFilePath);
    if (!Mdl)
        return MI;

    MI.Init(Mdl);

    AssignMaterials(someFilePath, Mdl.get(), MI);

    return MI;
}

AnimatedModelInstance ModelFactory::GetAnimatedModel(const std::wstring& someFilePath)
{
    // This needs to be moved to separate memory structures at some point.
    AnimatedModelInstance MI;

    std::shared_ptr<Model> Mdl = GetModelInternal(someFilePath);
    if (!Mdl || !Mdl->GetIsAnimated())
        return MI;

    MI.Init(Mdl);

    AssignMaterials(someFilePath, Mdl.get(), MI);

    return MI;
}

ModelInstance ModelFactory::GetUnitCube()
{
    return GetModel(L"Cube");
}

ModelInstance ModelFactory::GetUnitPlane()
{
    return GetModel(L"Plane");
}

#pragma optimize("", off)
bool ModelFactory::ModelHasMesh(const std::wstring& someFilePath)
{
    Assimp::Importer aiImporter;

    const aiScene* aiScene = aiImporter.ReadFile(string_cast<std::string>(someFilePath),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ConvertToLeftHanded |
        aiProcess_SortByPType
    );
    return aiScene->mMeshes ? true : false;
}

std::shared_ptr<Model> ModelFactory::LoadModel(const std::wstring& someFilePath)
{
    Assimp::Importer aiImporter;

    const aiScene* aiScene = aiImporter.ReadFile(string_cast<std::string>(someFilePath),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ConvertToLeftHanded |
        aiProcess_SortByPType
    );

	if(!aiScene)
	{
        return nullptr;
	}

    std::shared_ptr<Model> model = std::make_shared<Model>();

    Model::MeshData modelMeshDatas[MAX_MESHES_PER_MODEL];
    int meshCount = aiScene->mNumMeshes;
    bool isAnimated = false;

    for (int i = 0; i<meshCount; i++)
    { 
        aiMesh* sourceMesh = aiScene->mMeshes[i];
    
        MeshData meshData;
        LoadVertices(sourceMesh, meshData);

        for (unsigned int k = 0; k < sourceMesh->mNumFaces; k++)
        {
            for (unsigned int j = 0; j < sourceMesh->mFaces[k].mNumIndices; j++)
            {
                meshData.myIndices.push_back(sourceMesh->mFaces[k].mIndices[j]);
            }
        }

        HRESULT result; 
	
        D3D11_BUFFER_DESC vertexBufferDesc{};
        vertexBufferDesc.ByteWidth = static_cast<UINT>(meshData.myVertices.size()) * static_cast<UINT>(sizeof(Vertex));
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
        vertexSubresourceData.pSysMem = &meshData.myVertices[0];

        ID3D11Buffer* vertexBuffer;
        result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
        if (FAILED(result))
        {
            return nullptr;
        }

        D3D11_BUFFER_DESC indexBufferDesc{};
        indexBufferDesc.ByteWidth = static_cast<UINT>(meshData.myIndices.size()) * static_cast<UINT>(sizeof(float)); // TODO: What :P Sizeof should be uint.
        indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA indexSubresourceData{};
        indexSubresourceData.pSysMem = &meshData.myIndices[0];

        ID3D11Buffer* indexBuffer;
        result = DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
        if (FAILED(result))
        {
            return nullptr;
        }

        Model::MeshData& modelMeshData = modelMeshDatas[i];
        modelMeshData.myNumberOfVertices = static_cast<UINT>(meshData.myVertices.size());
        modelMeshData.myNumberOfIndices = static_cast<UINT>(meshData.myIndices.size());
        modelMeshData.myStride = sizeof(Vertex);
        modelMeshData.myOffset = 0;
        modelMeshData.myVertexBuffer = vertexBuffer;
        modelMeshData.myIndexBuffer = indexBuffer;
        modelMeshData.myBounds = meshData.myBounds;
        modelMeshData.myName = sourceMesh->mName.C_Str();
        modelMeshData.myMaterialName = aiScene->mMaterials[sourceMesh->mMaterialIndex]->GetName().C_Str();
        isAnimated |= sourceMesh->HasBones();
    }

    model->Init(modelMeshDatas, meshCount, isAnimated, someFilePath);
    myModels.push_back(model);
    myLoadedModels.insert(std::pair<std::wstring, Model*>(someFilePath, model.get()));

    return model;
}

#pragma optimize("", on)

std::shared_ptr<Model> ModelFactory::GetModelInternal(const std::wstring& someFilePath)
{
    for (std::vector<Model>::size_type i = 0; i < myModels.size(); i++)
    {
        if (myModels[i]->GetPath() == someFilePath)
        {
            return myModels.at(i);
        }
    }
	
    return LoadModel(someFilePath);
}