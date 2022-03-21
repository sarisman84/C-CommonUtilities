#include <string>


#include "../../TutorialCommon/TutorialCommon.h"
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/engine.h>
#include <tga2d/light/LightManager.h>

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

		Tga2D::Engine& engine = *Tga2D::Engine::GetInstance();
		Tga2D::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());

		Tga2D::SpriteSharedData spriteSharedData = {};
		Tga2D::Texture* texture = engine.GetTextureManager().GetTexture(L"sprites/tga_logo.dds");
		spriteSharedData.myTexture = texture;

		// Set normal map
		spriteSharedData.myMaps[Tga2D::NORMAL_MAP] = engine.GetTextureManager().GetTexture(L"../source/tutorials/Tutorial5Light/data/tga_logo_n.dds");

		// Create instance data. 
		Tga2D::Sprite2DInstanceData spriteInstance = {};

		// Setting the pivot so all operations will be in the middle of the image (rotation, position, etc.)
		spriteInstance.myPivot = Tga2D::Vector2f(0.5f, 0.5f);
		spriteInstance.mySize = texture->mySize;

		const int numberOfLogos = 50;
		std::vector<Tga2D::Vector2f> positions;
		for (int i = 0; i < numberOfLogos; i++)
		{
			float randX = (float)(rand() % 100) * 0.01f;
			float randY = (float)(rand() % 100) * 0.01f;
			positions.push_back({ randX , randY });
		}

		Tga2D::Engine::GetInstance()->SetAmbientLightValue(0.1f);
		// LIGHT (Up to 8)

		srand((unsigned int)time(0));

#define RANOM_0_TO_1 (rand() % 100 / 100.0f)
#define RANOM_1_OR_0 (float)(rand() % 2)
		std::vector<Tga2D::PointLight> myLights;
		for (int i = 0; i < NUMBER_OF_LIGHTS_ALLOWED; i++)
		{
			Tga2D::PointLight light(Tga2D::Transform(Tga2D::Vector3f(RANOM_0_TO_1, RANOM_0_TO_1, 0.0)), Tga2D::Color(RANOM_1_OR_0, RANOM_1_OR_0, RANOM_1_OR_0, 1), 0.1f + (RANOM_0_TO_1 + RANOM_0_TO_1), 0.2f);
			myLights.push_back(light);
		}

		float timer = 0;
		while (true)
		{
			timer += 1.0f / 60.0f;
			if (!Tga2D::Engine::GetInstance()->BeginFrame())
			{
				break;
			}

			Tga2D::LightManager& lightManager = Tga2D::Engine::GetInstance()->GetLightManager();
			lightManager.ClearPointLights();
			for (Tga2D::PointLight& light : myLights)
			{
				lightManager.AddPointLight(light);
			}

			{
				Tga2D::SpriteBatchScope batch = spriteDrawer.BeginBatch(spriteSharedData);

				// Render all the loggos onto the sprite
				for (int i = 0; i < numberOfLogos; i++)
				{
					spriteInstance.myPosition = { positions[i].x, positions[i].y };
					spriteInstance.myRotation = static_cast<float>(cos(timer * 0.5f * sin(positions[i].x)));
					batch.Draw(spriteInstance);
				}
			}

			Tga2D::Engine::GetInstance()->EndFrame();
		}
	}
	Tga2D::Engine::GetInstance()->Shutdown();
}
