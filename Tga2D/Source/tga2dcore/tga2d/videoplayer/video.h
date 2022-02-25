/*
This class will store a texture bound to DX11
*/
#pragma once
#include <tga2d/engine_defines.h>

#ifdef USE_VIDEO
namespace Tga2D
{
	enum class EVideoStatus
	{
		Idle,
		Playing,
		ReachedEnd
	};

	class CVideo
	{
	public:
		CVideo();
		~CVideo();
		bool Init(const char* aPath, bool aPlayAudio = false);
		void Play(bool aLoop = false);
		void Pause();
		void Stop();

		void Update(float aDelta);

		/* Will return false if there was something wrong with the load of the video */
		void Restart();
		
		class CTexture* GetTexture() const { return myTexture; }

		VECTOR2I GetVideoSize() const { return mySize; }
		EVideoStatus GetStatus() const { return myStatus; }
	private:
		class CTexture* myTexture;
		class CVideoPlayer* myPlayer;

		ComPtr<ID3D11ShaderResourceView> myShaderResource;
		ComPtr<ID3D11Texture2D> myD3DTexture;
		
		int *myBuffer;

		VECTOR2I mySize;

		float myUpdateTime;
		EVideoStatus myStatus;
		bool myWantsToPlay;
		bool myIsLooping;

		int myPowerSizeX;
		int myPowerSizeY;
	};
}
#endif