#include <string>
#include <tga2d/engine.h>
#include <tga2d/error/ErrorManager.h>

#include <tga2d/input/XInput.h>
#include <tga2d/model/AnimatedModelInstance.h>
#include "../../TutorialCommon/TutorialCommon.h"
#include <tga2d/audio/audio.h>
#include <tga2d/math/CommonMath.h>
#include <tga2d/graphics/Camera.h>
#include <tga2d/graphics/dx11.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/drawers/ModelDrawer.h>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/model/modelfactory.h>

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

Tga2D::Audio myFootstepAudios[4];
Tga2D::Audio myShootAudio;

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
		ModelFactory& modelFactory = ModelFactory::GetInstance();
		Tga2D::AnimatedModelInstance model = modelFactory.GetAnimatedModel(L"../source/tutorials/Tutorial11SkinnedAnimation/data/ani/popp_sk.fbx");
		model.SetLocation({ 0.0f, 0.0f, 0.5f });
		model.SetScale(Vector3f(0.1f, 0.1f, 0.1f));
		XInput myInput;

		std::vector<std::wstring> animations;
		animations.push_back(L"../source/tutorials/Tutorial11SkinnedAnimation/data/ani/popp_sk.fbx");
		animations.push_back(L"../source/tutorials/Tutorial11SkinnedAnimation/data/ani/idle.fbx");
		animations.push_back(L"../source/tutorials/Tutorial11SkinnedAnimation/data/ani/run.fbx");
		animations.push_back(L"../source/tutorials/Tutorial11SkinnedAnimation/data/ani/jump.fbx");

		model.InitAnimations(animations);

		model.RegisterAnimationEventCallback("event_step", [] { myFootstepAudios[rand() % 3].Play(); });

		TextureResource* texture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"../source/tutorials/Tutorial11SkinnedAnimation/data/ani/atlas.tga");
		model.SetTexture(0, 0, texture);

		Vector2ui resolution = DX11::GetResolution();
		Tga2D::Camera camera;
		camera.SetOrtographicProjection(1.0, resolution.y / (float)resolution.x, 1.0f);
		Tga2D::Engine::GetInstance()->GetGraphicsEngine().SetCamera(camera);


		float timer = 0;

		const int IDLE_INDEX = 1;
		const int RUNNING_INDEX = 2;
		const int JUMPING_INDEX = 3;

		int myCurrentAnimIndex = IDLE_INDEX;
		while (true)
		{
			timer += 1.0f / 60.0f;
			if (!Tga2D::Engine::GetInstance()->BeginFrame())
			{
				break;
			}

			myInput.Refresh();

			ModelStatus modelStatus = model.Update(1.0f / 60.0f);

			if (myCurrentAnimIndex != JUMPING_INDEX)
			{
				if (myInput.leftStickX > 0.2f)
				{
					myCurrentAnimIndex = RUNNING_INDEX;
					model.SetScale(Vector3f(0.1f,0.1f,0.1f));
				}
				else if (myInput.leftStickX < -0.2f)
				{
					myCurrentAnimIndex = RUNNING_INDEX;
					model.SetScale(Vector3f(-0.1f, 0.1f, 0.1f));
				}
				else
				{
					myCurrentAnimIndex = IDLE_INDEX;
				}
			}
			else if (myCurrentAnimIndex == JUMPING_INDEX)
			{
				if (modelStatus == ModelStatus::Animation_End)
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

			DX11::BackBuffer->SetAsActiveTarget(DX11::DepthBuffer);
			Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetModelDrawer().Draw(model);

			Tga2D::Engine::GetInstance()->EndFrame();
		}
	}

	Tga2D::Engine::GetInstance()->Shutdown();

}
