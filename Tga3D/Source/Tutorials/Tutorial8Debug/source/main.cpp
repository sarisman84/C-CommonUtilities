#include <string>

#include <tga2d/drawers/DebugDrawer.h>
#include <tga2d/math/vector2.h>
#include <tga2d/engine.h>
#include "../../TutorialCommon/TutorialCommon.h"


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
		TutorialCommon::Init(L"TGA2D: Tutorial 8", true);

		Tga2D::Vector2f targetPos;

		const float deltaTime = 1.0f / 60.0f;
		float timer = 0.0f;
		while (true)
		{
			timer += deltaTime;
			if (!Tga2D::Engine::GetInstance()->BeginFrame())
			{
				break;
			}

			targetPos.Set(0.5f, (cosf(timer) * 0.2f) + 0.5f);

			// Arrow
			{
				Tga2D::Vector2f from(0, 0.5f);
				Tga2D::Vector2f to(0.5f, 0.5f);
				Tga2D::Color color(1, 0, 0, 1);
				Tga2D::Engine::GetInstance()->GetDebugDrawer().DrawArrow(from, targetPos, color);
			}

			//Circle
			{
				Tga2D::Engine::GetInstance()->GetDebugDrawer().DrawCircle({ 0.3f, 0.4f }, 0.03f + (cosf(timer) * 0.1f), Tga2D::Color(cosf(timer), -cosf(timer * 3), sinf(timer), 1));
			}

			//Lines
			struct Line
			{
				Tga2D::Vector2f myFrom;
				Tga2D::Vector2f myTo;
			};

			// Single Line
			{
				Line line;
				line.myFrom.Set(0.5f, 0.1f);
				line.myTo.Set(0.6f, 0.3f);
				Tga2D::Engine::GetInstance()->GetDebugDrawer().DrawLine(line.myFrom, line.myTo, { 1, 1, 1, 1 });
			}

			// Multiple lines, ONE drawcall!
			{
				Tga2D::Vector2f from[6];
				Tga2D::Vector2f to[6];
				Tga2D::Color colors[6];

				from[0].Set(0.5f, 0.4f);
				to[0].Set(0.5f, 0.6f);
				colors[0].Set(1, 1, 1, 1);

				from[1].Set(0.5f, 0.4f);
				to[1].Set(0.6f, 0.4f);
				colors[1].Set(1, 1, 1, 1);

				from[2].Set(0.6f, 0.4f);
				to[2].Set(0.6f, 0.6f);
				colors[2].Set(1, 1, 1, 1);

				from[3].Set(0.6f, 0.6f);
				to[3].Set(0.5f, 0.6f);
				colors[3].Set(1, 1, 1, 1);

				from[4].Set(0.5f, 0.4f);
				to[4].Set(0.6f, 0.6f);
				colors[4].Set(1, 1, 1, 1);

				for (int i = 0; i < 5; i++)
				{
					from[i].y += targetPos.y - 0.5f;
					to[i].y += targetPos.y - 0.5f;
				}

				Tga2D::Engine::GetInstance()->GetDebugDrawer().DrawLines(from, to, colors, 5);
			}


			Tga2D::Engine::GetInstance()->EndFrame();
		}
		Tga2D::Engine::GetInstance()->Shutdown();
	}
}
