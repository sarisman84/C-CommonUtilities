#include "stdafx.h"

#include <tga2d/engine.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/debugging/MemoryTracker.h>
#include <tga2d/drawers/DebugDrawer.h>
#include <tga2d/error/ErrorManager.h>
#include <tga2d/filewatcher/FileWatcher.h>
#include <tga2d/graphics/dx11.h>
#include <tga2d/light/LightManager.h>
#include <tga2d/render/renderer.h>
#include <tga2d/text/TextService.h>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/model/ModelFactory.h>
#include <tga2d/windows/WindowsWindow.h>
#include <tga2d/graphics/HTMLParser.h>
#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX 
#include <windows.h>
#include <tga2d/imguiinterface/ImGuiInterface.h>

#pragma comment( lib, "user32.lib" )

using namespace Tga2D; 
Engine* Tga2D::Engine::myInstance = nullptr;
Engine::Engine( const EngineCreateParameters& aCreateParameters)
: myWindow(nullptr)
, myGraphicsEngine(nullptr)
, myRenderer(nullptr)
, myTextureManager(nullptr)
, myDebugDrawer(nullptr)
, myHwnd(nullptr)
, myHInstance(nullptr)
, myLightManager(nullptr)
, myCreateParameters(aCreateParameters)
, myRunEngine(true)
, myTotalTime(0.0f)
, myDeltaTime(0.0f)
, myShouldExit(false)
, myWantToUpdateSize(false)
{
	myWindowSize.x = myCreateParameters.myWindowWidth;
	myWindowSize.y = myCreateParameters.myWindowHeight;

	myRenderSize.x = myCreateParameters.myRenderWidth;
	myRenderSize.y = myCreateParameters.myRenderHeight;

	myTargetSize = myRenderSize;
	if (myCreateParameters.myTargetWidth > 0 && myCreateParameters.myTargetHeight > 0)
	{
		myTargetSize.x = myCreateParameters.myTargetWidth;
		myTargetSize.y = myCreateParameters.myTargetHeight;
	}
	

	myDebugDrawer = std::make_unique<DebugDrawer>(myCreateParameters.myActivateDebugSystems != DebugFeature::None);

	myInitFunctionToCall = myCreateParameters.myInitFunctionToCall;
	myUpdateFunctionToCall = myCreateParameters.myUpdateFunctionToCall;
	
	myHwnd = myCreateParameters.myHwnd;
	myHInstance = myCreateParameters.myHInstance;
	myCreateParameters.myHInstance = myCreateParameters.myHInstance;
	myClearColor = myCreateParameters.myClearColor;

	myErrorManager = std::make_unique<ErrorManager>();
	myErrorManager->AddLogListener(aCreateParameters.myLogFunction, aCreateParameters.myErrorFunction);

	myImguiInterFace = std::make_unique<ImGuiInterface>();
}


Engine::~Engine()
{}


void Tga2D::Engine::DestroyInstance()
{
	if (myInstance)
	{
		delete myInstance;
		myInstance = nullptr;
		StopMemoryTrackingAndPrint();
	}
}

bool Engine::Start(const EngineCreateParameters& aCreateParameters)
{
	if (!myInstance)
	{
		MemoryTrackingSettings trackingSettings;
		trackingSettings.myShouldTrackAllAllocations = ((aCreateParameters.myActivateDebugSystems & DebugFeature::MemoryTrackingAllAllocations) != DebugFeature::None);
		trackingSettings.myShouldStoreStackTraces = ((aCreateParameters.myActivateDebugSystems & DebugFeature::MemoryTrackingStackTraces) != DebugFeature::None);
		StartMemoryTracking(trackingSettings);

		myInstance = new Engine(aCreateParameters);
		return myInstance->InternalStart();
	}
	else
	{
		ERROR_PRINT("%s", "DX2D::Engine::CreateInstance called twice, thats bad.");
	}
	return false;
}

bool Engine::InternalStart()
{
	INFO_PRINT("%s", "#########################################");
	INFO_PRINT("%s", "---TGA 2D Starting, dream big and dare to fail---");
	myFileWatcher = std::make_unique<FileWatcher>();
	myWindow = std::make_unique<WindowsWindow>();
	if (!myWindow->Init(myWindowSize, myHwnd, &myCreateParameters, myHInstance, myCreateParameters.myWinProcCallback))
	{
		ERROR_PRINT("%s", "Window failed to be created!");
		return false;
	}

	myDx11 = std::make_unique<DX11>();
	if (!myDx11->Init(myWindow.get()))
	{
		ERROR_PRINT("%s", "D3D failed to be created!");
		myWindow->Close();
		return false;
	}

	myGraphicsEngine = std::make_unique<GraphicsEngine>();
	if (!myGraphicsEngine->Init())
	{
		ERROR_PRINT("%s", "D3D engine failed to be created!");
		myWindow->Close();
		return false;
	}

	myRenderer = std::make_unique<Renderer>();
	myHTMLParser = std::make_unique<HTML_Parser>();

	myTextureManager = std::make_unique<TextureManager>();
	myTextureManager->Init();

	myTextService = std::make_unique<TextService>();
	myTextService->Init();

	myLightManager = std::make_unique<LightManager>();
	CalculateRatios();

	if (myDebugDrawer)
	{
		myDebugDrawer->Init();
	}

	if (myInitFunctionToCall)
	{
		myInitFunctionToCall();
	}

	myImguiInterFace->Init();

	myStartOfTime = std::chrono::steady_clock::now();

    if( myUpdateFunctionToCall )
    {
        StartStep();
		myInstance->Shutdown();
        DestroyInstance();
    }

	return true;
}

