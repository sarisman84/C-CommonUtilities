/*
This class handles the creation of the actual window
*/

#pragma once
#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX 
#include <windows.h>
#include <tga2d/engine.h>

namespace Tga2D
{
	struct EngineCreateParameters;
	class WindowsWindow
	{
	public:
		WindowsWindow(void);
		~WindowsWindow(void);
		bool Init(Vector2ui aWindowSize, HWND*& aHwnd, EngineCreateParameters* aSetting, HINSTANCE& aHInstanceToFill, callback_function_wndProc aWndPrcCallback);
		HWND GetWindowHandle() const {return myWindowHandle;}
		void SetResolution(Vector2ui aResolution);
		void Close();
		unsigned int GetWidth()
		{
			return myResolution.X;
		}
		unsigned int  GetHeight()
		{
			return myResolution.Y;
		}
	private:
		LRESULT LocWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		HWND myWindowHandle;
		WNDCLASSEX myWindowClass;
		callback_function_wndProc myWndProcCallback;
		Vector2ui myResolution;
		Vector2ui myResolutionWithBorderDifference;
	};
}
