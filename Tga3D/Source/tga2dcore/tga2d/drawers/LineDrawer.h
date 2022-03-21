#pragma once
#include <tga2d/render/RenderCommon.h>
#include <tga2d/shaders/Shader.h>

namespace Tga2D
{
    class RenderObjectLine;
    class GraphicsEngine;
    class LineDrawer : public Shader
    {
    public:
        LineDrawer(Engine* anEngine);
        ~LineDrawer();
        bool Init() override;
        void Draw(RenderObjectLine* aObject);
		void Draw(LineMultiPrimitive* aObject);
    private:
        LineDrawer &operator =( const LineDrawer &anOther ) = delete;
        bool CreateInputLayout(const std::string& aVS) override;
        bool InitShaders();
        void CreateBuffer();

        void SetShaderParameters(RenderObjectLine* aObject);
        void UpdateVertexes(RenderObjectLine* aObject);
        ComPtr<ID3D11Buffer> myVertexBuffer;
        SimpleVertex myVertices[2] = {{},{}};
    };
}