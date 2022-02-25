#include <string>
#include <tga2d/engine.h>
#include <tga2d/d3d/direct_3d.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/drawers/sprite_drawer.h>
#include <tga2d/texture/texture_manager.h>
#include <tga2d/error/error_manager.h>

#include "../../TutorialCommon/TutorialCommon.h"
#include <thread>
#include <vector>
#include <mutex>
#include <tga2d/text/text.h>
#include <chrono>

#define SIMULATE_HEAVY_RESOURCE_LOADING false
#define SIMULATE_GAMEPLAY_PERFORMANCE_ISSUES false
#define SIMULATE_ENGINE_PERFORMANCE_ISSUES false

using namespace std::chrono_literals;

// Making sure that DX2DEngine lib is linked
//
#ifdef _DEBUG
#pragma comment(lib,"tga2dcore_Debug.lib")
#pragma comment(lib,"External_Debug.lib")
#else
#pragma comment(lib,"tga2dcore_Release.lib")
#pragma comment(lib,"External_Release.lib")
#endif // _DEBUG

using namespace std::placeholders;
using namespace Tga2D;


bool myShouldExit = false;
bool myIsEngineStarted = false;
std::thread *myRenderThread = nullptr;
std::thread *myLoadingThread = nullptr;
std::thread *myGameThreadThread = nullptr;

unsigned int myIDDistribution = 0;

// This is a struct that only lives on the engine/loading side. Gameplay should never ever touch the sprite data
struct SSpriteResourceEngine
{
	unsigned int myID = 0;
	Tga2D::SSpriteSharedData mySharedData = {};
	Tga2D::SSpriteInstanceData myInstanceData = {};
};

//This lives on the gameplay side, the only thing connecting this to a sprite is the ID
struct SSpriteResourceGame
{
	unsigned int myID = 0;
	VECTOR2F myPosition;
};

std::mutex myLoadingEngineTransferMutex; // When resources are loaded, we need to send them to the engine
std::mutex myGameEngineTransferMutex; // When we want to tell the engine what to draw
std::vector<SSpriteResourceEngine> myLoadedResources; // These resources live only in the loading thread and engine thread
std::vector<SSpriteResourceGame> myGameSpritesToBeRendered; // These resources live in engine and in game
int myLoadingThreaFPS = 0; // Just to show fps
int myGameThreadFPS = 0;// Just to show fps

void RunEngineThread();
void RunLoadingThread();
void RunGameThread();
int main( const int /*argc*/, const char * /*argc*/[] )
{
	myRenderThread = new std::thread(RunEngineThread); // Create render thread
	myLoadingThread = new std::thread(RunLoadingThread); // Create loading thread
	myGameThreadThread = new std::thread(RunGameThread); // Create game thread
   
	// the main thread is now unused, this is a waste in a real game, we should use the main as some kind of worker
	while (!myShouldExit)
	{
		// Main thread should just wait
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	// Sync and shut down all threads
	if (myRenderThread)
	{
		myRenderThread->join();
	}
	if (myLoadingThread)
	{
		myLoadingThread->join();
	}
	if (myGameThreadThread)
	{
		myGameThreadThread->join();
	}

    return 0;
}


void RunEngineThread()
{
	{
		TutorialCommon::Init(L"TGA2D: Tutorial 14, threading", true, false);
		myIsEngineStarted = true;
		std::vector<SSpriteResourceEngine> myEngineSprites;

		Tga2D::CText textEngineFPS("Text/BarlowSemiCondensed-Regular.ttf", Tga2D::EFontSize_18);
		textEngineFPS.SetText("");
		textEngineFPS.SetPosition({ 0.1f, 0.1f });
		textEngineFPS.SetColor(Tga2D::CColor(1.0f, 1.0f, 1.0f, 1.0f));

		Tga2D::CText textLoaderFPS("Text/BarlowSemiCondensed-Regular.ttf", Tga2D::EFontSize_18);
		textLoaderFPS.SetText("");
		textLoaderFPS.SetPosition({ 0.1f, 0.12f });
		textLoaderFPS.SetColor(Tga2D::CColor(1.0f, 1.0f, 1.0f, 1.0f));

		Tga2D::CText textGameFPS("Text/BarlowSemiCondensed-Regular.ttf", Tga2D::EFontSize_18);
		textGameFPS.SetText("");
		textGameFPS.SetPosition({ 0.1f, 0.14f });
		textGameFPS.SetColor(Tga2D::CColor(1.0f, 1.0f, 1.0f, 1.0f));


		using clock = std::chrono::high_resolution_clock;
		auto time_start = clock::now();

		Tga2D::CEngine& engine = *Tga2D::CEngine::GetInstance();
		Tga2D::CSpriteDrawer& spriteDrawer(engine.GetDirect3D().GetSpriteDrawer());

		while (true)
		{
			std::chrono::duration<double, std::milli> delta_time = clock::now() - time_start;
			time_start = clock::now();


			// If there are new resources loaded, transfer ownership to the render
			while (myLoadedResources.size() > 0)
			{
				myLoadingEngineTransferMutex.lock();
				myEngineSprites.push_back(myLoadedResources[0]);
				myLoadedResources.erase(myLoadedResources.begin());
				myLoadingEngineTransferMutex.unlock();
			}


			//Render all game sprites that is pushed from the game thread
			std::vector<SSpriteResourceGame> toRenderSprites;
			myGameEngineTransferMutex.lock();
			toRenderSprites = (myGameSpritesToBeRendered);
			myGameEngineTransferMutex.unlock();

			if (!engine.BeginFrame())
			{
				myShouldExit = true;
				break;
			}

			for (SSpriteResourceGame& gameSprite : toRenderSprites)
			{
				if (myEngineSprites.size() > gameSprite.myID)
				{
					myEngineSprites[gameSprite.myID].myInstanceData.myPosition = gameSprite.myPosition;
					spriteDrawer.Draw(myEngineSprites[gameSprite.myID].mySharedData, myEngineSprites[gameSprite.myID].myInstanceData);
				}

			}
			textEngineFPS.SetText("Engine thread FPS: " + std::to_string((int)(1.0f / (delta_time.count() / 1000.0f))));
			textEngineFPS.Render();

			textLoaderFPS.SetText("Loading thread FPS: " + std::to_string(myLoadingThreaFPS));
			textLoaderFPS.Render();

			textGameFPS.SetText("Game thread FPS: " + std::to_string(myGameThreadFPS));
			textGameFPS.Render();

			engine.EndFrame();

			if (SIMULATE_ENGINE_PERFORMANCE_ISSUES)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(70));
			}
			else
			{
				std::this_thread::yield();
			}
		}
	}

	Tga2D::CEngine::GetInstance()->Shutdown();
}

