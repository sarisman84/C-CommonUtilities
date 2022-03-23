#include "stdafx.h"
#include <tga2d/videoplayer/video.h>
#include <tga2d/videoplayer/videoplayer.h>

#ifdef USE_VIDEO

#include <tga2d/sprite/sprite.h>
#include <tga2d/d3d/direct_3d.h>
#include <tga2d/texture/texture_manager.h>

using namespace Tga2D;
Tga2D::CVideo::CVideo()
	:myPlayer(nullptr)
{
	myBuffer = nullptr;
	myUpdateTime = 0.0f;
	myStatus = EVideoStatus::Idle;
	myWantsToPlay = false;
	myTexture = nullptr;
	myIsLooping = false;
}

Tga2D::CVideo::~CVideo()
{
	if (myTexture)
	{
		delete myTexture;
		myTexture = nullptr;
	}

	if (myBuffer)
	{
		delete[] myBuffer;
	}

	delete myPlayer;
	myPlayer = nullptr;
	
}

void Tga2D::CVideo::Play(bool aLoop)
{
	myWantsToPlay = true;
	myIsLooping = aLoop;
}

void Tga2D::CVideo::Pause()
{
	myWantsToPlay = false;
}

void Tga2D::CVideo::Stop()
{
	myStatus = EVideoStatus::Idle;
	myWantsToPlay = false;
	myPlayer->Stop();
}

void Tga2D::CVideo::Restart()
{
	myWantsToPlay = true;
	myPlayer->RestartStream();
}
#include <functional>
bool CVideo::Init(const char* aPath, bool aPlayAudio)
{
	if (myPlayer)
	{
		return false;
	}

	myPlayer = new CVideoPlayer();
	if (myPlayer)
	{
		EVideoError error = myPlayer->Init(aPath, aPlayAudio);
		if (error == EVideoError_WrongFormat)
		{
			ERROR_PRINT("%s %s %s", "Could not load video: ", aPath, ". Wrong format?");
			return false;
		}
		else if (error == EVideoError_FileNotFound)
		{
			ERROR_PRINT("%s %s %s", "Could not load video: ", aPath, ". File not found");
			return false;
		}
	}

	myTexture = new CTexture();

	if (!myPlayer->DoFirstFrame())
	{
		ERROR_PRINT("%s %s %s", "Video error: ", aPath, ". First frame not found?");
		return false;
	}
	
	mySize.x = myPlayer->GetAvVideoFrame()->width;
	mySize.y = myPlayer->GetAvVideoFrame()->height;

	myPowerSizeX = (int)powf(2, ceilf(logf((float)mySize.x) / logf(2)));
	myPowerSizeY = (int)powf(2, ceilf(logf((float)mySize.y) / logf(2)));

	myBuffer = new int[(myPowerSizeX* myPowerSizeY)];

	myStatus = EVideoStatus::Playing;


	if (!myShaderResource)
	{
		D3D11_TEXTURE2D_DESC texture_desc;
		texture_desc.Width = myPowerSizeX;
		texture_desc.Height = myPowerSizeY;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;

		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DYNAMIC;
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		texture_desc.MiscFlags = 0;

		CEngine::GetInstance()->GetDirect3D().GetDevice()->CreateTexture2D(&texture_desc, nullptr, myD3DTexture.ReleaseAndGetAddressOf());
		CEngine::GetInstance()->GetDirect3D().GetDevice()->CreateShaderResourceView(myD3DTexture.Get(), NULL, myShaderResource.ReleaseAndGetAddressOf());

		myTexture->myResource = myShaderResource;
		myTexture->mySize = CTextureManager::GetTextureSize(myShaderResource.Get());
		myTexture->myImageSize = CTextureManager::GetTextureSize(myShaderResource.Get(), false);
	}

	// Force one frame so we dont get a blank video
	bool wantsToPlay = myWantsToPlay;
	myWantsToPlay = true;
	myUpdateTime = 0.0001f;
	Update(0);
	myUpdateTime = 0.0f;
	myWantsToPlay = wantsToPlay;

	return true;
}

void CVideo::Update(float aDelta)
{
	if (!myWantsToPlay || !myPlayer)
	{
		return;
	}
	myUpdateTime += aDelta;

	double fps = myPlayer->GetFps();

	myPlayer->Updateound();
	while (myUpdateTime >= 1.0f / fps)
	{
		if (myShaderResource && myD3DTexture)
		{
			int status = myPlayer->GrabNextFrame();
			if (status < 0)
			{
				myStatus = EVideoStatus::ReachedEnd;
				if (myIsLooping)
				{
					Restart();
				}
			}

			CDirectEngine& engine = CEngine::GetInstance()->GetDirect3D();
			D3D11_MAPPED_SUBRESOURCE  mappedResource;
			HRESULT result = engine.GetContext()->Map(myD3DTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return;
			}

			unsigned int* source = (unsigned int*)(mappedResource.pData);
			myPlayer->Update(source, myPowerSizeX, myPowerSizeY);
			engine.GetContext()->Unmap(myD3DTexture.Get(), 0);
		}
		myUpdateTime -= 1.0f / static_cast<float>(fps);
	}
}

#endif