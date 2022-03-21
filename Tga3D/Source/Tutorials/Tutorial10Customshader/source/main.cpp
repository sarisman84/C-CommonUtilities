#include <string>
#include <tga2d/engine.h>
#include <tga2d/error/ErrorManager.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/engine.h>
#include <tga2d/primitives/CustomShape.h>
#include <tga2d/shaders/SpriteShader.h>
#include <tga2d/texture/TextureManager.h>
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
		TutorialCommon::Init(L"TGA2D: Tutorial 10 - Shaders from shadertoy.com", true);

		// Create the sprite WITHOUT a image!
		Tga2D::SpriteSharedData sprite1Shared = {}, sprite2Shared = {}, sprite3Shared = {};
		Tga2D::Sprite2DInstanceData sprite1Instance = {}, sprite2Instance = {}, sprite3Instance = {};

		// Setting the pivot so all operations will be in the middle of the image (rotation, position, etc.)
		sprite1Instance.myPivot = Tga2D::Vector2f(0.5f, 0.5f);
		sprite2Instance.myPivot = Tga2D::Vector2f(0.5f, 0.5f);
		sprite3Instance.myPivot = Tga2D::Vector2f(0.5f, 0.5f);

		/////////// Custom shader

		// Create a new shader
		Tga2D::SpriteShader customShader; // Create
		customShader.SetShaderdataFloat4(Tga2D::Vector4f(1, 0, 0, 1), Tga2D::ShaderDataID_1); // Add some data to it
		customShader.SetTextureAtRegister(Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"sprites/tga_logo.dds"), Tga2D::ShaderTextureSlot_1); // Add a texture

		// Run PostInit to set all the data
		customShader.Init(L"shaders/instanced_sprite_shader_vs.cso", L"tutorial10/shaders/custom_sprite_pixel_shader_ps.cso");

		// Tell the sprite to use this shader
		sprite1Shared.myCustomShader = &customShader;
		sprite1Instance.myPosition = Tga2D::Vector2f(0.3f, 0.5f);
		sprite1Instance.mySize = { 0.3f, 0.3f };


		// Second sprite
		Tga2D::SpriteShader customShader2; // Create
		customShader2.SetShaderdataFloat4(Tga2D::Vector4f(1, 0, 1, 1), Tga2D::ShaderDataID_1); // Add some data to it
		customShader2.SetTextureAtRegister(Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"sprites/tga_logo.dds"), Tga2D::ShaderTextureSlot_1); // Add a texture																																				   // Run PostInit to set all the data
		customShader2.Init(L"shaders/instanced_sprite_shader_vs.cso", L"tutorial10/shaders/custom_sprite_pixel_shader_2_ps.cso");

		// Tell the sprite to use this shader
		sprite2Shared.myCustomShader = &customShader2;
		sprite2Instance.myPosition = Tga2D::Vector2f(0.5f, 0.5f);
		sprite2Instance.mySize = { 0.3f, 0.3f };

		// Third
		Tga2D::SpriteShader customShader3; // Create
		customShader3.SetShaderdataFloat4(Tga2D::Vector4f(1, 0, 1, 1), Tga2D::ShaderDataID_1); // Add some data to it
		customShader3.SetTextureAtRegister(Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"sprites/tga_logo.dds"), Tga2D::ShaderTextureSlot_1); // Add a texture																																				   // Run PostInit to set all the data
		customShader3.Init(L"shaders/instanced_sprite_shader_vs.cso", L"tutorial10/shaders/custom_sprite_pixel_shader_3_ps.cso");

		// Tell the sprite to use this shader
		sprite3Shared.myCustomShader = &customShader3;
		sprite3Instance.myPosition = Tga2D::Vector2f(0.7f, 0.5f);
		sprite3Instance.mySize = { 0.3f, 0.3f };

		float timer = 0;
		while (true)
		{
			timer += 1.0f / 60.0f;
			if (!Tga2D::Engine::GetInstance()->BeginFrame())
			{
				break;
			}

			Tga2D::SpriteDrawer& spriteDrawer = Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer();

			spriteDrawer.Draw(sprite1Shared, sprite1Instance);
			spriteDrawer.Draw(sprite2Shared, sprite2Instance);
			spriteDrawer.Draw(sprite3Shared, sprite3Instance);

			Tga2D::Engine::GetInstance()->EndFrame();
		}
	}

	Tga2D::Engine::GetInstance()->Shutdown();
}
