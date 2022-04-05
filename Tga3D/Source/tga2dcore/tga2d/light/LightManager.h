#pragma once
#include <vector>
#include <tga2d/graphics/AmbientLight.h>
#include <tga2d/graphics/DirectionalLight.h>
#include <tga2d/graphics/PointLight.h>

namespace Tga2D
{
	class LightManager
	{
	public:
		LightManager();
		~LightManager();

		size_t GetPointLightCount() const { return myPointLights.size(); }

		void ClearPointLights() { myPointLights.clear(); }
		void AddPointLight(const PointLight& aPointLight);
		const PointLight* GetPointLights() const { return myPointLights.data(); };

		void SetDirectionalLight(DirectionalLight light) { myDirectionalLight = light; }
		const DirectionalLight& GetDirectionalLight() const { return myDirectionalLight; }

		void SetAmbientLight(AmbientLight light) { myAmbientLight = light; }
		const AmbientLight& GetAmbientLight() const { return myAmbientLight; }

	private:
		std::vector<PointLight> myPointLights;
		DirectionalLight myDirectionalLight;
		AmbientLight myAmbientLight;
	};
}

