#include "stdafx.h"
#include <tga2d/light/LightManager.h>
#include <tga2d/engine.h>
#include <tga2d/render/Renderer.h>

using namespace Tga2D;

LightManager::LightManager()
: myAmbientLight(Color { 1, 1, 1}, 1.0)
, myDirectionalLight({}, Color { 0, 0, 0}, 0.0)
{}

LightManager::~LightManager()
{
}

void LightManager::AddPointLight(const PointLight& aPointLight)
{
	if (myPointLights.size() >= NUMBER_OF_LIGHTS_ALLOWED)
	{
		ERROR_PRINT("%s%i%s", "We only allow ", NUMBER_OF_LIGHTS_ALLOWED, " lights at the same time and you are trying to push more");
		return;
	}
	myPointLights.push_back(aPointLight);
}
