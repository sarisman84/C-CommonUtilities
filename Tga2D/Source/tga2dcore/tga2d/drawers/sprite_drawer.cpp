#include "stdafx.h"

#include <tga2d/drawers/sprite_drawer.h>
#include <tga2d/d3d/direct_3d.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/texture/texture_manager.h>
#include <tga2d/shaders/sprite_shader.h>
#include <tga2d/engine.h>

using namespace Tga2D;

constexpr size_t BATCH_SIZE = 1024;

CSpriteBatchScope::~CSpriteBatchScope()
{
	if (mySpriteDrawer != nullptr)
	{
		UnMapAndRender();
		mySpriteDrawer->EndBatch();
		mySpriteDrawer = nullptr;
	}
}

void CSpriteBatchScope::Draw(const SSpriteInstanceData& aInstance)
{
	Draw(&aInstance, 1);
};

void CSpriteBatchScope::Draw(const SSpriteInstanceData* aInstances, size_t aInstanceCount)
{
	assert(myInstanceCount < BATCH_SIZE);

	for (int i = 0; i < aInstanceCount; i++)
	{
		const SSpriteInstanceData& instance = aInstances[i];

		if (instance.myIsHidden)
			continue;

		SSpriteShaderInstanceData& shaderInstance = myInstanceData[myInstanceCount];

		shaderInstance.myPosition.x = (instance.myPosition.x * 2.0f) - 1.0f;
		shaderInstance.myPosition.y = -(instance.myPosition.y * 2.0f) + 1.0f;

		shaderInstance.myPivot.x = instance.myPivot.x;
		shaderInstance.myPivot.y = instance.myPivot.y;
		shaderInstance.myRotationAndSize.x = instance.myRotation;

		shaderInstance.myRotationAndSize.y = (instance.mySize.x * 2) * instance.mySizeMultiplier.x;
		shaderInstance.myRotationAndSize.z = (instance.mySize.y * 2) * instance.mySizeMultiplier.y;

		shaderInstance.myRotationAndSize.w = 0;

		shaderInstance.myUVRect.x = instance.myTextureRect.myStartX;
		shaderInstance.myUVRect.y = instance.myTextureRect.myStartY;
		shaderInstance.myUVRect.z = instance.myTextureRect.myEndX;
		shaderInstance.myUVRect.w = instance.myTextureRect.myEndY;

		shaderInstance.myUV.x = instance.myUV.x;
		shaderInstance.myUV.y = instance.myUV.y;
		shaderInstance.myUV.z = instance.myUVScale.x;
		shaderInstance.myUV.w = instance.myUVScale.y;

		shaderInstance.myColor.x = instance.myColor.myR;
		shaderInstance.myColor.y = instance.myColor.myG;
		shaderInstance.myColor.z = instance.myColor.myB;
		shaderInstance.myColor.w = instance.myColor.myA;

		myInstanceCount++;
		if (myInstanceCount >= BATCH_SIZE)
		{
			UnMapAndRender();
			Map();
		}
	}
};

void CSpriteBatchScope::UnMapAndRender()
{
	assert(mySpriteDrawer);

	ID3D11DeviceContext* context = mySpriteDrawer->myEngine->GetContext();

	context->Unmap(mySpriteDrawer->myInstanceBuffer.Get(), 0);
	context->DrawInstanced(6, (UINT)myInstanceCount, 0, 0);

	mySpriteDrawer->myEngine->IncreaseObjectRenderCount();

	myInstanceData = nullptr;
	myInstanceCount = 0;
}

void CSpriteBatchScope::Map()
{
	assert(mySpriteDrawer);
	assert(myInstanceCount == 0);

	D3D11_MAPPED_SUBRESOURCE mappedObjectResource;
	HRESULT result = mySpriteDrawer->myEngine->GetContext()->Map(mySpriteDrawer->myInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedObjectResource);
	if (FAILED(result))
	{
		INFO_PRINT("Error in rendering!");
		return;
	}

	myInstanceData = static_cast<SSpriteShaderInstanceData*>(mappedObjectResource.pData);
}

