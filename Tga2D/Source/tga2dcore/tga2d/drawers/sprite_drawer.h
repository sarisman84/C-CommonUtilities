#pragma once

#include <memory>
#include <tga2d/render/render_common.h>
#include <tga2d/render/render_object.h>
#include <tga2d/shaders/shader_common.h>

struct ID3D11Buffer;
using Microsoft::WRL::ComPtr;

namespace Tga2D
{
	class CTexture;
	class CSpriteDrawer;
	class CSpriteShader;
	struct SSpriteInstanceData;
	struct SSpriteSharedData;

	class CSpriteBatchScope
	{
		friend class CSpriteDrawer;
	public:
		~CSpriteBatchScope();
		void Draw(const SSpriteInstanceData& aInstance);
		void Draw(const SSpriteInstanceData* aInstances, size_t aInstanceCount);
	private:
		CSpriteBatchScope(CSpriteDrawer& aSpriteDrawer)
			: mySpriteDrawer(&aSpriteDrawer) {}
		CSpriteBatchScope(CSpriteBatchScope&& scope) noexcept
			: mySpriteDrawer(scope.mySpriteDrawer)
			, myInstanceData(scope.myInstanceData)
			, myInstanceCount(scope.myInstanceCount) 
		{
			scope.mySpriteDrawer = nullptr;
			scope.myInstanceData = nullptr;
			scope.myInstanceCount = 0;
		} 

		CSpriteBatchScope(const CSpriteBatchScope&) = delete;
		void operator=(const CSpriteBatchScope&) = delete;

		void UnMapAndRender();
		void Map();

		CSpriteDrawer* mySpriteDrawer;
		SSpriteShaderInstanceData* myInstanceData = nullptr;
		size_t myInstanceCount = 0;
	};

	class CSpriteDrawer
	{
		friend class CSpriteBatchScope;
	public:
		CSpriteDrawer(CDirectEngine* aEngine);
		~CSpriteDrawer();
		void Init();

		inline void Draw(const SSpriteSharedData& aSharedData, const SSpriteInstanceData& aInstance);
		inline void Draw(const SSpriteSharedData& aSharedData, const SSpriteInstanceData* aInstances, size_t aInstanceCount);

		CSpriteBatchScope BeginBatch(const SSpriteSharedData& aSharedData);

	private:
		void EndBatch();

		bool InitShaders();
		bool CreateBuffer();

		ComPtr<ID3D11Buffer> myVertexBuffer = nullptr;
		ComPtr<ID3D11Buffer> myInstanceBuffer = nullptr;
		SVertexInstanced myVertices[6] = {};
		CDirectEngine* myEngine = nullptr;

		std::unique_ptr<CSpriteShader> myDefaultShader;
		bool myIsLoaded = false;
		bool myIsInBatch = false;
	};

	void CSpriteDrawer::Draw(const SSpriteSharedData& aSharedData, const SSpriteInstanceData& aInstance)
	{
		CSpriteBatchScope scope = BeginBatch(aSharedData);
		scope.Draw(aInstance);
	}

	void CSpriteDrawer::Draw(const SSpriteSharedData& aSharedData, const SSpriteInstanceData* aInstances, size_t aInstanceCount)
	{
		CSpriteBatchScope scope = BeginBatch(aSharedData);
		scope.Draw(aInstances, aInstanceCount);
	}
}
