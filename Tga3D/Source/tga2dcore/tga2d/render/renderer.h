#pragma once
#include "RenderCommon.h"

struct ID3D11ShaderResourceView;
namespace Tga2D
{
	class RenderObject;
	class Light;
	class GraphicsEngine;
	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		void AddToRender(RenderObject* aObject);
		void Update();
	private:
		struct OptimTip
		{
			OptimTip()
			{
				myCount = 0;
				myHasWarned = false;
			}
			unsigned int myCount;
			bool myHasWarned;
			std::string myPath;
		};
		
		typedef std::map<ID3D11ShaderResourceView*, OptimTip> OptimisationMap;
		OptimisationMap myRenderedSpritesOfSameType;
		GraphicsEngine* myDXEngine;		
	};
}
