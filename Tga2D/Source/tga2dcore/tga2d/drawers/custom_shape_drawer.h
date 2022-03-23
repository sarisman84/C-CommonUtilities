#pragma once
#include <tga2d/render/render_common.h>
#include <tga2d/shaders/shader.h>

namespace Tga2D
{
    struct SCustomShapeConstantBufferData
    {
		Vector4f myPosition;
        Vector4f myRotationAndSize;
    };

    class CDirectEngine;
    class CCustomShape;
    class CRenderObjectCustom;
    class CCustomShapeDrawer : public CShader
    {
    public:
        CCustomShapeDrawer(CEngine* anEngine);
        ~CCustomShapeDrawer();
        bool Init();
        void Draw(CRenderObjectCustom* aObject);
    private:
        CCustomShapeDrawer &operator =( const CCustomShapeDrawer &anOther ) = delete;

        bool CreateInputLayout(ID3D10Blob* aVS) override;
        bool InitShaders();
        void CreateBuffer();

        int SetShaderParameters(CRenderObjectCustom* aObject);
        int UpdateVertexes(CRenderObjectCustom* aObject);
        ComPtr<ID3D11Buffer> myVertexBuffer;
        CDirectEngine& myEngine;
        ComPtr<ID3D11Buffer> myObjectBuffer;
        int myMaxPoints = 0;
    };
}