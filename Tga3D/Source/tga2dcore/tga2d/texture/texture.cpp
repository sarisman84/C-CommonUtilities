#include "stdafx.h"
#include <tga2d/texture/Texture.h>
#include <ScreenGrab/ScreenGrab11.h>
#include <tga2d/texture/TextureManager.h>

Tga2D::Texture::Texture()
{
	myPath = L"undefined";
	myIsFailedTexture = false;
}

Tga2D::Texture::~Texture()
{
	if (myIsFailedTexture)
	{
		return;
	}
	myPath = L"undefined";
	myID = 0;
}