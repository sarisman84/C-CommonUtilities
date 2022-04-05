#pragma once
#include <tga2d/render/RenderObject.h>
#include <tga2d/EngineDefines.h>
namespace Tga2D
{
	class Texture;
	class CustomShape : public RenderObjectCustom
	{
	public:
		CustomShape();
		~CustomShape();

		// Do this first, must be %3 (3, 6, 9, 12 etc..)
		int AddPoint(Vector2f aPoint, Color aColor, Vector2f aUV = Vector2f(0, 0));
		void Reset();
		void RemovePoint(int aIndex);

		// Do this second
		void BuildShape();
		void Render();
		void SetTexture(const wchar_t* aPath);

		const Texture* GetTexture() const { return myTexture; }
	private:
		Texture* myTexture;
		bool myIsDirty;
		int myIndex;
		bool myHasTexture;
		
		
	};
}