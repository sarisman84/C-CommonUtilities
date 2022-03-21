#include "stdafx.h"

#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/engine.h>
#include <tga2d/drawers/CustomShapeDrawer.h>
#include <tga2d/drawers/LineDrawer.h>
#include <tga2d/drawers/ModelDrawer.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/graphics/DX11.h>
#include <tga2d/graphics/FullscreenEffect.h>
#include <tga2d/graphics/ForwardRenderer.h>
#include <tga2d/light/LightManager.h>
#include <tga2d/math/CommonMath.h>
#include <tga2d/render/RenderCommon.h>
#include <tga2d/render/RenderObject.h>
#include <tga2d/texture/texture.h>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/windows/WindowsWindow.h>
#include <DDSTextureLoader/DDSTextureLoader11.h>
#include <tga2d/primitives/LinePrimitive.h>
#include <tga2d/EngineDefines.h>


#include <fstream>
#include <d3dcompiler.h>
#include <d3d11_1.h>
#include <dxgi.h>
#include <thread>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

using namespace Tga2D;

GraphicsEngine::GraphicsEngine()
	: myIsInitiated(false)
{}

GraphicsEngine::~GraphicsEngine(void)
{
}

bool GraphicsEngine::Init()
{
	INFO_PRINT("%s", "Starting graphics engine");

	myCamera = std::make_unique<Camera>();

	DX11::BackBuffer->SetAsActiveTarget();

	mySpriteDrawer = std::make_unique<SpriteDrawer>();
	mySpriteDrawer->Init();

	myModelDrawer = std::make_unique<ModelDrawer>();
	myModelDrawer->Init();

	myCustomShapeDrawer = std::make_unique<CustomShapeDrawer>(Engine::GetInstance());
	myCustomShapeDrawer->Init();

	myLineDrawer = std::make_unique<LineDrawer>(Engine::GetInstance());
	myLineDrawer->Init();

	myForwardRenderer = std::make_unique<ForwardRenderer>();
	if (!myForwardRenderer->Init())
		return false;

	myFullscreenCopy = std::make_unique<FullscreenEffect>();
	if (!myFullscreenCopy->Init("Shaders/PostprocessCopyPS.cso"))
		return false;
	myFullscreenTonemap = std::make_unique<FullscreenEffect>();
	if (!myFullscreenTonemap->Init("Shaders/PostprocessTonemapPS.cso"))
		return false;
	myFullscreenVerticalGaussianBlur = std::make_unique<FullscreenEffect>();
	if (!myFullscreenVerticalGaussianBlur->Init("Shaders/PostprocessGaussianV_PS.cso"))
		return false;
	myFullscreenHorizontalGaussianBlur = std::make_unique<FullscreenEffect>();
	if (!myFullscreenHorizontalGaussianBlur->Init("Shaders/PostprocessGaussianH_PS.cso"))
		return false;

	HRESULT result = S_OK;

	D3D11_BUFFER_DESC commonBufferDesc;
	commonBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	commonBufferDesc.ByteWidth = sizeof(CommonConstantBufferData);
	commonBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	commonBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	commonBufferDesc.MiscFlags = 0;
	commonBufferDesc.StructureByteStride = 0;

	result = DX11::Device->CreateBuffer(&commonBufferDesc, NULL, myCommonBuffer.ReleaseAndGetAddressOf());

	// LIGHTS
	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightConstantBufferData);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	LightConstantBufferData lightType;
	D3D11_SUBRESOURCE_DATA lightData;
	lightData.pSysMem = &lightType;
	lightData.SysMemPitch = 0;
	lightData.SysMemSlicePitch = 0;

	result = DX11::Device->CreateBuffer(&lightBufferDesc, &lightData, myLightBuffer.ReleaseAndGetAddressOf());

	INFO_PRINT("%s", "All done, starting...");
	INFO_PRINT("%s", "#########################################");
	myIsInitiated = true;
	return true;
}

bool GraphicsEngine::IsInitiated()
{
	return myIsInitiated;
}

void Tga2D::GraphicsEngine::SetCamera(const Camera& camera)
{
	*myCamera = camera;
}

const Camera& Tga2D::GraphicsEngine::GetCamera() const
{
	return *myCamera;
}

