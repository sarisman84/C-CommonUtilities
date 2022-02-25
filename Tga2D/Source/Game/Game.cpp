#include "stdafx.h"
#include <tga2d/engine.h>
#include "Game.h"

#include <tga2d/error/error_manager.h>

using namespace std::placeholders;

#ifdef _DEBUG
#pragma comment(lib,"tga2dcore_Debug.lib")
#pragma comment(lib,"External_Debug.lib")
std::wstring BUILD_NAME = L"Debug";
#endif // DEBUG
#ifdef _RELEASE
#pragma comment(lib,"tga2dcore_Release.lib")
#pragma comment(lib,"External_Release.lib")
std::wstring BUILD_NAME = L"Release";
#endif // DEBUG
#ifdef _RETAIL
#pragma comment(lib,"tga2dcore_Retail.lib")
#pragma comment(lib,"External_Retail.lib")
std::wstring BUILD_NAME = L"Retail";
#endif // DEBUG

CGame::CGame()
{
}


CGame::~CGame()
{
}

LRESULT CGame::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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


bool CGame::Init(const std::wstring& aVersion, HWND /*aHWND*/)
{
	Tga2D::SEngineCreateParameters createParameters;

	createParameters.myInitFunctionToCall = [this] {InitCallBack(); };
	createParameters.myWinProcCallback = [this](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {return WinProc(hWnd, message, wParam, lParam); };
	createParameters.myUpdateFunctionToCall = [this] {UpdateCallBack(); };
	createParameters.myApplicationName = L"TGA 2D " + BUILD_NAME + L"[" + aVersion + L"] ";
	//createParameters.myPreferedMultiSamplingQuality = Tga2D::EMultiSamplingQuality::High;
	createParameters.myActivateDebugSystems = Tga2D::EDebugFeature::Fps |
		Tga2D::EDebugFeature::Mem |
		Tga2D::EDebugFeature::Drawcalls |
		Tga2D::EDebugFeature::Cpu |
		Tga2D::EDebugFeature::Filewatcher |
		Tga2D::EDebugFeature::OptimizeWarnings;

	if (!Tga2D::CEngine::Start(createParameters))
	{
		ERROR_PRINT("Fatal error! Engine could not start!");
		system("pause");
		return false;
	}

	// End of program
	return true;
}

void CGame::InitCallBack()
{
	myGameWorld.Init();
}

void CGame::UpdateCallBack()
{
	myGameWorld.Update(Tga2D::CEngine::GetInstance()->GetDeltaTime());
	myGameWorld.Render();

}
