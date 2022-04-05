#pragma once
#include <vector>
#include <tga2d/math/Color.h>
#include "RenderCommon.h"
namespace Tga2D
{
	class GraphicsEngine;
	class RenderObject : public RenderObjectSharedData
	{
		friend class GraphicsEngine;
	public:
		RenderObject();
		virtual ~RenderObject(){}

		void SetBlendState(BlendState aBlendState) { myBlendState = aBlendState; }
		void SetSamplerState(SamplerFilter aFilter, SamplerAddressMode aAddressMode) { mySamplerFilter = aFilter; mySamplerAddressMode = aAddressMode; }
		bool myIsScreenSpace;
		BlendState myBlendState;
		SamplerFilter mySamplerFilter;
		SamplerAddressMode mySamplerAddressMode;

	protected:
		virtual void Draw(GraphicsEngine* aRenderer) = 0;
	};

	class RenderObjectLine : public RenderObject
	{
	public:
		RenderObjectLine();
		virtual ~RenderObjectLine(){}
		void Draw(GraphicsEngine* aRenderer) override;

		Vector4f myColor;
		Vector2f myFromPosition;
		Vector2f myToPosition;
	};

	struct SCustomPoint
	{
		SCustomPoint()
		{
			myPosition.Set(0, 0);
			myColor.Set(1, 1, 1, 1);
			myUV.Set(0.5f, 0.5f);
		}
		Vector2f myPosition;
		Color myColor;
		Vector2f myUV;
		int myIndex;
	};

	class RenderObjectCustom : public RenderObject
	{
	public:
		RenderObjectCustom();
		~RenderObjectCustom(){}
		void Draw(GraphicsEngine* aRenderer) override;
		std::vector<SCustomPoint> myPoints;

		void SetSize(const Vector2f& aSize)
		{
			mySize = aSize;
		}
		const Vector2f& GetSize()
		{
			return mySize;
		}

		void SetPosition(const Vector2f& aPosition)
		{
			myPosition = aPosition;
		}
		const Vector2f& GetPosition()
		{
			return myPosition;
		}

		void SetRotation(const float aRotation)
		{
			myRotation = aRotation;
		}
		const float GetRotation()
		{
			return myRotation;
		}

		BlendState myBlendState;

	private:
		Vector2f mySize;
		Vector2f myPosition;
		float myRotation = 0;
	};
}