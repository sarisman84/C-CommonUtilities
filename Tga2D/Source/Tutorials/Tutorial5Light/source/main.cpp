#include <string>


#include "../../TutorialCommon/TutorialCommon.h"
#include <tga2d/d3d/direct_3d.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/drawers/sprite_drawer.h>
#include <tga2d/texture/texture_manager.h>
#include <tga2d/light/light.h>
#include <tga2d/engine.h>
#include <tga2d/light/light_manager.h>

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
		TutorialCommon::Init(L"TGA2D: Tutorial 5");

		Tga2D::CEngine& engine = *Tga2D::CEngine::GetInstance();
		Tga2D::CSpriteDrawer& spriteDrawer(engine.GetDirect3D().GetSpriteDrawer());

		Tga2D::SSpriteSharedData spriteSharedData = {};
		spriteSharedData.myTexture = engine.GetTextureManager().GetTexture("sprites/tga_logo.dds");

		// Set normal map
		spriteSharedData.myMaps[Tga2D::NORMAL_MAP] = engine.GetTextureManager().GetTexture("../source/tutorials/Tutorial5Light/data/tga_logo_n.dds");

		// Create instance data. 
		Tga2D::SSpriteInstanceData spriteInstance = {};

		// Setting the pivot so all operations will be in the middle of the image (rotation, position, etc.)
		spriteInstance.myPivot = VECTOR2F(0.5f, 0.5f);
		spriteInstance.mySize = spriteSharedData.myTexture->mySize;

		const int numberOfLogos = 50;
		std::vector<VECTOR2F> positions;
		for (int i = 0; i < numberOfLogos; i++)
		{
			float randX = (float)(rand() % 100) * 0.01f;
			float randY = (float)(rand() % 100) * 0.01f;
			positions.push_back({ randX , randY });
		}

		Tga2D::CEngine::GetInstance()->SetAmbientLightValue(0.1f);
		// LIGHT (Up to 8)

		srand((unsigned int)time(0));

#define RANOM_0_TO_1 (rand() % 100 / 100.0f)
#define RANOM_1_OR_0 (float)(rand() % 2)
		std::vector<Tga2D::CLight*> myLights;
		for (int i = 0; i < NUMBER_OF_LIGHTS_ALLOWED; i++)
		{
			Tga2D::CLight* light = new Tga2D::CLight();
			light->myFallOff = 0.2f;
			light->myIntensity = 0.1f + (RANOM_0_TO_1 + RANOM_0_TO_1);
			light->myPosition.Set(RANOM_0_TO_1, RANOM_0_TO_1);
			light->myColor.Set(RANOM_1_OR_0, RANOM_1_OR_0, RANOM_1_OR_0, 1);
			myLights.push_back(light);
		}

		float timer = 0;
		while (true)
		{
			timer += 1.0f / 60.0f;
			if (!Tga2D::CEngine::GetInstance()->BeginFrame())
			{
				break;
			}

			for (Tga2D::CLight* light : myLights)
			{
				light->Render();
			}

			{
				Tga2D::CSpriteBatchScope batch = spriteDrawer.BeginBatch(spriteSharedData);

				// Render all the loggos onto the sprite
				for (int i = 0; i < numberOfLogos; i++)
				{
					spriteInstance.myPosition = { positions[i].x, positions[i].y };
					spriteInstance.myRotation = static_cast<float>(cos(timer * 0.5f * sin(positions[i].x)));
					batch.Draw(spriteInstance);
				}
			}

			Tga2D::CEngine::GetInstance()->EndFrame();
		}

		for (Tga2D::CLight* light : myLights)
		{
			delete light;
		}
		myLights.clear();
	}
	Tga2D::CEngine::GetInstance()->Shutdown();
}
