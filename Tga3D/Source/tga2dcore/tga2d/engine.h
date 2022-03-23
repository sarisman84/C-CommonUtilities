/*
Engine
This class is the interface between game logic and rendering. 
It takes care of creating everything necessary for window handling and rendering
*/

#pragma once

#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX 
#include <windows.h>

#include <functional>
#include <tga2d/math/color.h>
#include <tga2d/math/vector2.h>
#include <tga2d/render/RenderCommon.h>
#include <chrono>
#include "StepTimer.h"
#include <tga2d/EngineDefines.h>

namespace Tga2D
{
    class WindowsWindow;
    class GraphicsEngine;
    class Renderer;
    class TextureManager;
    class DebugDrawer;
    class TextService;
    class LightManager;
    class ErrorManager;
    class FileWatcher;
    class ImGuiInterface;
    class DX11;
}


#define TGA_DEFAULT_CRYSTAL_BLUE { 3.0f / 255.0f, 153.0f / 255.0f, 176.0f / 255.0f, 1.0f }
namespace Tga2D
{
    using callback_function        = std::function<void()>;
    using callback_function_update = std::function<void()>;
	using callback_function_wndProc = std::function<LRESULT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>;
    using callback_function_log    = std::function<void( std::string )>;
    using callback_function_error  = std::function<void( std::string )>;

    enum class DebugFeature
    {
        None = 0,
        Fps = 1 << 0,
        Mem = 1 << 1,
        Drawcalls = 1 << 2,
        Cpu = 1 << 3,
        FpsGraph = 1 << 4,
        Filewatcher = 1 << 5,
        OptimizeWarnings = 1 << 6,
        MemoryTrackingStackTraces = 1 << 7,
        MemoryTrackingAllAllocations =  1 << 8,
        All = (1 << 9) - 1,
    };
    inline DebugFeature operator|(DebugFeature lhs, DebugFeature rhs)
    {
        return static_cast<DebugFeature>(static_cast<std::underlying_type<DebugFeature>::type>(lhs) | static_cast<std::underlying_type<DebugFeature>::type>(rhs));
    }
    inline DebugFeature operator&(DebugFeature lhs, DebugFeature rhs)
    {
        return static_cast<DebugFeature>(static_cast<std::underlying_type<DebugFeature>::type>(lhs) & static_cast<std::underlying_type<DebugFeature>::type>(rhs));
    }

    enum class WindowSetting
    {
        Overlapped,
        Borderless,
    };

	enum class MultiSamplingQuality
	{
		Off,	
		Low = 1,
		Medium = 2,
		High = 3,
	};

    struct EngineCreateParameters
    {
        EngineCreateParameters()
        { 
            myHwnd							= nullptr; 
            myHInstance						= nullptr;  
            myWindowWidth					= 1280;
            myWindowHeight					= 720; 
            myEnableVSync					= false; 
            myRenderWidth					= myWindowWidth; 
            myRenderHeight					= myWindowHeight; 
            myErrorFunction					= nullptr; 
			myLogFunction					= nullptr;
			myWinProcCallback				= nullptr;
            myStartInFullScreen				= false;
            myWindowSetting					= WindowSetting::Overlapped;
            myTargetHeight					= myWindowHeight;
            myTargetWidth					= myWindowWidth;
            myUseLetterboxAndPillarbox		= true;
            myActivateDebugSystems			= DebugFeature::Fps | DebugFeature::Mem;
			myPreferedMultiSamplingQuality	= MultiSamplingQuality::Off;
			myClearColor					= TGA_DEFAULT_CRYSTAL_BLUE;
        }

        callback_function myInitFunctionToCall;
        callback_function_update myUpdateFunctionToCall;
		callback_function_wndProc myWinProcCallback;
        callback_function_log myLogFunction;
        callback_function_error myErrorFunction;

		/* How big should the window be? */
        unsigned short myWindowWidth;
        unsigned short myWindowHeight;

		/* What resolution should we render everything in?*/
        unsigned short myRenderWidth;
        unsigned short myRenderHeight;

		/* In what resolution are the graphic artist working?*/
        unsigned short myTargetHeight;
        unsigned short myTargetWidth;

        /* Will show the FPS and memory text*/
        DebugFeature myActivateDebugSystems;
        Color myClearColor;
        HWND *myHwnd;
        HINSTANCE myHInstance;
        std::wstring myApplicationName;
        bool myEnableVSync;
        bool myStartInFullScreen;
        WindowSetting myWindowSetting;
        bool myUseLetterboxAndPillarbox;

		MultiSamplingQuality myPreferedMultiSamplingQuality;
    };


