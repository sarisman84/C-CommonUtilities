#include <string>
#include <tga2d/engine.h>
#include <tga2d/error/error_manager.h>

#include "../../TutorialCommon/TutorialCommon.h"
#include <tga2d/input/XInput.h>
#include <tga2d/drawers/debug_drawer.h>


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
using namespace Tga2D;

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
		TutorialCommon::Init(L"TGA2D: Tutorial 13, Gamepad");

		CXInput myInput;
		CDebugDrawer& debugDrawer = Tga2D::CEngine::GetInstance()->GetDebugDrawer();
		float timer = 0;
		while (true)
		{
			timer += 1.0f / 60.0f;
			if (!Tga2D::CEngine::GetInstance()->BeginFrame())
			{
				break;
			}

			debugDrawer.DrawCircle({ 0.45f, 0.3f }, 0.01f, myInput.IsConnected(0) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));
			debugDrawer.DrawCircle({ 0.45f, 0.3f }, 0.005f, myInput.IsConnected(0) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));

			// left Thumb
			float leftX = (myInput.leftStickX) * 0.1f + 0.3f;
			float leftY = (-myInput.leftStickY) * (0.1f * Tga2D::CEngine::GetInstance()->GetWindowRatio()) + 0.5f;
			debugDrawer.DrawCircle({ leftX,leftY }, 0.01f);
			debugDrawer.DrawCircle({ 0.3f, 0.5f }, 0.1f);

			float rightX = (myInput.rightStickX) * 0.1f + 0.6f;
			float rightY = (-myInput.rightStickY) * (0.1f * Tga2D::CEngine::GetInstance()->GetWindowRatio()) + 0.5f;
			debugDrawer.DrawCircle({ rightX,rightY }, 0.01f);
			debugDrawer.DrawCircle({ 0.6f, 0.5f }, 0.1f);


			debugDrawer.DrawCircle({ 0.45f, 0.4f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_A) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));
			debugDrawer.DrawCircle({ 0.45f, 0.44f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_B) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));
			debugDrawer.DrawCircle({ 0.45f, 0.48f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_X) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));
			debugDrawer.DrawCircle({ 0.45f, 0.52f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_Y) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));

			debugDrawer.DrawCircle({ 0.45f, 0.58f }, 0.01f, CColor(myInput.leftTrigger, 1, 0, 1));
			debugDrawer.DrawCircle({ 0.45f, 0.62f }, 0.01f, CColor(myInput.rightTrigger, 1, 0, 1));

			debugDrawer.DrawCircle({ 0.42f, 0.66f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_LEFT_SHOULDER) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));
			debugDrawer.DrawCircle({ 0.48f, 0.66f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));


			debugDrawer.DrawCircle({ 0.42f, 0.70f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_LEFT_THUMB) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));
			debugDrawer.DrawCircle({ 0.48f, 0.70f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_RIGHT_THUMB) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));

			debugDrawer.DrawCircle({ 0.45f, 0.78f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_DPAD_UP) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));
			debugDrawer.DrawCircle({ 0.45f, 0.82f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_DPAD_DOWN) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));
			debugDrawer.DrawCircle({ 0.42f, 0.80f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_DPAD_LEFT) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));
			debugDrawer.DrawCircle({ 0.48f, 0.80f }, 0.01f, myInput.IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT) ? CColor(0, 1, 0, 1) : CColor(1, 0, 0, 1));

			myInput.Refresh();

			Tga2D::CEngine::GetInstance()->EndFrame();
		}
	}

	Tga2D::CEngine::GetInstance()->Shutdown();
}
