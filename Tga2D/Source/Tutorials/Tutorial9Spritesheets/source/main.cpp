#include <string>

#include <tga2d/drawers/debug_drawer.h>
#include <tga2d/math/vector2.h>
#include <tga2d/engine.h>
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


void Go( void )
{
	{
		TutorialCommon::Init(L"TGA2D: Tutorial 9");

		Tga2D::CEngine& engine = *Tga2D::CEngine::GetInstance();

		// Get a sprite drawer, for drawing sprites
		Tga2D::CSpriteDrawer& spriteDrawer(engine.GetDirect3D().GetSpriteDrawer());

		// Sprite data is split into two parts. Shared data that is shared across multiple sprites and instance data that is unique per instance.
		// This split is done for performance reasons.

		// Create shared data and assign a texture.
		Tga2D::SSpriteSharedData sharedData = {};
		sharedData.myTexture = engine.GetTextureManager().GetTexture("../source/tutorials/Tutorial9Spritesheets/animation.dds");

		Tga2D::SSpriteInstanceData instance = {};
		instance.mySize = sharedData.myTexture->mySize; // We have 4 images wide, then set the image to half the size
		instance.mySizeMultiplier = { 0.5f, 0.5f }; // We have 4 images wide, then set the image to half the size
		instance.myPosition = { 0.5f, 0.5f };
		instance.myPivot = { 0.5f, 0.5f };

		struct UV
		{
			UV(VECTOR2F aStart, VECTOR2F aEnd) { myStart = aStart; myEnd = aEnd; }
			VECTOR2F myStart;
			VECTOR2F myEnd;
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
			if (!Tga2D::CEngine::GetInstance()->BeginFrame())
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

			Tga2D::CEngine::GetInstance()->EndFrame();
		}
	}

	Tga2D::CEngine::GetInstance()->Shutdown();
}
