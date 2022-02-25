#include "stdafx.h"
#include <tga2d/texture/Texture.h>
#include "DirectXTK/Inc/ScreenGrab.h"
#include <tga2d/texture/texture_manager.h>

Tga2D::CTexture::CTexture()
{
	myPath = "undefined";
	myIsFailedTexture = false;
	myVideoMemeorySize = 0;
}

Tga2D::CTexture::~CTexture()
{
	if (myIsFailedTexture)
	{
		return;
	}
	myPath = "undefined";
	myID = 0;
	delete[] myPixelColors;
	myPixelColors = nullptr;
}

Tga2D::CColor Tga2D::CTexture::GetPixelColor(unsigned short aX, unsigned short aY, bool aRefreshData)
{
	CColor retColor;

	if (aX >= myImageSize.x
		||
		aY >= myImageSize.y)
	{
		ERROR_PRINT("%s%i%s%i%s%i%s%i", "GetPixelColor out of bounds! You want to get a pixel from: X", aX, " Y", aY, " but the image is: X", myImageSize.x - 1, " Y", myImageSize.y - 1);
		return retColor;
	}
	if (aRefreshData)
	{
		delete[] myPixelColors;
		myPixelColors = nullptr;
		myHasGrabbedColorData = false;
	}
	if (!myPixelColors && myHasGrabbedColorData)
	{
		return retColor;
	}
	if (!myPixelColors && !myHasGrabbedColorData)
	{
		myPixelColors = GrabPixelColors();
		myHasGrabbedColorData = true;
		if (!myPixelColors)
		{
			ERROR_PRINT("%s%s", "GetPixelColor can only grab data from a .DDS with format: A8R8G8B8! File:", myPath.c_str());
			return retColor;
		}
	}

	unsigned int pixelPlace = aX + (aY * myImageSize.x);

	pixelPlace *= 4;

	retColor.myA = myPixelColors[pixelPlace + 3];
	retColor.myR = myPixelColors[pixelPlace + 2];
	retColor.myG = myPixelColors[pixelPlace + 1];
	retColor.myB = myPixelColors[pixelPlace + 0];


	return retColor;
}

BYTE* Tga2D::CTexture::GrabPixelColors()
{
	ID3D11Resource* resource = NULL;;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	if (myResource)
	{
		myResource->GetResource(&resource);
		myResource->GetDesc(&srvDesc);
	}
	else
	{
		return nullptr;
	}

	ID3D11Texture2D* tex = (ID3D11Texture2D*)resource;
	BYTE* bytes = nullptr;
	if (tex)
	{
		bytes = DirectX::GrabPixelData(CEngine::GetInstance()->GetDirect3D().GetContext(), tex);
	}


	return bytes;

}


Tga2D::CRenderTarget::CRenderTarget() : CTexture()
{
	myTarget = nullptr;
}

Tga2D::CRenderTarget::~CRenderTarget()
{
}