    class Engine
    {
        friend class TextureManager;
        friend class Renderer;
        friend class Shader;
        friend class ErrorManager;
        friend class GraphicsEngine;
		friend class WindowsWindow;
		friend class ErrorManager;
		friend class CustomShapeDrawer;
		friend class LineDrawer;
		friend class DebugDrawer;
		friend class CustomShape;
		friend class LinePrimitive;
		friend class Texture;
		friend class Text;
		friend class TextService;
		friend class Video;
		friend class LineMultiPrimitive;
		friend class Light;
		friend class ModelShape;
        friend class HTML_Parser;

    public:
        Engine &operator =( const Engine &anOther ) = delete;
		static bool Start(const EngineCreateParameters& aCreateParameters);
		static void Shutdown();
        static Engine* GetInstance() {return myInstance;}
        
        HTML_Parser& GetHTMLParser() const { return *myHTMLParser; }
        TextureManager& GetTextureManager() const {return *myTextureManager;}
        DebugDrawer& GetDebugDrawer() const {return *myDebugDrawer;}

		ErrorManager& GetErrorManager() const { return *myErrorManager; }

		Vector2ui GetWindowSize() const {return myWindowSize;}
		Vector2ui GetRenderSize() const { return myRenderSize; }
		Vector2ui GetTargetSize() const { return myTargetSize; }
        float GetWindowRatio() const;
        float GetWindowRatioInversed() const;

		float GetDeltaTime() const { return myDeltaTime; }
        float GetTotalTime() const { return myTotalTime; }

        HWND* GetHWND() const;
        HINSTANCE GetHInstance() const;
      
		bool IsDebugFeatureOn(DebugFeature aFeature) const;

		void SetTargetSize(const Vector2ui& aResolution);
		void SetResolution(const Vector2ui &aResolution, bool aAlsoSetWindowSize = true);
        void SetFullScreen(bool aFullScreen);
		
		void SetClearColor(const Color& aClearColor); 
		// If you want to manually tell the engine to render instead of the callback function with the (myUpdateFunctionToCall)
		bool BeginFrame(const Color &aClearColor = TGA_DEFAULT_CRYSTAL_BLUE);
		void EndFrame(void);

        LightManager& GetLightManager() const { return *myLightManager; }

		void SetAmbientLightValue(float aAmbientLight);
		GraphicsEngine& GetGraphicsEngine() const { return *myGraphicsEngine; }
    private:
		static void DestroyInstance();

		FileWatcher* GetFileWatcher() { return myFileWatcher.get(); }
		
		Renderer& GetRenderer() const { return *myRenderer; }
		TextService& GetTextService() const { return *myTextService; }
		WindowsWindow& GetWindow() const { return *myWindow; }
        Engine(const EngineCreateParameters& aCreateParameters);
        ~Engine();
		
		bool InternalStart();

        void StartStep();
        void DoStep();
		void IdleProcess();
        void CalculateRatios();
		void SetViewPort(float aTopLeftX, float aTopLeftY, float aWidth, float aHeight, float aMinDepth = 0.0f, float aMaxDepth = 1.0f, bool aSetEngineResolution = false);
		void SetWantToUpdateSize() { myWantToUpdateSize = true; }

		void UpdateWindowSizeChanges(bool aIgnoreAutoUpdate = false);

		Vector2f GetWindowRatioVec() const;
		Vector2f GetWindowRatioInversedVec() const;

        static Engine* myInstance;

        callback_function_update myUpdateFunctionToCall;
        callback_function myInitFunctionToCall;
        std::unique_ptr<WindowsWindow> myWindow;
        std::unique_ptr<DX11> myDx11;
        std::unique_ptr<GraphicsEngine> myGraphicsEngine;
        std::unique_ptr<Renderer> myRenderer;
        std::unique_ptr<TextureManager> myTextureManager;
        std::unique_ptr<DebugDrawer> myDebugDrawer;
        std::unique_ptr<TextService> myTextService;
        std::unique_ptr<HTML_Parser> myHTMLParser;
		Vector2ui myWindowSize;
		Vector2ui myRenderSize;
		Vector2ui myTargetSize;
		Vector2ui myNativeRenderSize;
        HWND *myHwnd;
        HINSTANCE myHInstance;
        Color myClearColor;
        EngineCreateParameters myCreateParameters;
        std::unique_ptr<LightManager> myLightManager;
        std::unique_ptr<ErrorManager> myErrorManager;
        std::unique_ptr<FileWatcher> myFileWatcher;
		bool myWantToUpdateSize;

        bool myRunEngine;
        float myWindowRatio;
        float myWindowRatioInversed;
		Vector2f myWindowRatioVec;
		Vector2f myWindowRatioInversedVec;

		std::chrono::steady_clock::time_point myStartOfTime;
        float myTotalTime;
        float myDeltaTime;

		DX::StepTimer myTimer;

		bool myShouldExit; // Only used when using beginframe and endframe
        std::unique_ptr<ImGuiInterface> myImguiInterFace = nullptr;
    };
}
