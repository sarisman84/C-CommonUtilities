#pragma once

#include <memory>
#include <tga2d/render/RenderCommon.h>
#include <tga2d/render/RenderObject.h>
#include <tga2d/shaders/ShaderCommon.h>
#include <wrl\client.h>

struct ID3D11Buffer;
using Microsoft::WRL::ComPtr;

namespace Tga2D
{
	class AnimatedModelInstance;
	class ModelInstance;
	class ModelShader;

	class ModelDrawer
	{
	public:
		ModelDrawer();
		~ModelDrawer();
		bool Init();

		void Draw(const AnimatedModelInstance& modelInstance);
		void Draw(const ModelInstance& modelInstance);

	private:

		std::unique_ptr<ModelShader> myDefaultShader;
		std::unique_ptr<ModelShader> myDefaultAnimatedModelShader;

		bool myIsLoaded = false;
		bool myIsInBatch = false;
	};
}
