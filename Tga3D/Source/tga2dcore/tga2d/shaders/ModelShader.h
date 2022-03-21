#pragma once

#include "shader.h"
#include "ShaderCommon.h"
#include <tga2d/animation/Animation.h>
#include <tga2d/math/CommonMath.h>
#include <tga2d/math/matrix4x4.h>
#include <tga2d/model/model.h>

namespace Tga2D
{
	class RenderObjectSprite;
	class ModelShader : public Shader
	{
	public:
		ModelShader(Engine* aEngine);
		~ModelShader();

		bool Init() override;
		bool Init(const wchar_t* aVertexShaderFile, const wchar_t* aPixelShaderFile);
		void Render(const RenderObjectSharedData& sharedData, const TextureResource* const* someTextures, const Model::MeshData& aModelData, const Matrix4x4f& aObToWorld, const Matrix4x4f* someBones = nullptr);
		bool CreateInputLayout(const std::string& aVS) override;
	private:
		struct ID3D11Buffer* myBoneBuffer;
		struct ID3D11Buffer* myObjectBuffer;
	};
} // namespace Tga2D