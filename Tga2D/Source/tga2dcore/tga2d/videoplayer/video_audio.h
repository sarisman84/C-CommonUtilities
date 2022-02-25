/*
This class will store a texture bound to DX11
*/
#pragma once
#include <tga2d/engine_defines.h>
#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX 
#include <windows.h>
#include <mmeapi.h>

#ifdef USE_VIDEO
namespace Tga2D
{
	class CWaveBuffer 
	{
	public:
		~CWaveBuffer();
		BOOL Init(HWAVEOUT hWave, int Size);
		BOOL Write(PBYTE pData, int nBytes, int& BytesWritten);
		void Flush();
	private:
		WAVEHDR      m_Hdr;
		HWAVEOUT     m_hWave;
		int          m_nBytes;
	};

	class CWaveOut {
	public:
		CWaveOut(LPCWAVEFORMATEX Format, int nBuffers, int BufferSize);
		~CWaveOut();
		void Write(PBYTE Data, int nBytes);
		void Flush();
		void Wait();
		void Reset();
	private:
		const HANDLE   m_hSem;
		const int      m_nBuffers;
		int            m_CurrentBuffer;
		BOOL           m_NoBuffer;
		CWaveBuffer   *m_Hdrs;
		HWAVEOUT       m_hWave;
	};

	

}
#endif