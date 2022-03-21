#pragma once
#include <array>
#include <memory>
#include <thread>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

struct ID3D11Buffer;

namespace Tga2D
{ 
	class Engine;
	class RenderObject;
	class RenderObjectLine;
	class SpriteDrawer;
	class LineDrawer;
	class CustomShapeDrawer;
	class ModelDrawer;
	class RenderObjectCustom;
	class LineMultiPrimitive;
	class Camera;
	class FullscreenEffect;
	class ForwardRenderer;
	class ParticleRenderer;

	class GraphicsEngine
	{
	public:
		GraphicsEngine(void);
		~GraphicsEngine(void);
		bool Init();
		bool IsInitiated();

		SpriteDrawer& GetSpriteDrawer() { return *mySpriteDrawer; };
		ModelDrawer& GetModelDrawer() { return *myModelDrawer; };
		ForwardRenderer& GetForwardRenderer() { return *myForwardRenderer; };

		FullscreenEffect& GetFullscreenEffectCopy() { return *myFullscreenCopy; };
		FullscreenEffect& GetFullscreenEffectTonemap() { return *myFullscreenTonemap; };
		FullscreenEffect& GetFullscreenEffectVerticalGaussianBlur() { return *myFullscreenVerticalGaussianBlur; };
		FullscreenEffect& GetFullscreenEffectHorizontalGaussianBlur() { return *myFullscreenHorizontalGaussianBlur; };

		void SetCamera(const Camera& camera);
		const Camera& GetCamera() const;

		void UpdateAndBindCommonBuffer();
		void UpdateAndBindLightBuffer();

		void Draw(RenderObject* aObject);
		void DoDraw(RenderObjectLine* aObject);
		void DoDraw(RenderObjectCustom* aCustomObject);
		void DoDraw(LineMultiPrimitive* aObject);

		void SetFullScreen(bool aFullScreen);
	private:

		std::unique_ptr<SpriteDrawer> mySpriteDrawer;
		std::unique_ptr<ModelDrawer> myModelDrawer;
		std::unique_ptr<CustomShapeDrawer> myCustomShapeDrawer;
		std::unique_ptr<LineDrawer> myLineDrawer;
		std::unique_ptr<ForwardRenderer> myForwardRenderer;

		std::unique_ptr<FullscreenEffect> myFullscreenCopy;
		std::unique_ptr<FullscreenEffect> myFullscreenTonemap;
		std::unique_ptr<FullscreenEffect> myFullscreenVerticalGaussianBlur;
		std::unique_ptr<FullscreenEffect> myFullscreenHorizontalGaussianBlur;

		ComPtr<ID3D11Buffer> myLightBuffer;
		ComPtr<ID3D11Buffer> myCommonBuffer;
		std::unique_ptr<Camera> myCamera;

		bool myIsInitiated;
	};
}