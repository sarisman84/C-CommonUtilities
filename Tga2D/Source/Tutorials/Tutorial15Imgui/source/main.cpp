#include <string>
#include <tga2d/engine.h>
#include <tga2d/error/error_manager.h>

#include "../../TutorialCommon/TutorialCommon.h"
#include <tga2d/audio/audio.h>
#include <tga2d/imguiinterface/CImGuiInterface.h>

// Making sure that DX2DEngine lib is linked
//
#ifdef _DEBUG
#pragma comment(lib,"tga2dcore_Debug.lib")
#pragma comment(lib,"External_Debug.lib")
#else
#pragma comment(lib,"tga2dcore_Release.lib")
#pragma comment(lib,"External_Release.lib")
#endif // _DEBUG

void Go(void);
int main(const int /*argc*/, const char* /*argc*/[])
{
	Go();
	return 0;
}



LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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


void InitCallBack()
{

}

void UpdateCallBack()
{
#ifndef _RETAIL // <-- SUPER IMPORTANT as imgui is not compiled in retail, if you dont have this you will get a compiler error on all imgui calls you do
	ImGui::ShowDemoWindow();

	//Blow we change the back color
	static bool isOpen = false;
	if (ImGui::Begin("Settings", &isOpen, 0))
	{
		static float rgb[3];
		ImGui::ColorPicker3("BG Color", rgb);

		Tga2D::CEngine::GetInstance()->SetClearColor(Tga2D::CColor(rgb[0], rgb[1], rgb[2], 1.0f));

	}
	ImGui::End();
#endif

}


void Go()
{
	{
#ifdef _RETAIL 
		MessageBoxA(nullptr, "Imgui is not avalible in retail, select release or debug instead", "Error", MB_OK);
#endif
		// The below code is the same as the start of the default TGA2D start. You can find this code in "Game.cpp" under the Game project
		Tga2D::SEngineCreateParameters createParameters;

		createParameters.myInitFunctionToCall = [] {InitCallBack(); };
		createParameters.myWinProcCallback = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {return WinProc(hWnd, message, wParam, lParam); };
		createParameters.myUpdateFunctionToCall = [] {UpdateCallBack(); };
		createParameters.myApplicationName = L"TGA 2D Tutorial 15 Imgui";
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
			return;
		}

		if (Tga2D::CEngine::GetInstance()) {
			Tga2D::CEngine::GetInstance()->RunUpdateLoop();
		}
		else {
			ImGui::DestroyContext();
		}
	}

	Tga2D::CEngine::GetInstance()->Shutdown();
}