CSpriteDrawer::CSpriteDrawer(CDirectEngine* aEngine)
	: myEngine(aEngine)
{
}

CSpriteDrawer::~CSpriteDrawer()
{
}

void CSpriteDrawer::Init()
{
	D3D11_BUFFER_DESC objectBufferDesc;
	objectBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	objectBufferDesc.ByteWidth = sizeof(SSpriteShaderInstanceData) * BATCH_SIZE;
	objectBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	objectBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	objectBufferDesc.MiscFlags = 0;
	objectBufferDesc.StructureByteStride = 0;

	HRESULT result;
	result = myEngine->GetDevice()->CreateBuffer(&objectBufferDesc, nullptr, myInstanceBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		ERROR_PRINT("%s", "Object Buffer error");
		return;
	}
	myIsLoaded = InitShaders() && CreateBuffer();
}

bool CSpriteDrawer::InitShaders()
{
	myDefaultShader = std::make_unique<CSpriteShader>();
	if (!myDefaultShader->Init())
	{
		return false;
	}

	return true;
}

bool CSpriteDrawer::CreateBuffer()
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	float startSize = 1.0f;
	float theZ = 0.0f;
	myVertices[0].X = -0;
	myVertices[0].Y = -startSize;
	myVertices[0].Z = theZ;
	myVertices[0].texX = 0.0f;
	myVertices[0].texY = 1.0f;
	myVertices[0].myVertexIndex = 0;

	myVertices[1].X = -0;
	myVertices[1].Y = -0;
	myVertices[1].Z = theZ;
	myVertices[1].texX = 0.0f;
	myVertices[1].texY = 0.0f;
	myVertices[1].myVertexIndex = 1;

	myVertices[2].X = startSize;
	myVertices[2].Y = -startSize;
	myVertices[2].Z = theZ;
	myVertices[2].texX = 1.0f;
	myVertices[2].texY = 1.0f;
	myVertices[2].myVertexIndex = 2;

	myVertices[3].X = startSize;
	myVertices[3].Y = -0;
	myVertices[3].Z = theZ;
	myVertices[3].texX = 1.0f;
	myVertices[3].texY = 0.0f;
	myVertices[3].myVertexIndex = 3;

	myVertices[4].X = startSize;
	myVertices[4].Y = -startSize;
	myVertices[4].Z = theZ;
	myVertices[4].texX = 1.0f;
	myVertices[4].texY = 1.0f;
	myVertices[4].myVertexIndex = 4;

	myVertices[5].X = -0;
	myVertices[5].Y = -0;
	myVertices[5].Z = theZ;
	myVertices[5].texX = 0.0f;
	myVertices[5].texY = 0.0f;
	myVertices[5].myVertexIndex = 5;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(SVertexInstanced) * 6;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = myVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT hr = myEngine->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, myVertexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		ERROR_PRINT("%s", "Buffer error");
		return false;
	}

	return true;
}

CSpriteBatchScope CSpriteDrawer::BeginBatch(const SSpriteSharedData& aSharedData)
{
	assert(myIsLoaded);
	assert(!myIsInBatch);
	myIsInBatch = true;

	if (aSharedData.myCustomShader)
	{
		aSharedData.myCustomShader->PrepareRender(aSharedData);
	}
	else
	{
		myDefaultShader->PrepareRender(aSharedData);
	}

	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(SVertexInstanced);
	strides[1] = sizeof(SSpriteShaderInstanceData);

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = myVertexBuffer.Get();
	bufferPointers[1] = myInstanceBuffer.Get();

	myEngine->GetContext()->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	CSpriteBatchScope scope(*this);
	scope.Map();

	return scope;
}

void CSpriteDrawer::EndBatch()
{
	assert(myIsInBatch);
	myIsInBatch = false;
}