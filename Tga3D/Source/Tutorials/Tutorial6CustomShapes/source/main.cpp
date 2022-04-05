#include <string>

#include "../../TutorialCommon/TutorialCommon.h"
#include <tga2d/sprite/sprite.h>
#include <tga2d/engine.h>
#include <tga2d/light/LightManager.h>
#include <tga2d/primitives/CustomShape.h>
#include "../SnakeShape.h"
#include "../CMayaObject.h"

// Making sure that DX2DEngine lib is linked
//
#ifdef _DEBUG
#pragma comment(lib,"tga2dcore_Debug.lib")
#pragma comment(lib,"External_Debug.lib")
#else
#pragma comment(lib,"tga2dcore_Release.lib")
#pragma comment(lib,"External_Release.lib")
#endif // _DEBUG


void Go( void );
int main( const int /*argc*/, const char * /*argc*/[] )
{
    Go();

    return 0;
}

void Go( void )
{
	{
		TutorialCommon::Init(L"TGA2D: Tutorial 6", true);

		Tga2D::CustomShape myShape;
		CSnakeShape mySnake;
		mySnake.Init(0.6f);

		CMayaObject myMayaObject;
		myMayaObject.Init("../source/tutorials/Tutorial6CustomShapes/source/from_maya.obj");

		float timer = 0;
		while (true)
		{
			timer += 1.0f / 60.0f;
			//timer *= 0.1f;
			
			{
				//PIXScopedEvent(0xffffff, "BeginFrame");
				if (!Tga2D::Engine::GetInstance()->BeginFrame())
				{
					break;
				}
			}

			{
				{
					myShape.Reset();
					myShape.AddPoint({ 0.3f, 0.5f }, Tga2D::Color(1, 0, 0, 1));
					myShape.AddPoint({ 0.7f, 0.5f }, Tga2D::Color(0, 1, 0, 1));
					myShape.AddPoint({ (cosf(timer) * 0.1f) + 0.5f,  (sinf(timer) * 0.3f) + 0.5f }, Tga2D::Color(0, 0, 1, 1));
					myShape.BuildShape();

					myShape.Render();
				}

				{
					mySnake.Update(1.0f / 60.0f);
				}

				{
					myMayaObject.Render(1.0f / 60.0f);
				}
			}

			{
				Tga2D::Engine::GetInstance()->EndFrame();
			}
		}
	}

	Tga2D::Engine::GetInstance()->Shutdown();
}
