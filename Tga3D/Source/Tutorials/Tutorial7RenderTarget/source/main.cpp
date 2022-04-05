#include <string>


#include "../../TutorialCommon/TutorialCommon.h"
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/engine.h>
#include <tga2d/light/LightManager.h>
#include <tga2d/graphics/DX11.h>
#include <tga2d/primitives/CustomShape.h>
#include <tga2d/shaders/SpriteShader.h>
#include <tga2d/texture/TextureManager.h>
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
		Tga2D::CustomShape myShape;
		myShape.Reset();
		myShape.AddPoint({ 0.0f, 0.0f }, Tga2D::Color(1, 0, 0, 1));
		myShape.AddPoint({ 1.0f, 0.0f }, Tga2D::Color(0, 1, 0, 1));
		myShape.AddPoint({ 0.0f, 1.0f }, Tga2D::Color(0, 0, 1, 1));

		myShape.AddPoint({ 1.0f, 0.0f }, Tga2D::Color(0, 1, 0, 1));
		myShape.AddPoint({ 1.0f, 1.0f }, Tga2D::Color(0, 0, 1, 1));
		myShape.AddPoint({ 0.0f, 1.0f }, Tga2D::Color(0, 0, 1, 1));
		myShape.BuildShape();

		Tga2D::Engine& engine = *Tga2D::Engine::GetInstance();

		//The target texture we will render to instead of the screen, this is a sprite, which means it have all the nice features of one (rotation, position etc.)
		Tga2D::RenderTarget myRenderTargetTexture = Tga2D::RenderTarget::Create(engine.GetTargetSize());
		Tga2D::RenderTarget& backBuffer = *Tga2D::DX11::BackBuffer;

		// Ordinary sprite that we will render onto the target
		Tga2D::SpriteSharedData logoSpriteSharedData = {};
		Tga2D::Texture* texture = engine.GetTextureManager().GetTexture(L"sprites/tga_logo.dds");
		logoSpriteSharedData.myTexture = texture;
		Tga2D::Vector2f logoSize = texture->mySize;

		// We need a couple of tga loggos!
		const int numberOfLoggos = 20;
		std::vector<Tga2D::Sprite2DInstanceData> logoInstances;
		for (int i = 0; i < numberOfLoggos; i++)
		{
			float randX = (float)(rand() % 100) * 0.01f;
			float randY = (float)(rand() % 100) * 0.01f;
			Tga2D::Sprite2DInstanceData logo;
			logo.myPosition = { randX , randY };
			logo.myPivot = { 0.5f, 0.5f };
			logo.mySize = { 0.5f * logoSize.myX, 0.5f * logoSize.myY };

			logoInstances.push_back(logo);
		}

		// Create a new shader to showcase the fullscreen shader with.
		Tga2D::SpriteShader customShader; // Create	
		customShader.Init(L"shaders/instanced_sprite_shader_vs.cso", L"Tutorial7/shaders/custom_sprite_pixel_shader_PS.cso");


		float timer = 0;
		while (true)
		{
			timer += 1.0f / 60.0f;
			if (!Tga2D::Engine::GetInstance()->BeginFrame(Tga2D::Color(0, 0, 0.7f, 1)))
			{
				break;
			}

			// Render background
			myShape.Render();

			// Set the new sprite as a target instead of the screen
			myRenderTargetTexture.SetAsActiveTarget();
			myRenderTargetTexture.Clear();

			// Render all the logos onto the sprite
			engine.GetGraphicsEngine().GetSpriteDrawer().Draw(logoSpriteSharedData, logoInstances.data(), logoInstances.size());

			// Set the target back to the screen
			backBuffer.SetAsActiveTarget();

			// Render the target sprite which holds a lot of loggos!
			{
				Tga2D::SpriteSharedData sharedData = {};
				sharedData.myTexture = &myRenderTargetTexture;
				sharedData.myCustomShader = &customShader;

				Tga2D::Sprite2DInstanceData instanceData = {};
				instanceData.myPosition = Tga2D::Vector2f(0.5f, 0.5f);
				instanceData.myPivot = Tga2D::Vector2f(0.5f, 0.5f);
				instanceData.mySize = Tga2D::Vector2f(0.7f, 0.7f);
				instanceData.myRotation = cosf(timer) * 0.1f;

				engine.GetGraphicsEngine().GetSpriteDrawer().Draw(sharedData, instanceData);
			}

			Tga2D::Engine::GetInstance()->EndFrame();
		}
	}

	Tga2D::Engine::GetInstance()->Shutdown();
}
