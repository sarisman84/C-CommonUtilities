#pragma once
#include <tga2d/render/RenderCommon.h>
#include <tga2d/shaders/shader.h>

namespace Tga2D
{
    struct CustomShapeConstantBufferData
    {
		Vector4f myPosition;
        Vector4f myRotationAndSize;
    };

    class GraphicsEngine;
    class CustomShape;
    class RenderObjectCustom;
    class CustomShapeDrawer : public Shader
    {
    public:
        CustomShapeDrawer(Engine* anEngine);
        ~CustomShapeDrawer();
        bool Init();
        void Draw(RenderObjectCustom* aObject);
    private:
        CustomShapeDrawer &operator =( const CustomShapeDrawer &anOther ) = delete;

        bool CreateInputLayout(const std::string& aVS) override;
        bool InitShaders();
        void CreateBuffer();

        int SetShaderParameters(RenderObjectCustom* aObject);
        int UpdateVertexes(RenderObjectCustom* aObject);
        ComPtr<ID3D11Buffer> myVertexBuffer;
        ComPtr<ID3D11Buffer> myObjectBuffer;
        int myMaxPoints = 0;
    };
}