void Tga2D::GraphicsEngine::UpdateAndBindCommonBuffer()
{
	if (!myCommonBuffer)
	{
		return;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResourceCommon;
	CommonConstantBufferData* dataPtrCommon;
	HRESULT result = DX11::Context->Map(myCommonBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceCommon);
	if (FAILED(result))
	{
		INFO_PRINT("Error in rendering!");
		return;
	}

	dataPtrCommon = (CommonConstantBufferData*)mappedResourceCommon.pData;
	dataPtrCommon->myResolution = Vector4f(static_cast<float>(DX11::GetResolution().x), static_cast<float>(DX11::GetResolution().y), 0, 0);
	dataPtrCommon->myTimings.x = Engine::GetInstance()->myTotalTime;
	dataPtrCommon->myTimings.y = Engine::GetInstance()->myDeltaTime;
	dataPtrCommon->myToCamera = Matrix4x4f::GetFastInverse(myCamera->GetTransform().GetMatrix());
	dataPtrCommon->myToProjection = myCamera->GetProjection();

	float camNearPlane = 0;
	float camFarPlane = 0;
	myCamera->GetProjectionPlanes(camNearPlane, camFarPlane);

	dataPtrCommon->myNearPlane = camNearPlane;
	dataPtrCommon->myFarPlane = camFarPlane;
	dataPtrCommon->myCameraPosition = myCamera->GetTransform().GetMatrix().GetPosition();

	DX11::Context->Unmap(myCommonBuffer.Get(), 0);
	DX11::Context->VSSetConstantBuffers((int)ConstantBufferSlot::Common, 1, myCommonBuffer.GetAddressOf());
	DX11::Context->PSSetConstantBuffers((int)ConstantBufferSlot::Common, 1, myCommonBuffer.GetAddressOf());
}

void Tga2D::GraphicsEngine::UpdateAndBindLightBuffer()
{
#ifdef USE_LIGHTS
	if (myLightBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResourceLight;
		LightConstantBufferData* dataPtrLights;

		HRESULT result = DX11::Context->Map(myLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceLight);
		if (FAILED(result))
		{
			INFO_PRINT("Error in rendering! Could not map lights");
			return;
		}


		size_t pointLightCount = Engine::GetInstance()->GetLightManager().GetPointLightCount();
		const PointLight* pointLights = Engine::GetInstance()->GetLightManager().GetPointLights();
		dataPtrLights = (LightConstantBufferData*)mappedResourceLight.pData;
		*dataPtrLights = {};

		dataPtrLights->myNumberOfLights = static_cast<unsigned int>(pointLightCount);

		for (int i = 0; i < pointLightCount; i++)
		{
			const PointLight& pointLight = pointLights[i];
			dataPtrLights->myPointLights[i].myColorAndIntensity = pointLight.GetColor().AsLinearVec4();
			dataPtrLights->myPointLights[i].myColorAndIntensity.w = pointLight.GetIntensity();
			dataPtrLights->myPointLights[i].myRange = pointLight.GetRange();
			dataPtrLights->myPointLights[i].myPosition = pointLight.GetTransform().GetPosition();
		}

		const DirectionalLight& directionalLight = Engine::GetInstance()->GetLightManager().GetDirectionalLight();
		const AmbientLight& ambientLight = Engine::GetInstance()->GetLightManager().GetAmbientLight();
		dataPtrLights->myDirectionalLightDirection = directionalLight.GetTransform().GetMatrix().GetForward();
		dataPtrLights->myDirectionalLightColorAndIntensity = directionalLight.GetColor().AsLinearVec4();
		dataPtrLights->myDirectionalLightColorAndIntensity.w = directionalLight.GetIntensity();
		dataPtrLights->myNumEnvMapMipLevels = ambientLight.GetNumMips();
		dataPtrLights->myAmbientLightColorAndIntensity = ambientLight.GetColor().AsLinearVec4();
		dataPtrLights->myAmbientLightColorAndIntensity.w = ambientLight.GetIntensity();

		DX11::Context->Unmap(myLightBuffer.Get(), 0);
	}

#endif
	DX11::Context->PSSetConstantBuffers((int)ConstantBufferSlot::Light, 1, myLightBuffer.GetAddressOf());
	DX11::Context->VSSetConstantBuffers((int)ConstantBufferSlot::Light, 1, myLightBuffer.GetAddressOf());
}

void Tga2D::GraphicsEngine::Draw(RenderObject* aObject)
{
#ifdef _DEBUG
	if (!DX11::IsOnSameThreadAsEngine())
	{
		ERROR_PRINT("%s", "Fatal error! You are trying to render on another thread that DX11 is created, this will result in a crash! DXContext is not thread safe! Create your engine on the same thread you are rendering with!");
	}
#endif
	aObject->Draw(this);
}

void Tga2D::GraphicsEngine::DoDraw(LineMultiPrimitive* aObject)
{
	myLineDrawer->Draw(aObject);
}

void Tga2D::GraphicsEngine::DoDraw(RenderObjectCustom* aCustomObject)
{
	myCustomShapeDrawer->Draw(aCustomObject);
}

void  Tga2D::GraphicsEngine::DoDraw(RenderObjectLine* aObject)
{
	myLineDrawer->Draw(aObject);
}

void Tga2D::GraphicsEngine::SetFullScreen(bool aFullScreen)
{
	DX11::SwapChain->SetFullscreenState(aFullScreen, nullptr);
}