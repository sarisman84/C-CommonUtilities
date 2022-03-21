#include <string>

#include <tga2d/drawers/DebugDrawer.h>
#include <tga2d/math/vector2.h>
#include <tga2d/engine.h>
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


void Go( void )
{
	{
		TutorialCommon::Init(L"TGA2D: Tutorial 9");

		Tga2D::Engine& engine = *Tga2D::Engine::GetInstance();

		// Get a sprite drawer, for drawing sprites
		Tga2D::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());

		// Sprite data is split into two parts. Shared data that is shared across multiple sprites and instance data that is unique per instance.
		// This split is done for performance reasons.

		// Create shared data and assign a texture.
		Tga2D::SpriteSharedData sharedData = {};
		Tga2D::Texture* texture = engine.GetTextureManager().GetTexture(L"../source/tutorials/Tutorial9Spritesheets/animation.dds");
		sharedData.myTexture = texture;

		Tga2D::Sprite2DInstanceData instance = {};
		instance.mySize = texture->mySize; // We have 4 images wide, then set the image to half the size
		instance.mySizeMultiplier = { 0.5f, 0.5f }; // We have 4 images wide, then set the image to half the size
		instance.myPosition = { 0.5f, 0.5f };
		instance.myPivot = { 0.5f, 0.5f };

		struct UV
		{
			UV(Tga2D::Vector2f aStart, Tga2D::Vector2f aEnd) { myStart = aStart; myEnd = aEnd; }
			Tga2D::Vector2f myStart;
			Tga2D::Vector2f myEnd;
		};

		const float addingUVX = 1.0f / 8.0f; // 8 sprites per row
		const float addingUVY = 1.0f / 8.0f; // 8 sprites per col
		std::vector<UV> myUvs;
		for (int j = 0; j < 8; j++)
		{
			for (int i = 0; i < 8; i++)
			{
				myUvs.push_back(UV({ addingUVX * i, addingUVY * j }, { (addingUVX * i) + addingUVX, (addingUVY * j) + addingUVY }));
			}
		}

		const float deltaTime = 1.0f / 60.0f;
		float timer = 0.0f;
		unsigned short aIndex = 0;
		while (true)
		{
			timer += deltaTime;
			if (!Tga2D::Engine::GetInstance()->BeginFrame())
			{
				break;
			}

			// Cycle the sheet
			if (timer >= 0.05f)
			{
				aIndex++;
				if (aIndex > 28)
				{
					aIndex = 0;
				}
				timer = 0.0f;
			}

			instance.myTextureRect = { myUvs[aIndex].myStart.x, myUvs[aIndex].myStart.y, myUvs[aIndex].myEnd.x, myUvs[aIndex].myEnd.y };
			spriteDrawer.Draw(sharedData, instance);

			Tga2D::Engine::GetInstance()->EndFrame();
		}
	}

	Tga2D::Engine::GetInstance()->Shutdown();
}
