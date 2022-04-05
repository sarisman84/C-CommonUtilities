#include "stdafx.h"
#include <tga2d/drawers/LineDrawer.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/DX11.h>
#include <tga2d/render/RenderObject.h>
#include <tga2d/shaders/shader.h>
#include <tga2d/engine.h>
#include <tga2d/primitives/LinePrimitive.h>

using namespace Tga2D;
LineDrawer::LineDrawer(Engine* anEngine)
	: Shader(anEngine)
{}

LineDrawer::~LineDrawer() {}

bool LineDrawer::Init()
{
	Shader::Init();
	InitShaders();
	CreateBuffer();
	return true;
}

void LineDrawer::CreateBuffer()
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	float theZ = 0.0f;
	myVertices[0].X = -0.5f;
	myVertices[0].Y = 0.5f;
	myVertices[0].Z = theZ;

	myVertices[1].X = -0.5f;
	myVertices[1].Y = -0.5f;
	myVertices[1].Z = theZ;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;       
	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex) * 2000;        
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = myVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	HRESULT hr = DX11::Device->CreateBuffer(&vertexBufferDesc, nullptr, myVertexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		ERROR_PRINT("%s", "Buffer error");
		return;
	}

}

void Tga2D::LineDrawer::Draw(LineMultiPrimitive* aObject)
{
	if (!myVertexBuffer)
	{
		return;
	}
	PrepareRender(*aObject);
	DX11::Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	D3D11_MAPPED_SUBRESOURCE mappedVertexResource;
	SimpleVertex* dataVertexPtr;
	HRESULT result = DX11::Context->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertexResource);
	if (FAILED(result))
	{
		INFO_PRINT("Error in rendering!");
		return;
	}

	dataVertexPtr = (SimpleVertex*)mappedVertexResource.pData;
	int theCount = 0;
	for (unsigned int i = 0; i < aObject->myCount; i++)
	{

		Vector2f correctedPositionFrom = Vector2f((aObject->myFromPosition[i].x * 2.0f) - 1.0f, -(aObject->myFromPosition[i].y * 2) + 1);
		static const float ratioX = (float)Engine::GetInstance()->GetWindowSize().x / (float)Engine::GetInstance()->GetWindowSize().y;

		Vector2f correctedPositionTo = Vector2f((aObject->myToPosition[i].x * 2.0f) - 1.0f, -(aObject->myToPosition[i].y * 2) + 1);

		dataVertexPtr[theCount].X = correctedPositionFrom.x;
		dataVertexPtr[theCount].Y = correctedPositionFrom.y;
		dataVertexPtr[theCount].Z = 0;

		dataVertexPtr[theCount + 1].X = correctedPositionTo.x;
		dataVertexPtr[theCount + 1].Y = correctedPositionTo.y;
		dataVertexPtr[theCount + 1].Z = 0;

		dataVertexPtr[theCount].myColorA = aObject->myColor[i].myA;
		dataVertexPtr[theCount].myColorR = aObject->myColor[i].myR;
		dataVertexPtr[theCount].myColorG = aObject->myColor[i].myG;
		dataVertexPtr[theCount].myColorB = aObject->myColor[i].myB;

		dataVertexPtr[theCount+1].myColorA = aObject->myColor[i].myA;
		dataVertexPtr[theCount+1].myColorR = aObject->myColor[i].myR;
		dataVertexPtr[theCount+1].myColorG = aObject->myColor[i].myG;
		dataVertexPtr[theCount+1].myColorB = aObject->myColor[i].myB;
		theCount += 2;
	}

	
	DX11::Context->Unmap(myVertexBuffer.Get(), 0);


	unsigned int strides = sizeof(SimpleVertex);
	unsigned int offsets = 0;
	DX11::Context->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &strides, &offsets);

	DX11::LogDrawCall();
	DX11::Context->Draw(aObject->myCount * 2, 0);
}

bool Tga2D::LineDrawer::InitShaders()
{
	CreateShaders(L"shaders/lineshader_VS.cso", L"shaders/lineshader_PS.cso");

	return true;
}

bool LineDrawer::CreateInputLayout(const std::string& aVS)
{
	// LAYOUT
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;


	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	HRESULT result = DX11::Device->CreateInputLayout(polygonLayout, numElements, aVS.data(), aVS.size(), myLayout.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		ERROR_PRINT("%s", "Layout error");
		return false;
	}
	return true;
}

void LineDrawer::SetShaderParameters(RenderObjectLine* aObject)
{
	UpdateVertexes(aObject);

	unsigned int strides = sizeof(SimpleVertex);
	unsigned int offsets = 0;
	DX11::Context->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &strides, &offsets);
}

void LineDrawer::UpdateVertexes(RenderObjectLine* aObject)
{
	D3D11_MAPPED_SUBRESOURCE mappedVertexResource;
	SimpleVertex* dataVertexPtr;
	HRESULT result = DX11::Context->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertexResource);
	if(FAILED(result))
	{
		INFO_PRINT("Error in rendering!");
		return;
	}

	Vector2f correctedPositionFrom = Vector2f( (aObject->myFromPosition.x * 2.0f) -1.0f, -(aObject->myFromPosition.y *2) +1) ;
	static const float ratioX = (float)Engine::GetInstance()->GetWindowSize().x / (float)Engine::GetInstance()->GetWindowSize().y;
	//correctedPositionFrom.x *= ratioX;

	Vector2f correctedPositionTo = Vector2f((aObject->myToPosition.x * 2.0f) - 1.0f, -(aObject->myToPosition.y * 2) + 1);
	//correctedPositionTo.x *= ratioX;

	//Vector2f correctedPosition = DX2D::Vector2f((aObject->myPosition.x * 2.0f) - 1.0f, (aObject->myPosition.y * 2.0f));

	dataVertexPtr = (SimpleVertex*)mappedVertexResource.pData;

	dataVertexPtr[0].X = correctedPositionFrom.x;
	dataVertexPtr[0].Y = correctedPositionFrom.y;
	dataVertexPtr[0].Z = 0;

	dataVertexPtr[1].X = correctedPositionTo.x;
	dataVertexPtr[1].Y = correctedPositionTo.y;
	dataVertexPtr[1].Z = 0;

	dataVertexPtr[0].myColorA = aObject->myColor.w;
	dataVertexPtr[0].myColorR = aObject->myColor.x;
	dataVertexPtr[0].myColorG = aObject->myColor.y;
	dataVertexPtr[0].myColorB = aObject->myColor.z;

	dataVertexPtr[1].myColorA = aObject->myColor.w;
	dataVertexPtr[1].myColorR = aObject->myColor.x;
	dataVertexPtr[1].myColorG = aObject->myColor.y;
	dataVertexPtr[1].myColorB = aObject->myColor.z;



	DX11::Context->Unmap(myVertexBuffer.Get(), 0);
}


void LineDrawer::Draw(RenderObjectLine* aObject)
{
	PrepareRender(*aObject);
	DX11::Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	SetShaderParameters(aObject);
	DX11::LogDrawCall();
	DX11::Context->Draw(2, 0);
}