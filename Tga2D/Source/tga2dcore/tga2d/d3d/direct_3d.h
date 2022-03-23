#pragma once
#include <array>
#include <tga2d/render/render_common.h>
#include <tga2d/math/matrix44.h>
#include <memory>
#include <thread>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D10Blob;
struct ID3D11SamplerState;
struct ID3D11Texture2D; 
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;
struct ID3D11DeviceChild;
struct ID3D11BlendState;
struct ID3D11Debug;
struct IDXGIAdapter;
struct D3D11_VIEWPORT;
namespace Tga2D
{ 
	class CEngine;
	class CRenderTarget;
	class CRenderObject;
	class CRenderObjectSprite;
	class CRenderObjectLine;
	class CSpriteDrawer;
	class CLineDrawer;
	class CCustomShapeDrawer;
	class CRenderObjectCustom;
	class CLineMultiPrimitive;
	class CDirectEngine
	{
	public:
		CDirectEngine(void);
		~CDirectEngine(void);
		bool Init(const CEngine& aEngine, VECTOR2UI aWindowSize, SEngineCreateParameters& someParameters);
		bool IsInitiated();
		void PostRenderFrame();
		void RenderFrame();
		void Set2DRenderState();
		void Set3DRenderState();
		void SetClearColor(CColor aColor);
		ID3D11DeviceContext* GetContext() const {return myDeviceContext.Get();}
		ID3D11Device* GetDevice() const {return myDevice.Get();}
	
		CSpriteDrawer& GetSpriteDrawer() { return *mySpriteDrawer; };

		void Draw(CRenderObject* aObject);
		void DoDraw(CRenderObjectSprite* aObject);
		void DoDraw(CRenderObjectLine* aObject);
		void DoDraw(CRenderObjectCustom* aCustomObject);
		void DoDraw(CLineMultiPrimitive* aObject);
		bool CompileShader(const char* aShader, const char* aMainFunction, const char* aTarget, ID3D10Blob*& aCodeBlob);
		VECTOR2UI myWindowSize;
		VECTOR2UI myTargetSize;

		void SetResolution(VECTOR2UI aResolution);
		void SetFullScreen(bool aFullScreen);
		void SetViewPort(float aTopLeftX, float aTopLeftY, float aWidth, float aHeight, float aMinDepth = 0.0f, float aMaxDepth = 1.0f);
		
		void SetBlendState(EBlendState aState);
		void SetSamplerState(ESamplerFilter aFilter, ESamplerAddressMode aAddressMode);

		Matrix44 myCorrectedWorldWatrix;
		Matrix44 myWorldWatrix;
		Matrix44 myViewWatrix;
		Matrix44 myProjWatrix;

        int GetObjectRenderCount();
		void IncreaseObjectRenderCount() { myRenderedCount++; };

		void SetRenderTarget(CRenderTarget* aRenderTargetTexture);

        void Clear( const CColor &aColor );
		bool IsOnSameThreadAsEngine(std::thread::id aOtherThread) const { return aOtherThread == myThreadID; }
	private:
		bool CreateBlendStates();
		bool CreateSamplers();

		bool File_exist(const char *fileName);
		bool CollectAdapters(VECTOR2UI aWindowSize, VECTOR2I& aNumDenumerator, IDXGIAdapter*& outAdapter);
		void SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_z_ std::string aName);

		ComPtr<IDXGISwapChain> mySwapchain;
		ComPtr<ID3D11Device> myDevice;
		ComPtr<ID3D11DeviceContext> myDeviceContext;
		CRenderTarget* myRendertarget;
		ComPtr<ID3D11RenderTargetView> myBackbuffer;
		std::thread::id myThreadID;

		std::array<std::array<ComPtr<ID3D11SamplerState>, (int)ESamplerAddressMode::Count>, (int)ESamplerFilter::Count> mySamplerStates;
		ComPtr<ID3D11Texture2D> myDepthStencilBuffer;
		ComPtr<ID3D11DepthStencilState> myDepthStencilState;
		ComPtr<ID3D11DepthStencilView> myDepthStencilView;
		ComPtr<ID3D11RasterizerState> myRasterState;
		std::array<ComPtr<ID3D11BlendState>, (int)EBlendState::Count> myBlendStates;
		ComPtr<ID3D11Debug> myD3dDebug;
			
		int myRenderedCount;

		std::unique_ptr<CSpriteDrawer> mySpriteDrawer;
		std::unique_ptr<CCustomShapeDrawer> myCustomShapeDrawer;
		std::unique_ptr<CLineDrawer> myLineDrawer;
		CColor myClearColor;
		int myVideoCardMemory;
		bool myEnableVSync;
		bool myIsInitiated;
		std::unique_ptr<D3D11_VIEWPORT> mySavedViewport;
		bool myHasSavedViewPort;
	};
}