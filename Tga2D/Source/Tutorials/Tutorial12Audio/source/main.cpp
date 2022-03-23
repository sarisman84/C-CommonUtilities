#include <string>
#include <tga2d/engine.h>
#include <tga2d/error/error_manager.h>

#include "../../TutorialCommon/TutorialCommon.h"
#include <tga2d/audio/audio.h>


// Making sure that DX2DEngine lib is linked
//
#ifdef _DEBUG
#pragma comment(lib,"tga2dcore_Debug.lib")
#pragma comment(lib,"External_Debug.lib")
#else
#pragma comment(lib,"tga2dcore_Release.lib")
#pragma comment(lib,"External_Release.lib")
#endif // _DEBUG

using namespace std::placeholders;

void Go( void );
int main( const int /*argc*/, const char * /*argc*/[] )
{
    Go();
    return 0;
}


// Run with  Left thumb stick, jump with A, shoot with X
void Go( void )
{
	{
		TutorialCommon::Init(L"TGA2D: Tutorial 12, Audio");

		Tga2D::CAudio audio;
		audio.Init("../source/tutorials/Tutorial12Audio/data/music.ogg", true);
		audio.Play();
		float timer = 0;
		while (true)
		{
			timer += 1.0f / 60.0f;
			if (!Tga2D::CEngine::GetInstance()->BeginFrame())
			{
				break;
			}

			POINT p;
			GetCursorPos(&p);
			ScreenToClient(*Tga2D::CEngine::GetInstance()->GetHWND(), &p);

			float panX = static_cast<float>(p.x) / static_cast<float>(Tga2D::CEngine::GetInstance()->GetWindowSize().x);
			// Go from 0-1 space to -1-1
			panX *= 2.0f;
			panX -= 1.0f;

			float panY = 0; // In 2D, y is nothing if we use stereo

			audio.SetPosition({ panX , panY });

			Tga2D::CEngine::GetInstance()->EndFrame();
		}
	}

	Tga2D::CEngine::GetInstance()->Shutdown();

}
