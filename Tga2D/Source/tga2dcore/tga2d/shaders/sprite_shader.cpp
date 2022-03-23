#include "stdafx.h"
#include <tga2d/shaders/sprite_shader.h>
#include <tga2d/shaders/shader_common.h>
#include <tga2d/texture/texture.h>
#include <tga2d/texture/texture_manager.h>

#include <tga2d/render/render_object.h>
#include <d3dcommon.h>
#include <d3d11.h>


Tga2D::CSpriteShader::CSpriteShader()
	: CShader(CEngine::GetInstance())
{
	myCurrentDataIndex = -1;
	myBufferIndex = EShaderDataBufferIndex_1;
	myCurrentTextureCount = 0;
}

bool Tga2D::CSpriteShader::Init(const char* aVertex, const char* aPixel)
{
	if (!CreateShaders(aVertex, aPixel))
	{
		return false;
	}

	D3D11_BUFFER_DESC commonBufferDesc;
	commonBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	commonBufferDesc.ByteWidth = sizeof(Tga2D::Vector4f) * 64;
	commonBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	commonBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	commonBufferDesc.MiscFlags = 0;
	commonBufferDesc.StructureByteStride = 0;

	HRESULT result = CEngine::GetInstance()->GetDirect3D().GetDevice()->CreateBuffer(&commonBufferDesc, NULL, myCustomBuffer.ReleaseAndGetAddressOf());
	if (result != S_OK)
	{
		ERROR_PRINT("DX2D::CCustomShader::PostInit - Size missmatch between CPU and GPU(shader)");
		return false;
	}

	return true;
}

void Tga2D::CSpriteShader::SetDataBufferIndex(EShaderDataBufferIndex aBufferRegisterIndex)
{
	myBufferIndex = (unsigned char)aBufferRegisterIndex;
}

void Tga2D::CSpriteShader::SetShaderdataFloat4(Tga2D::Vector4f someData, EShaderDataID aID)
{
	if (aID > MAX_SHADER_DATA)
	{
		ERROR_PRINT("DX2D::CCustomShader::SetShaderdataFloat4() The id is bigger than allowed size");
		return;
	}
	myCustomData[aID] = someData;
	if (aID > myCurrentDataIndex)
	{
		myCurrentDataIndex = aID;
	}
}

void Tga2D::CSpriteShader::SetTextureAtRegister(Tga2D::CTexture* aTexture, EShaderTextureSlot aRegisterIndex)
{
	myBoundTextures[aRegisterIndex - 4] = SBoundTexture(aTexture, (unsigned char)aRegisterIndex);

	if (myCurrentTextureCount < (aRegisterIndex - 4) + 1)
	{
		myCurrentTextureCount = static_cast<unsigned char>((aRegisterIndex - 4) + 1);
	}
}

bool Tga2D::CSpriteShader::PrepareRender(const SSpriteSharedData& aSharedData)
{
	if (!CShader::PrepareRender(aSharedData))
		return false;

	CEngine& engine = *CEngine::GetInstance();

	ID3D11DeviceContext* context = engine.GetDirect3D().GetContext();

	ID3D11ShaderResourceView* textures[1 + EShaderMap::MAP_MAX];

	textures[0] = aSharedData.myTexture ? aSharedData.myTexture->myResource.Get() : engine.GetTextureManager().GetWhiteSquareTexture()->myResource.Get();

	for (unsigned short index = 0; index < EShaderMap::MAP_MAX; index++)
	{
		textures[1 + index] = engine.GetTextureManager().GetDefaultNormalMapResource();
		if (aSharedData.myMaps[index])
		{
			textures[1 + index] = aSharedData.myMaps[index]->myResource.Get();
		}
	}
	myDirect3dEngine->GetContext()->PSSetShaderResources(1, 1 + EShaderMap::MAP_MAX, textures);

	if (myCustomBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResourceCommon;
		Tga2D::Vector4f* dataPtrCommon;
		HRESULT result = context->Map(myCustomBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceCommon);
		if (FAILED(result))
		{
			return false;
		}

		dataPtrCommon = (Tga2D::Vector4f*)mappedResourceCommon.pData;
		for (int i = 0; i < myCurrentDataIndex + 1; i++)
		{
			dataPtrCommon[i].x = myCustomData[i].x;
			dataPtrCommon[i].y = myCustomData[i].y;
			dataPtrCommon[i].z = myCustomData[i].z;
			dataPtrCommon[i].w = myCustomData[i].w;
		}

		context->Unmap(myCustomBuffer.Get(), 0);
		context->VSSetConstantBuffers(myBufferIndex, 1, myCustomBuffer.GetAddressOf());
		context->PSSetConstantBuffers(myBufferIndex, 1, myCustomBuffer.GetAddressOf());
	}

	for (int i = 0; i < myCurrentTextureCount; i++)
	{
		ID3D11ShaderResourceView* customTextures[1];
		customTextures[0] = myBoundTextures[i].myTexture->myResource.Get();

		context->PSSetShaderResources(myBoundTextures[i].myIndex, 1, customTextures);
	}

	return true;
}

bool Tga2D::CSpriteShader::CreateInputLayout(ID3D10Blob* aVS)
{
	D3D11_INPUT_ELEMENT_DESC polygonLayout[9];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_UINT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 1;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TEXCOORD";
	polygonLayout[3].SemanticIndex = 2;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[3].InputSlot = 1;
	polygonLayout[3].AlignedByteOffset = 0;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[3].InstanceDataStepRate = 1;

	polygonLayout[4].SemanticName = "TEXCOORD";
	polygonLayout[4].SemanticIndex = 3;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[4].InputSlot = 1;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[4].InstanceDataStepRate = 1;

	polygonLayout[5].SemanticName = "TEXCOORD";
	polygonLayout[5].SemanticIndex = 4;
	polygonLayout[5].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[5].InputSlot = 1;
	polygonLayout[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[5].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[5].InstanceDataStepRate = 1;

	polygonLayout[6].SemanticName = "TEXCOORD";
	polygonLayout[6].SemanticIndex = 5;
	polygonLayout[6].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[6].InputSlot = 1;
	polygonLayout[6].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[6].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[6].InstanceDataStepRate = 1;

	polygonLayout[7].SemanticName = "TEXCOORD";
	polygonLayout[7].SemanticIndex = 6;
	polygonLayout[7].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[7].InputSlot = 1;
	polygonLayout[7].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[7].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[7].InstanceDataStepRate = 1;

	polygonLayout[8].SemanticName = "TEXCOORD";
	polygonLayout[8].SemanticIndex = 7;
	polygonLayout[8].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[8].InputSlot = 1;
	polygonLayout[8].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[8].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[8].InstanceDataStepRate = 1;

	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	HRESULT result = myDirect3dEngine->GetDevice()->CreateInputLayout(polygonLayout, numElements, aVS->GetBufferPointer(), aVS->GetBufferSize(), myLayout.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		ERROR_PRINT("%s", "Layout error");
	}
	return true;
}


