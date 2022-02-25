#include <string>
#include <tga2d/engine.h>
#include <tga2d/error/error_manager.h>

#include <tga2d/input/XInput.h>
#include <tga2d/model/FBXModel.h>
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
using namespace Tga2D;
void Go( void );
int main( const int /*argc*/, const char * /*argc*/[] )
{
    Go();
    return 0;
}

Tga2D::CAudio myFootstepAudios[4];
Tga2D::CAudio myShootAudio;

/* This function will trigger as soon as an animation is sending an event from the animation, could be anything! Footsteps, something spawning, anything really
The animators will add these events in spine, and set what should be send, maybe a string, maybe a float or integer, thats why there are so many in the function.
*/
void AnimCallback()
{
	
}

// Run with  Left thumb stick, jump with A, shoot with X
void Go(void)
{
	{
		TutorialCommon::Init(L"TGA2D: Tutorial 11");

		myFootstepAudios[0].Init("../source/tutorials/Tutorial11SkinnedAnimation/data/audio/footstep_wood_run_01.wav");
		myFootstepAudios[1].Init("../source/tutorials/Tutorial11SkinnedAnimation/data/audio/footstep_wood_run_02.wav");
		myFootstepAudios[2].Init("../source/tutorials/Tutorial11SkinnedAnimation/data/audio/footstep_wood_run_03.wav");
		myFootstepAudios[3].Init("../source/tutorials/Tutorial11SkinnedAnimation/data/audio/footstep_wood_run_04.wav");
		myShootAudio.Init("../source/tutorials/Tutorial11SkinnedAnimation/data/audio/sci-fi_weapon_blaster_laser_boom_small_02.wav");

		// Create the sprite with the path to the image
		Tga2D::FBXModel model;
		model.SetPosition({ 0.5f, 0.5f });
		CXInput myInput;

		std::vector<std::string> animations;
		animations.push_back("../source/tutorials/Tutorial11SkinnedAnimation/data/ani/idle.fbx");
		animations.push_back("../source/tutorials/Tutorial11SkinnedAnimation/data/ani/run.fbx");
		animations.push_back("../source/tutorials/Tutorial11SkinnedAnimation/data/ani/jump.fbx");

		model.Init("../source/tutorials/Tutorial11SkinnedAnimation/data/ani/popp_sk.fbx");
		model.InitAnimations(animations);

		model.RegisterAnimationEventCallback("event_step", [] { myFootstepAudios[rand() % 3].Play(); });

		//model.SetPosition({ 0.5f, 0.7f });

		float timer = 0;

		const int IDLE_INDEX = 1;
		const int RUNNING_INDEX = 2;
		const int JUMPING_INDEX = 3;

		int myCurrentAnimIndex = IDLE_INDEX;
		while (true)
		{
			timer += 1.0f / 60.0f;
			if (!Tga2D::CEngine::GetInstance()->BeginFrame())
			{
				break;
			}

			myInput.Refresh();

			EModelStatus modelStatus = model.Update(1.0f / 60.0f);

			if (myCurrentAnimIndex != JUMPING_INDEX)
			{
				if (myInput.leftStickX > 0.2f)
				{
					myCurrentAnimIndex = RUNNING_INDEX;
					model.SetFlipX(false);
				}
				else if (myInput.leftStickX < -0.2f)
				{
					myCurrentAnimIndex = RUNNING_INDEX;
					model.SetFlipX(true);
				}
				else
				{
					myCurrentAnimIndex = IDLE_INDEX;
				}
			}
			else if (myCurrentAnimIndex == JUMPING_INDEX)
			{
				if (modelStatus == EModelStatus::Animation_End)
				{
					myCurrentAnimIndex = IDLE_INDEX;
				}
			}

			if (myInput.IsPressed(XINPUT_GAMEPAD_A))
			{
				if (myCurrentAnimIndex != JUMPING_INDEX)
				{
					myCurrentAnimIndex = JUMPING_INDEX;
				}

			}

			model.SetCurrentAnimationIndex((unsigned short)myCurrentAnimIndex);

			model.Render();

			Tga2D::CEngine::GetInstance()->EndFrame();
		}
	}

	Tga2D::CEngine::GetInstance()->Shutdown();

}