void Tga2D::Engine::Shutdown()
{
	ModelFactory::DestroyInstance();

	if (myInstance)
	{
		DestroyInstance();
	}
}

void Tga2D::Engine::StartStep()
{
	DoStep();
}

void Tga2D::Engine::IdleProcess()
{
	if (!myGraphicsEngine || myGraphicsEngine->IsInitiated() == false)
	{
		return;
	}
	myTimer.Tick([&]()
	{
		myImguiInterFace->PreFrame();
		myDeltaTime = static_cast<float>(myTimer.GetElapsedSeconds());
		myTotalTime += static_cast<float>(myTimer.GetElapsedSeconds());

		BeginFrame(myClearColor);

		myTextureManager->Update();
		
		if (myUpdateFunctionToCall)
		{
			myUpdateFunctionToCall();
		}

		if (myDebugDrawer)
		{
			myDebugDrawer->Update(myDeltaTime);
			myDebugDrawer->Render();
		}
		myHTMLParser->Update();
		myHTMLParser->Render();
		myRenderer->Update();
		myImguiInterFace->Render();
		EndFrame();

		if (myWantToUpdateSize)
		{
			UpdateWindowSizeChanges();
			myWantToUpdateSize = false;
		}
		myFileWatcher->FlushChanges();
	});
}

void Tga2D::Engine::DoStep()
{
	MSG msg = {0};
	while (myRunEngine)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			if(msg.message == WM_QUIT)
			{
				INFO_PRINT("%s", "Exiting...");
				myRunEngine = false;
				break;
			}				

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if(myRunEngine)
		IdleProcess();
		
		std::this_thread::yield();
		
	}
}

void GetDesktopResolution(int& horizontal, int& vertical, HWND aHwnd)
{
	RECT r;
	GetClientRect(aHwnd, &r); //get window rect of control relative to screen
	horizontal = r.right - r.left;
	vertical = r.bottom - r.top;
}

void Tga2D::Engine::UpdateWindowSizeChanges(bool aIgnoreAutoUpdate)
{
	if (myCreateParameters.myUseLetterboxAndPillarbox || aIgnoreAutoUpdate)
	{
		int horizontal = 0;
		int vertical = 0;
		GetDesktopResolution(horizontal, vertical, *myHwnd);

		// Both these values must be your real window size, so of course these values can't be static
		float screen_width = (float)horizontal;
		float screen_height = (float)vertical;

		// This is your target virtual resolution for the game, the size you built your game to
		float virtual_width = (float)myTargetSize.x;
		float  virtual_height = (float)myTargetSize.y;

		float targetAspectRatio = virtual_width / virtual_height;

		// figure out the largest area that fits in this resolution at the desired aspect ratio
		float  width = screen_width;
		float  height = (float)(width / targetAspectRatio);

		if (height > screen_height)
		{
			//It doesn't fit our height, we must switch to pillarbox then
			height = screen_height;
			width = (float)(height * targetAspectRatio);
		}

		// set up the new viewport centered in the backbuffer
		float  vp_x = (screen_width / 2.0f) - (width / 2.0f);
		float  vp_y = (screen_height / 2.0f) - (height / 2.0f);

		static float lastWidth = 0.0f;
		static float lastHeight = 0.0f;

		{
			if (myCreateParameters.myTargetWidth != width || myCreateParameters.myTargetHeight != height)
			{
				INFO_TIP("%s", "Warning: Window is resized, it is not matching your native target resolution, there might be flickering on sprites that are close to one another");
			}

			lastWidth = width;
			lastHeight = height;
			SetViewPort(vp_x, vp_y, width, height, 0, 1, true);
		}
		
	}
	else
	{
		SetResolution(Vector2ui(myCreateParameters.myTargetWidth, myCreateParameters.myTargetHeight), false);
		SetViewPort(0, 0, myCreateParameters.myTargetWidth, myCreateParameters.myTargetHeight);
	}
}

float Tga2D::Engine::GetWindowRatio() const
{
	return myWindowRatio;
}

float Tga2D::Engine::GetWindowRatioInversed() const
{
	return myWindowRatioInversed;
}

Vector2f Tga2D::Engine::GetWindowRatioVec() const
{
	return myWindowRatioVec;
}

Vector2f Tga2D::Engine::GetWindowRatioInversedVec() const
{
	return myWindowRatioInversedVec;
}

