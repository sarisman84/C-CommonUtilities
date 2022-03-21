#include "stdafx.h"
#include <tga2d/engine.h>
#include "Game.h"

#include <tga2d/error/ErrorManager.h>

using namespace std::placeholders;

#ifdef _DEBUG
#pragma comment(lib,"tga2dcore_Debug.lib")
#pragma comment(lib,"External_Debug.lib")
std::wstring BUILD_NAME = L"Debug";
#else
#pragma comment(lib,"tga2dcore_Release.lib")
#pragma comment(lib,"External_Release.lib")
std::wstring BUILD_NAME = L"Release";
#endif // DEBUG


Game::Game()
{
}


Game::~Game()
{
}

LRESULT Game::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	lParam;
	wParam;
	hWnd;
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	}

	return 0;
}


bool Game::Init(const std::wstring& aVersion, HWND /*aHWND*/)
{
	Tga2D::EngineCreateParameters createParameters;

	createParameters.myInitFunctionToCall = [this] {InitCallBack(); };
	createParameters.myWinProcCallback = [this](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {return WinProc(hWnd, message, wParam, lParam); };
	createParameters.myUpdateFunctionToCall = [this] {UpdateCallBack(); };
	createParameters.myApplicationName = L"TGA 2D " + BUILD_NAME + L"[" + aVersion + L"] ";
	//createParameters.myPreferedMultiSamplingQuality = Tga2D::EMultiSamplingQuality::High;
	createParameters.myActivateDebugSystems = Tga2D::DebugFeature::Fps |
		Tga2D::DebugFeature::Mem |
		Tga2D::DebugFeature::Drawcalls |
		Tga2D::DebugFeature::Cpu |
		Tga2D::DebugFeature::Filewatcher |
		Tga2D::DebugFeature::OptimizeWarnings;

	if (!Tga2D::Engine::Start(createParameters))
	{
		ERROR_PRINT("Fatal error! Engine could not start!");
		system("pause");
		return false;
	}

	// End of program
	return true;
}

void Game::InitCallBack()
{
	myGameWorld.Init();
}

void Game::UpdateCallBack()
{
	myGameWorld.Update(Tga2D::Engine::GetInstance()->GetDeltaTime());
	myGameWorld.Render();
}
