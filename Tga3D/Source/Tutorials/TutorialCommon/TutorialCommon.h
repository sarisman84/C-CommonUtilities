#pragma once
#include <tga2d/engine.h>
#include <tga2d/error/ErrorManager.h>

namespace TutorialCommon
{
	void Init(std::wstring aAppName, bool aFullDebug = false, bool aVsyncEnable = false)
	{
		unsigned short windowWidth = 1920;
		unsigned short windowHeight = 1080;


		Tga2D::EngineCreateParameters createParameters;
		if (aFullDebug)
		{
			createParameters.myActivateDebugSystems = Tga2D::DebugFeature::Fps | Tga2D::DebugFeature::Mem | Tga2D::DebugFeature::Filewatcher | Tga2D::DebugFeature::Cpu | Tga2D::DebugFeature::Drawcalls | Tga2D::DebugFeature::OptimizeWarnings;

		}
		else
		{
			createParameters.myActivateDebugSystems = Tga2D::DebugFeature::Filewatcher;

		}

		createParameters.myWindowHeight = windowHeight;
		createParameters.myWindowWidth = windowWidth;
		createParameters.myRenderHeight = windowHeight;
		createParameters.myRenderWidth = windowWidth;
		createParameters.myTargetWidth = windowWidth;
		createParameters.myTargetHeight = windowHeight;
		createParameters.myWindowSetting = Tga2D::WindowSetting::Overlapped;
		//createParameters.myAutoUpdateViewportWithWindow = true;
		createParameters.myStartInFullScreen = false;
		createParameters.myPreferedMultiSamplingQuality = Tga2D::MultiSamplingQuality::High;

		createParameters.myApplicationName = aAppName;
		createParameters.myEnableVSync = aVsyncEnable;

		if (!Tga2D::Engine::GetInstance()->Start(createParameters))
		{
			ERROR_PRINT("Fatal error! Engine could not start!");
			system("pause");
	
		}
	}
}