void Tga2D::Engine::SetTargetSize(const Vector2ui& aTargetSize)
{
	myTargetSize = aTargetSize;
}

void Tga2D::Engine::SetResolution(const Vector2ui &aResolution, bool aAlsoSetWindowSize)
{
	myWindowSize = aResolution;
	if (aAlsoSetWindowSize)
	{
		myWindow->SetResolution(aResolution);
	}
	myDx11->Resize(aResolution);
	DX11::BackBuffer->SetAsActiveTarget();

	CalculateRatios();
}

void Tga2D::Engine::CalculateRatios()
{
	float sizeX = static_cast<float>(myWindowSize.x);
	float sizeY = static_cast<float>(myWindowSize.y);
	if (sizeY > sizeX)
	{
		float temp = sizeX;
		sizeX = sizeY;
		sizeY = temp;
	}

	myWindowRatio = static_cast<float>(sizeX) / static_cast<float>(sizeY);
	myWindowRatioInversed = static_cast<float>(sizeY) / static_cast<float>(sizeX);
	
	myWindowRatioVec.x = 1.0f;
	myWindowRatioVec.y = 1.0f;
	myWindowRatioInversedVec.x = 1.0f;
	myWindowRatioInversedVec.y = 1.0f;
	if (sizeX >= sizeY)
	{
		myWindowRatioVec.y = myWindowRatio;
		myWindowRatioInversedVec.y = myWindowRatioInversed;
	}
	else
	{
		myWindowRatioVec.x = myWindowRatio;
		myWindowRatioInversedVec.x = myWindowRatioInversed;
	}
}

HWND* Tga2D::Engine::GetHWND() const
{
	return myHwnd;
}


HINSTANCE Tga2D::Engine::GetHInstance() const
{
	return myHInstance;
}

void Tga2D::Engine::SetViewPort(float aTopLeftX, float aTopLeftY, float aWidth, float aHeight, float aMinDepth, float aMaxDepth, bool aSetEngineResolution)
{
	if (myGraphicsEngine)
	{
		if (aSetEngineResolution)
		{
			SetResolution(Vector2ui(static_cast<unsigned int>(aWidth), static_cast<unsigned int>(aHeight)), false);
		}	

		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = aTopLeftX;
		viewport.TopLeftY = aTopLeftY;
		viewport.Width = aWidth;
		viewport.Height = aHeight;
		viewport.MinDepth = aMinDepth;
		viewport.MaxDepth = aMaxDepth;

		DX11::Context->RSSetViewports(1, &viewport);
	}
}

void Tga2D::Engine::SetClearColor(const Color& aClearColor)
{
	myClearColor = aClearColor;
}

void Tga2D::Engine::SetFullScreen(bool aFullScreen)
{
	if (myGraphicsEngine)
	{
		myGraphicsEngine->SetFullScreen(aFullScreen);
	}
}

void Tga2D::Engine::SetAmbientLightValue(float aAmbientLight)
{
	myLightManager->SetAmbientLight(AmbientLight({ aAmbientLight,aAmbientLight,aAmbientLight}, 0.0));
}

bool Tga2D::Engine::IsDebugFeatureOn(DebugFeature aFeature) const
{
	const bool all = ((myCreateParameters.myActivateDebugSystems & DebugFeature::All) != DebugFeature::None);
	if (all)
	{
		return true;
	}

	const bool specific = ((myCreateParameters.myActivateDebugSystems & aFeature) != DebugFeature::None);
	return specific;
}

//void Tga2D::Engine::SetSampler(ESamplerType aType)
//{
//	if (myGraphicsEngine)
//	{
//		myGraphicsEngine->SetSampler(aType);
//	}
//}
//
//ESamplerType Tga2D::Engine::GetSamplerType() const
//{
//	if (myGraphicsEngine)
//	{
//		return myGraphicsEngine->GetSamplerType();
//	}
//	return ESamplerType_Linear;
//}

bool Engine::BeginFrame(const Color &aClearColor)
{
	if (myShouldExit)
	{
		return false;
	}

	MSG msg = { 0 };

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			INFO_PRINT("%s", "Exiting...");
			myShouldExit = true;
			return false;
		}
	}

    myFileWatcher->FlushChanges();
	
	myDx11->BeginFrame(aClearColor);
    myTextureManager->Update();
	DX11::RenderStateManager->ResetStates();
	DX11::ResetDrawCallCounter();

	return true;
}


void Engine::EndFrame( void )
{
	myTimer.Tick([&]()
	{
		myDeltaTime = static_cast<float>(myTimer.GetElapsedSeconds());
		myTotalTime += static_cast<float>(myTimer.GetElapsedSeconds());

		myRenderer->Update();
			
		if (myDebugDrawer)
		{
			myDebugDrawer->Update(myDeltaTime);
			myDebugDrawer->Render();
		}

		myDx11->EndFrame(myCreateParameters.myEnableVSync);

		if (myWantToUpdateSize)
		{
			UpdateWindowSizeChanges();
			myWantToUpdateSize = false;
		}
	});

    std::this_thread::yield();
}
