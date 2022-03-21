#include <string>
#include <tga2d/engine.h>
#include <tga2d/error/ErrorManager.h>

#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/drawers/SpriteDrawer.h>
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



// This is where the application starts of for real. By keeping this in it's own file
// we will have the same behaviour for both console and windows startup of the
// application.
//
void Go( void )
{
	{
		TutorialCommon::Init(L"TGA2D: Tutorial 3");

		Tga2D::Engine& engine = *Tga2D::Engine::GetInstance();

		// Get a sprite drawer, for drawing sprites
		Tga2D::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());

		Tga2D::SpriteSharedData sharedData;
		sharedData.myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"sprites/tga_logo.dds");

		std::vector<Tga2D::Sprite2DInstanceData> instances;
		for (unsigned int i = 0; i < 100000; i++)
		{
			float randomX = static_cast<float>(rand() % 1000) / 1000.0f;
			float randomY = static_cast<float>(rand() % 1000) / 1000.0f;

			Tga2D::Sprite2DInstanceData instance = {};
			instance.myPosition = Tga2D::Vector2f(randomX, randomY);
			instance.myColor = Tga2D::Color(randomX, randomY, randomX, 1);
			instance.mySize = Tga2D::Vector2f(0.1f, 0.1f);
			instances.push_back(instance);
		}

		// MAIN LOOP
		while (true)
		{
			if (!Tga2D::Engine::GetInstance()->BeginFrame())
			{
				break;
			}

			spriteDrawer.Draw(sharedData, &instances[0], instances.size());

			Tga2D::Engine::GetInstance()->EndFrame();
		}
	}

	Tga2D::Engine::GetInstance()->Shutdown();
}
