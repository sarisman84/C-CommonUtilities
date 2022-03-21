#include "stdafx.h"
#include "ForwardRenderer.h"

#include <vector>

#include "AmbientLight.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include <tga2d/graphics/DX11.h>
#include "Light.h"
#include <tga2d/model/Model.h>
#include <tga2d/model/ModelInstance.h>
#include <tga2d/model/AnimatedModelInstance.h>
#include "PointLight.h"
#include <tga2d/shaders/ShaderCommon.h>

using namespace Tga2D;

ForwardRenderer::ForwardRenderer()
{
}

ForwardRenderer::~ForwardRenderer()
{
}

bool ForwardRenderer::Init()
{
	HRESULT result = S_FALSE;
	
	ID3D11Device* Device = DX11::Device;

	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(ObjectBufferData);
	result = Device->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer);

	// GPU
	D3D11_BUFFER_DESC matrixVertexBufferDesc;
	matrixVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixVertexBufferDesc.ByteWidth = sizeof(Matrix4x4f) * 64;
	matrixVertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixVertexBufferDesc.MiscFlags = 0;
	matrixVertexBufferDesc.StructureByteStride = 0;

	result = DX11::Device->CreateBuffer(&matrixVertexBufferDesc, NULL, &myBoneBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
	//Start Shader
	std::string vsData;
	if (!DX11::LoadVertexShader("Shaders/VertexShaderVS.cso", &myModelVertexShader, vsData))
		return false;
	if (!DX11::LoadVertexShader("Shaders/AnimatedVertexShaderVS.cso", &myAnimatedModelVertexShader, vsData))
		return false;
	if (!DX11::LoadPixelShader("Shaders/PixelShaderPS.cso", &myModelPixelShader))
		return false;
	//End Shader
	
	//Start Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 3, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	result = DX11::Device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &myModelInputLayout);
	if (FAILED(result))
	{
		return false;
	}
	//End Layout

	return true;
}

bool ForwardRenderer::SetEnvironmentMap(
	const AmbientLight& aAmbientLight)
{
	DX11::Context->PSSetShaderResources(0, 1, aAmbientLight.GetCubemap());
	return true;
}


void ForwardRenderer::Render(const ModelInstance& aModelInstance)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;
	
	Model* model = aModelInstance.GetModel();
	const Model::MeshData* meshDatas = model->GetMeshDatas();
	int meshCount = model->GetMeshCount();

	for (int j = 0; j < meshCount; j++)
	{
		const Model::MeshData& meshData = meshDatas[j];

		myObjectBufferData.myToWorld = aModelInstance.GetTransform().GetMatrix();

		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = DX11::Context->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if (FAILED(result))
		{
			// Boom!
		}

		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::Context->Unmap(myObjectBuffer, 0);

		DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11::Context->IASetInputLayout(myModelInputLayout);

		DX11::Context->IASetVertexBuffers(0, 1, &meshData.myVertexBuffer, &meshData.myStride, &meshData.myOffset);
		DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		DX11::Context->VSSetConstantBuffers((int)ConstantBufferSlot::Object, 1, &myObjectBuffer);
		DX11::Context->PSSetConstantBuffers((int)ConstantBufferSlot::Object, 1, &myObjectBuffer);

		DX11::Context->VSSetShader(myModelVertexShader, nullptr, 0);
		DX11::Context->GSSetShader(nullptr, nullptr, 0);
		DX11::Context->PSSetShader(myModelPixelShader, nullptr, 0);

		{
			const TextureResource* const* textures = aModelInstance.GetTextures(j);
			ID3D11ShaderResourceView* resourceViews[4];
			int i = 0;
			while (i < 4 && textures[i] != nullptr)
			{
				resourceViews[i] = textures[i]->GetShaderResourceView();
				i++;
			}

			DX11::Context->PSSetShaderResources(1, i, resourceViews);
		}

		DX11::RenderStateManager->SetSamplerState(SamplerFilter::Trilinear, SamplerAddressMode::Wrap);

		DX11::LogDrawCall();
		DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
	}
}

void Tga2D::ForwardRenderer::Render(const AnimatedModelInstance& aModelInstance)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	Model* model = aModelInstance.myModel.get();
	const Model::MeshData* meshDatas = model->GetMeshDatas();
	int meshCount = model->GetMeshCount();

	for (int j = 0; j < meshCount; j++)
	{
		const Model::MeshData& meshData = meshDatas[j];

		myObjectBufferData.myToWorld = aModelInstance.GetTransform().GetMatrix();
		D3D11_MAPPED_SUBRESOURCE mappedVResource;
		result = DX11::Context->Map(myBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVResource);
		if (FAILED(result))
		{
			//INFO_PRINT("Error in rendering!");
			return;
		}
		char* dataVPtr = (char*)mappedVResource.pData;
		if (aModelInstance.myBones)
		{
			memcpy(dataVPtr, aModelInstance.myBones, sizeof(Matrix4x4f) * MAX_ANIMATION_BONES);
		}

		DX11::Context->Unmap(myBoneBuffer, 0);
		DX11::Context->VSSetConstantBuffers((int)ConstantBufferSlot::Bones, 1, &myBoneBuffer);

		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = DX11::Context->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if (FAILED(result))
		{
			// Boom!
		}

		memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
		DX11::Context->Unmap(myObjectBuffer, 0);

		DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11::Context->IASetInputLayout(myModelInputLayout);

		DX11::Context->IASetVertexBuffers(0, 1, &meshData.myVertexBuffer, &meshData.myStride, &meshData.myOffset);
		DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		DX11::Context->VSSetConstantBuffers((int)ConstantBufferSlot::Object, 1, &myObjectBuffer);
		DX11::Context->PSSetConstantBuffers((int)ConstantBufferSlot::Object, 1, &myObjectBuffer);

		DX11::Context->VSSetShader(myAnimatedModelVertexShader, nullptr, 0);
		DX11::Context->GSSetShader(nullptr, nullptr, 0);
		DX11::Context->PSSetShader(myModelPixelShader, nullptr, 0);

		{
			const TextureResource* const* textures = aModelInstance.myTextures[j];
			ID3D11ShaderResourceView* resourceViews[4];
			int i = 0;
			while (i < 4 && textures[i] != nullptr)
			{
				resourceViews[i] = textures[i]->GetShaderResourceView();
				i++;
			}

			DX11::Context->PSSetShaderResources(1, i, resourceViews);
		}

		DX11::RenderStateManager->SetSamplerState(SamplerFilter::Trilinear, SamplerAddressMode::Wrap);

		DX11::LogDrawCall();
		DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
	}
}
