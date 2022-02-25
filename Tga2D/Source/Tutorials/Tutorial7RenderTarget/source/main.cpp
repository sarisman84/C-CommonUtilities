#include <string>


#include "../../TutorialCommon/TutorialCommon.h"
#include <tga2d/drawers/sprite_drawer.h>
#include <tga2d/d3d/direct_3d.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/light/light.h>
#include <tga2d/engine.h>
#include <tga2d/light/light_manager.h>
#include <tga2d/primitives/custom_shape.h>
#include <tga2d/shaders/sprite_shader.h>
#include <tga2d/texture/texture_manager.h>
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
		TutorialCommon::Init(L"TGA2D: Tutorial 7");

		// Background, not needed, but beatiful
		Tga2D::CCustomShape myShape;
		myShape.Reset();
		myShape.AddPoint({ 0.0f, 0.0f }, Tga2D::CColor(1, 0, 0, 1));
		myShape.AddPoint({ 1.0f, 0.0f }, Tga2D::CColor(0, 1, 0, 1));
		myShape.AddPoint({ 0.0f, 1.0f }, Tga2D::CColor(0, 0, 1, 1));

		myShape.AddPoint({ 1.0f, 0.0f }, Tga2D::CColor(0, 1, 0, 1));
		myShape.AddPoint({ 0.0f, 1.0f }, Tga2D::CColor(0, 0, 1, 1));
		myShape.AddPoint({ 1.0f, 1.0f }, Tga2D::CColor(0, 0, 1, 1));
		myShape.BuildShape();

		Tga2D::CEngine& engine = *Tga2D::CEngine::GetInstance();

		//The target texture we will render to instead of the screen, this is a sprite, which means it have all the nice features of one (rotation, position etc.)
		std::unique_ptr<Tga2D::CRenderTarget> myRenderTargetTexture = engine.GetTextureManager().CreateRenderTarget(engine.GetTargetSize());

		// Ordinary sprite that we will render onto the target
		Tga2D::SSpriteSharedData logoSpriteSharedData = {};
		logoSpriteSharedData.myTexture = engine.GetTextureManager().GetTexture("sprites/tga_logo.dds");
		VECTOR2F logoSize = logoSpriteSharedData.myTexture->mySize;

		// We need a couple of tga loggos!
		const int numberOfLoggos = 20;
		std::vector<Tga2D::SSpriteInstanceData> logoInstances;
		for (int i = 0; i < numberOfLoggos; i++)
		{
			float randX = (float)(rand() % 100) * 0.01f;
			float randY = (float)(rand() % 100) * 0.01f;
			Tga2D::SSpriteInstanceData logo;
			logo.myPosition = { randX , randY };
			logo.myPivot = { 0.5f, 0.5f };
			logo.mySize = { 0.5f * logoSize.myX, 0.5f * logoSize.myY };

			logoInstances.push_back(logo);
		}

		// Create a new shader to showcase the fullscreen shader with.
		Tga2D::CSpriteShader customShader; // Create	
		customShader.Init("shaders/instanced_sprite_shader_vs.fx", "../source/tutorials/Tutorial7RenderTarget//custom_sprite_pixel_shader.fx");

		float timer = 0;
		while (true)
		{
			timer += 1.0f / 60.0f;
			if (!Tga2D::CEngine::GetInstance()->BeginFrame(Tga2D::CColor(0, 0, 0.7f, 1)))
			{
				break;
			}

			// Render background
			myShape.Render();

			// Set the new sprite as a target instead of the screen
			Tga2D::CEngine::GetInstance()->SetRenderTarget(myRenderTargetTexture.get());
			// Render all the logos onto the sprite
			engine.GetDirect3D().GetSpriteDrawer().Draw(logoSpriteSharedData, logoInstances.data(), logoInstances.size());

			// Set the target back to the screen
			Tga2D::CEngine::GetInstance()->SetRenderTarget(nullptr);

			// Render the target sprite which holds a lot of loggos!
			{
				Tga2D::SSpriteSharedData sharedData = {};
				sharedData.myTexture = myRenderTargetTexture.get();
				sharedData.myCustomShader = &customShader;

				Tga2D::SSpriteInstanceData instanceData = {};
				instanceData.myPosition = VECTOR2F(0.5f, 0.5f);
				instanceData.myPivot = VECTOR2F(0.5f, 0.5f);
				instanceData.mySize = VECTOR2F(0.7f, 0.7f);
				instanceData.myRotation = cosf(timer) * 0.1f;

				engine.GetDirect3D().GetSpriteDrawer().Draw(sharedData, instanceData);
			}

			Tga2D::CEngine::GetInstance()->EndFrame();
		}
	}

	Tga2D::CEngine::GetInstance()->Shutdown();
}
