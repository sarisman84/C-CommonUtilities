#include <string>
#include <tga2d/engine.h>
#include <tga2d/error/ErrorManager.h>

#include "../../TutorialCommon/TutorialCommon.h"
#include <tga2d/audio/audio.h>
#include <tga2d/imguiinterface/ImGuiInterface.h>

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

		Tga2D::Engine::GetInstance()->SetClearColor(Tga2D::Color(rgb[0], rgb[1], rgb[2], 1.0f));

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
		Tga2D::EngineCreateParameters createParameters;

		createParameters.myInitFunctionToCall = [] {InitCallBack(); };
		createParameters.myWinProcCallback = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {return WinProc(hWnd, message, wParam, lParam); };
		createParameters.myUpdateFunctionToCall = [] {UpdateCallBack(); };
		createParameters.myApplicationName = L"TGA 2D Tutorial 15 Imgui";
		//createParameters.myPreferedMultiSamplingQuality = Tga2D::MultiSamplingQuality::High;
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
			return;
		}
	}

	Tga2D::Engine::GetInstance()->Shutdown();
}

