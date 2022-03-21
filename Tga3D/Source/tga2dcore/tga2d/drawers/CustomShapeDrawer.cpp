#include "stdafx.h"

#include <tga2d/drawers/CustomShapeDrawer.h>
#include <tga2d/render/RenderObject.h>
#include <tga2d/engine.h>
#include <tga2d/graphics/DX11.h>
#include <tga2d/light/LightManager.h>
#include <tga2d/shaders/ShaderCommon.h>
#include <tga2d/primitives/CustomShape.h>
#include <tga2d/texture/texture.h>
#include <tga2d/EngineDefines.h>

using namespace Tga2D;

CustomShapeDrawer::CustomShapeDrawer( Engine* anEngine )
    : Shader( anEngine )
{}

CustomShapeDrawer::~CustomShapeDrawer() {}

bool CustomShapeDrawer::Init()
{
    myMaxPoints = MAX_POINTS_IN_CUSTOM_SHAPE;
    Shader::Init();

    D3D11_BUFFER_DESC objectBufferDesc;
    objectBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    objectBufferDesc.ByteWidth = sizeof( CustomShapeConstantBufferData );
    objectBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    objectBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    objectBufferDesc.MiscFlags = 0;
    objectBufferDesc.StructureByteStride = 0;

    HRESULT result;
    result = DX11::Device->CreateBuffer( &objectBufferDesc, NULL, myObjectBuffer.ReleaseAndGetAddressOf() );
    if( FAILED( result ) )
    {
        ERROR_PRINT( "%s", "Object Buffer error" );
        return false;
    }

    InitShaders();
    CreateBuffer();
    return true;
}

void CustomShapeDrawer::CreateBuffer()
{
    D3D11_BUFFER_DESC vertexBufferDesc;

    ZeroMemory( &vertexBufferDesc, sizeof( vertexBufferDesc ) );

    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof( SimpleVertex ) * myMaxPoints;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    HRESULT hr = DX11::Device->CreateBuffer( &vertexBufferDesc, nullptr, myVertexBuffer.ReleaseAndGetAddressOf());
    if( FAILED( hr ) )
    {
        ERROR_PRINT( "%s", "Buffer error" );
        return;
    }
}

bool CustomShapeDrawer::InitShaders()
{
    CreateShaders( L"shaders/custom_shape_VS.cso", L"shaders/custom_shape_PS.cso" );

    return true;
}

bool CustomShapeDrawer::CreateInputLayout(const std::string& aVS)
{
    // LAYOUT
    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

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

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 1;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

    unsigned int numElements = sizeof( polygonLayout ) / sizeof( polygonLayout[0] );

    // Create the vertex input layout.
    HRESULT result = DX11::Device->CreateInputLayout( polygonLayout, numElements, aVS.data(), aVS.size(), myLayout.ReleaseAndGetAddressOf() );
    if( FAILED( result ) )
    {
        ERROR_PRINT( "%s", "Layout error" );
        return false;
    }
    return true;
}

int CustomShapeDrawer::SetShaderParameters( RenderObjectCustom* aObject )
{
    if( !myObjectBuffer )
    {
        return 0;
    }
    int tris = UpdateVertexes( aObject );

    D3D11_MAPPED_SUBRESOURCE mappedObjectResource;

    HRESULT result = DX11::Context->Map( myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedObjectResource );
    if( FAILED( result ) )
    {
        INFO_PRINT( "Error in rendering!" );
        return 0;
    }

    CustomShapeConstantBufferData* objectDataPtr;
    objectDataPtr = (CustomShapeConstantBufferData*)mappedObjectResource.pData;

    bool haveLights = Engine::GetInstance()->GetLightManager().GetPointLightCount() > 0;
    float lightType = haveLights ? 1.0f : 0.0f;

    objectDataPtr->myRotationAndSize = Vector4f(aObject->GetRotation(), aObject->GetSize().x, aObject->GetSize().y, lightType );
	objectDataPtr->myPosition.x = aObject->GetPosition().x;
	objectDataPtr->myPosition.y = aObject->GetPosition().y;

    DX11::Context->Unmap( myObjectBuffer.Get(), 0 );

    unsigned int strides = sizeof( SimpleVertex );
    unsigned int offsets = 0;
    DX11::Context->IASetVertexBuffers( 0, 1, myVertexBuffer.GetAddressOf(), &strides, &offsets );

	if (CustomShape* shape = static_cast<CustomShape*>(aObject))
	{
        shape->GetTexture()->SetAsResourceOnSlot(0);
	}
	
    DX11::Context->VSSetConstantBuffers((int)ConstantBufferSlot::Object, 1, myObjectBuffer.GetAddressOf() );
    return tris;
}

int CustomShapeDrawer::UpdateVertexes( RenderObjectCustom* aObject )
{
    D3D11_MAPPED_SUBRESOURCE mappedVertexResource;
    SimpleVertex* dataVertexPtr;
    HRESULT result = DX11::Context->Map( myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertexResource );
    if( FAILED( result ) )
    {
        INFO_PRINT( "Error in rendering!" );
        return 0;
    }

    dataVertexPtr = (SimpleVertex*)mappedVertexResource.pData;

    int index = 0;
    for( SCustomPoint& point : aObject->myPoints )
    {
        if( index >= myMaxPoints )
        {
            INFO_PRINT( "%s%i%s", "Customshape:Render - Too many points rendered at one custom shape! We support: ", myMaxPoints, " skipping the rest, increase this nuber in engine_defines.h" );
            continue;
        }

		Vector2f offsetPos(Tga2D::Engine::GetInstance()->GetWindowRatio(), -1.0f);
		Vector2f correctedPositionFrom = Vector2f((point.myPosition.x * 2.0f) - 1.0f, -(point.myPosition.y * 2.0f) + 1.0f);

        dataVertexPtr[index].X = correctedPositionFrom.x;
        dataVertexPtr[index].Y = correctedPositionFrom.y;
        dataVertexPtr[index].Z = 0;

        Vector4f linearColor = point.myColor.AsLinearVec4();
        dataVertexPtr[index].myColorR = linearColor.x;
        dataVertexPtr[index].myColorG = linearColor.y;
        dataVertexPtr[index].myColorB = linearColor.z;
        dataVertexPtr[index].myColorA = linearColor.w;

		dataVertexPtr[index].myU = point.myUV.x;
		dataVertexPtr[index].myV = point.myUV.y;

        index++;
    }

    DX11::Context->Unmap( myVertexBuffer.Get(), 0 );
    return index;
}

void Tga2D::CustomShapeDrawer::Draw( RenderObjectCustom* aObject )
{
	if (!PrepareRender(*aObject))
	{
		return;
	}

	DX11::RenderStateManager->SetBlendState(aObject->myBlendState);
    DX11::Context->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    int tris = SetShaderParameters( aObject );
    if( tris > 0 && tris % 3 == 0 )
    {
        DX11::LogDrawCall();
        DX11::Context->Draw( tris, 0 );
    }
}
