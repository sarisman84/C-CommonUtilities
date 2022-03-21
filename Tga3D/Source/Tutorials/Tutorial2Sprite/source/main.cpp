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
	TutorialCommon::Init(L"TGA2D: Tutorial 2");
   
	Tga2D::Engine& engine = *Tga2D::Engine::GetInstance();

	// Get a sprite drawer, for drawing sprites
	Tga2D::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());

	// Sprite data is split into two parts. Shared data that is shared across multiple sprites and instance data that is unique per instance.
	// This split is done for performance reasons.

	// Create shared data and assign a texture.
	Tga2D::SpriteSharedData sharedData = {};
	sharedData.myTexture = engine.GetTextureManager().GetTexture(L"sprites/tga_logo.dds");
	
	// Create instance data. 
	Tga2D::Sprite2DInstanceData spriteInstance = {};

	// Setting the pivot so all operations will be in the middle of the image (rotation, position, etc.)
	spriteInstance.myPivot = Tga2D::Vector2f(0.5f, 0.5f);

	float timer = 0;
	while (true)
	{
		timer += 1.0f / 60.0f;
		if (!engine.BeginFrame())
		{
			break;
		}

		{
			// Start a sprite batch. This allows multiple sprites to be drawn efficiently as long as they share the same shared data.
			// The batch will be completed when the batch scope goes out of scope
			Tga2D::SpriteBatchScope batch = spriteDrawer.BeginBatch(sharedData);

			// Set a new position
			spriteInstance.myPosition = Tga2D::Vector2f((cosf(timer) + 1) / 2, (sinf(timer) + 1) / 2);
			// Set the rotation
			spriteInstance.myRotation = cosf(timer);

			// Draw the sprite
			batch.Draw(spriteInstance);

			// using the same instance we reuse the image and set a new position
			spriteInstance.myPosition = Tga2D::Vector2f((sinf(timer) + 1) / 2, (cosf(timer) + 1) / 2);

			// Draw a second time in a new position
			batch.Draw(spriteInstance);

			// Batch goes out of scope and flushes all rendering
		}

		engine.EndFrame();
	}
	engine.Shutdown();
}