std::vector<std::string> myToLoadResources;
void RunLoadingThread()
{
	while (!myIsEngineStarted) // Wait for engine to start
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	// Here we should get all resources from a file/level
	const unsigned int numberOfResources = 100;
	for (int i=0; i< numberOfResources; i++)
	{
		myToLoadResources.push_back("sprites/tga_logo.dds");
	}

	using clock = std::chrono::high_resolution_clock;
	auto time_start = clock::now();

	while (!myShouldExit)
	{
		std::chrono::duration<double, std::milli> delta_time = clock::now() - time_start;
		time_start = clock::now();

		if (myToLoadResources.size() > 0)
		{
			SSpriteResourceEngine newEngineResource;
			newEngineResource.myID = myIDDistribution;
			newEngineResource.mySharedData.myTexture = Tga2D::CEngine::GetInstance()->GetTextureManager().GetTexture(myToLoadResources[0].c_str());
			newEngineResource.myInstanceData.mySize = newEngineResource.mySharedData.myTexture->mySize;
			newEngineResource.myInstanceData.myPivot = { 0.5f, 0.5f };
			
			myToLoadResources.erase(myToLoadResources.begin());
			myIDDistribution++;

			myLoadingEngineTransferMutex.lock();
			myLoadedResources.push_back(newEngineResource);
			myLoadingEngineTransferMutex.unlock();
		}

		myLoadingThreaFPS = (int)(1.0f / (delta_time.count() / 1000.0f));
		if (SIMULATE_HEAVY_RESOURCE_LOADING)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(40));
		}
		else
		{
			std::this_thread::yield();
		}
	}
}

void RunGameThread()
{
	while (!myIsEngineStarted) // Wait for engine to start
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	using clock = std::chrono::high_resolution_clock;
	auto time_start = clock::now();

	std::vector<SSpriteResourceGame> spritesThatShouldBeSendToEngineForRender;
	spritesThatShouldBeSendToEngineForRender.reserve(100);

	for (int i = 0; i < 100; i++)
	{
		SSpriteResourceGame spriteToRender;
		spriteToRender.myID = i % 100;
		spriteToRender.myPosition.x = static_cast<float>(i % 10) / 10.0f;
		spriteToRender.myPosition.y = static_cast<float>(i / 10) / 10.0f;
		spritesThatShouldBeSendToEngineForRender.push_back(spriteToRender);
	}

	float myTotalTime = 0;
	while (!myShouldExit) // Wait for engine to start
	{

		std::chrono::duration<double, std::milli> delta_time = clock::now() - time_start;
		time_start = clock::now();

		float fDeltaTime = (static_cast<float>(delta_time.count()) / 1000.0f);
		myTotalTime += fDeltaTime;
		for (SSpriteResourceGame& gameResource : spritesThatShouldBeSendToEngineForRender)
		{
			gameResource.myPosition.x += cosf(myTotalTime + gameResource.myPosition.x) * fDeltaTime * 0.1f;
			gameResource.myPosition.y += sinf(myTotalTime + gameResource.myPosition.y) * fDeltaTime* 0.1f;
		}

		myGameEngineTransferMutex.lock();
	
		myGameSpritesToBeRendered = (spritesThatShouldBeSendToEngineForRender);
		myGameEngineTransferMutex.unlock();

		myGameThreadFPS = (int)(1.0f / (delta_time.count() / 1000.0f));

		if (SIMULATE_GAMEPLAY_PERFORMANCE_ISSUES)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		else
		{
			std::this_thread::yield();
		}
	}
}