/*
Hello and welcome!
This project is the project which creates the executable file, dont do any gameplay in this project, do tht in the "Game" project :)
*/

#define NOMINMAX // to avoid windows defining a min macro, causing conflicts with std::min
#include <Windows.h>
#include "Game.h"
#include "VersionNo.h"
#include <string>
#include <stringapiset.h>
#include <tga2d/engine.h>

void Init();

#ifdef _DEBUG
#pragma comment(lib,"Game_Debug.lib")
#endif // DEBUG
#ifdef _RELEASE
#pragma comment(lib,"Game_Release.lib")
#endif // _RELEASE
#ifdef _RETAIL
#pragma comment(lib,"Game_Retail.lib")
#endif // _RETAIL


// Comment out below define to disable command line

#ifndef _RETAIL
#define USE_CONSOLE_COMMAND
#endif

#pragma region WindowsInit
void InitConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
#pragma warning( pop )
}

void CloseConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
	fclose(stdin); 
	fclose(stdout);
	fclose(stderr);
#pragma warning( pop )
}

int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, char*, int /*nShowCmd*/)
{	
#ifdef USE_CONSOLE_COMMAND
	InitConsole();
#endif

	Init();

#ifdef USE_CONSOLE_COMMAND
	CloseConsole();
#endif
	return 0;
}
#pragma endregion MainFunction


void Init() 
{ 
	{
		CGame myGame;

		int version[] = { PRODUCTVER };
		std::wstring versionNumber;

		int count = sizeof(version) / sizeof(version[0]);
		for (int i = 0; i < count; i++)
		{
			versionNumber += std::to_wstring(version[i]);
			versionNumber += L",";
		}
		versionNumber.pop_back();

		myGame.Init(versionNumber); // Blocking

		if (Tga2D::CEngine::GetInstance())
			Tga2D::CEngine::GetInstance()->RunUpdateLoop();
	}

	if (Tga2D::CEngine::GetInstance())
		Tga2D::CEngine::GetInstance()->Shutdown();
}


