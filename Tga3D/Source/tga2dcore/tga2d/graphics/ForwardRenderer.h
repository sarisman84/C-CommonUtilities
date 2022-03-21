#pragma once
#include <d3d11.h>
#include <vector>
#include <vector>

#include "AmbientLight.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include <Tga2D/Math/Matrix4x4.h>

namespace Tga2D
{

class PointLight;
class AnimatedModelInstance;
class ModelInstance;
class ForwardRenderer
{
	struct ObjectBufferData
	{
		Matrix4x4f myToWorld;	
	} myObjectBufferData;

	ID3D11Buffer* myFrameBuffer;
	ID3D11Buffer* myObjectBuffer;
	ID3D11Buffer* myBoneBuffer; 

	ID3D11VertexShader* myAnimatedModelVertexShader;
	ID3D11VertexShader* myModelVertexShader;
	ID3D11InputLayout* myModelInputLayout;
	ID3D11PixelShader* myModelPixelShader;
	
public:
	
	ForwardRenderer();
	~ForwardRenderer();
	bool Init();

	/**
	 * Tell the renderer which camera to use, the directional light and the ambient light.
	 * This configures the Frame Buffer.
	 * @param aCamera The Camera to use when rendering.
	 * @param aDirectionalLight The scene directional light.
	 * @param aAmbientLight The scene ambient light.
	 * @returns True if everything went well.
	 */
	bool SetEnvironmentMap(
		const AmbientLight& aAmbientLight
	);

	/**
	 * Render the specified model instance with the specified lights
	 * @param aModelInstance The ModelInstance to render.
	 * @param somePointLights A set of Point Lights which affect this model. May be zero-length.
	 */
	void Render(const ModelInstance& aModelInstance);
	void Render(const AnimatedModelInstance& aModelInstance);
};

} // namespace Tga2D