#include <string>
#include <tga2d/engine.h>
#include <tga2d/error/error_manager.h>

#include <tga2d/d3d/direct_3d.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/drawers/sprite_drawer.h>
#include <tga2d/texture/texture_manager.h>
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

		Tga2D::CEngine& engine = *Tga2D::CEngine::GetInstance();

		// Get a sprite drawer, for drawing sprites
		Tga2D::CSpriteDrawer& spriteDrawer(engine.GetDirect3D().GetSpriteDrawer());

		Tga2D::SSpriteSharedData sharedData;
		sharedData.myTexture = Tga2D::CEngine::GetInstance()->GetTextureManager().GetTexture("sprites/tga_logo.dds");

		std::vector<Tga2D::SSpriteInstanceData> instances;
		for (unsigned int i = 0; i < 100000; i++)
		{
			float randomX = static_cast<float>(rand() % 1000) / 1000.0f;
			float randomY = static_cast<float>(rand() % 1000) / 1000.0f;

			Tga2D::SSpriteInstanceData instance = {};
			instance.myPosition = VECTOR2F(randomX, randomY);
			instance.myColor = Tga2D::CColor(randomX, randomY, randomX, 1);
			instance.mySize = VECTOR2F(0.1f, 0.1f);
			instances.push_back(instance);
		}

		// MAIN LOOP
		while (true)
		{
			if (!Tga2D::CEngine::GetInstance()->BeginFrame())
			{
				break;
			}

			spriteDrawer.Draw(sharedData, &instances[0], instances.size());

			Tga2D::CEngine::GetInstance()->EndFrame();
		}
	}

	Tga2D::CEngine::GetInstance()->Shutdown();
}
