// TGP.cpp : Defines the entry point for the application.
//

#include <fstream>
#include <iomanip>
#include <tga2d/light/LightManager.h>
#include <tga2d/graphics/AmbientLight.h>
#include <tga2d/graphics/DirectionalLight.h>
#include <tga2d/graphics/PointLight.h>
#include <tga2d/input/InputManager.h>
#include <tga2d/error/ErrorManager.h>
#include <tga2d/graphics/Camera.h>
#include <tga2d/graphics/DX11.h>
#include <tga2d/drawers/ModelDrawer.h>
#include <tga2d/graphics/FullscreenEffect.h>
#include <tga2d/graphics/ForwardRenderer.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/RenderTarget.h>
#include <tga2d/Model/ModelFactory.h>
#include <tga2d/Model/ModelInstance.h>
#include <tga2d/graphics/RenderStateManager.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/Timer.h>
#ifdef _DEBUG
#pragma comment(lib,"tga2dcore_Debug.lib")
#pragma comment(lib,"External_Debug.lib")
#else
#pragma comment(lib,"tga2dcore_Release.lib")
#pragma comment(lib,"External_Release.lib")
#endif // _DEBUG

#pragma comment (lib, "assimp-vc142-mt.lib")
#pragma comment (lib, "d3d11.lib")

using namespace Tga2D;
struct RenderData
{
    RenderTarget* myBackBuffer;

    std::shared_ptr<ModelInstance> myModel;
    std::shared_ptr<AnimatedModelInstance> myAnimatedModel;
    std::shared_ptr<DirectionalLight> myDirectionalLight;
    std::shared_ptr<AmbientLight> myAmbientLight;

    std::shared_ptr<Camera> myMainCamera;

    Tga2D::SpriteSharedData mySpriteSharedData;
    Tga2D::Sprite3DInstanceData mySpriteInstanceData;

    //platform for the modells (I put it here to avoid being global)
    std::shared_ptr<ModelInstance> myPlane;

};

RenderData GlobalRenderData;
void Render(RenderData& renderData, GraphicsEngine& graphicsEngine)
{

    ////////////////////////////////////////////////////////////////////////////////
    //// Update Camera

    const std::shared_ptr<Camera> renderCamera = renderData.myMainCamera;
    graphicsEngine.SetCamera(*renderCamera);
    graphicsEngine.UpdateAndBindCommonBuffer();
    Engine::GetInstance()->GetLightManager().SetDirectionalLight(*renderData.myDirectionalLight);
    Engine::GetInstance()->GetLightManager().SetAmbientLight(*renderData.myAmbientLight);

    graphicsEngine.UpdateAndBindLightBuffer();
    graphicsEngine.GetForwardRenderer().SetEnvironmentMap(*renderData.myAmbientLight);

    ////////////////////////////////////////////////////////////////////////////////
    //// Draw all forward rendered objects

    DX11::RenderStateManager->SetBlendState(BlendState::Disabled);
    renderData.myBackBuffer->SetAsActiveTarget(DX11::DepthBuffer);

    if(renderData.myModel)
    {
        graphicsEngine.GetForwardRenderer().Render(*renderData.myModel);
    }
    if(renderData.myAnimatedModel)
    {
        graphicsEngine.GetForwardRenderer().Render(*renderData.myAnimatedModel);
    }
    graphicsEngine.GetForwardRenderer().Render(*renderData.myPlane);

    ////////////////////////////////////////////////////////////////////////////////

    {
        Tga2D::SpriteBatchScope batch = graphicsEngine.GetSpriteDrawer().BeginBatch(renderData.mySpriteSharedData);
        batch.Draw(renderData.mySpriteInstanceData);
    }

    renderData.myBackBuffer->SetAsActiveTarget();
}

void Go(void);
int main(const int /*argc*/, const char* /*argc*/[])
{
    Go();
    return 0;
}

Tga2D::InputManager* SInputManager;
void CreatePreviewForAsset(std::wstring filePath)
{
    ModelFactory& modelFactory = ModelFactory::GetInstance();
    bool hasMesh = modelFactory.ModelHasMesh(filePath);

    if (hasMesh) 
    {
        std::shared_ptr <Tga2D::AnimatedModelInstance> animatedModel = std::make_shared<Tga2D::AnimatedModelInstance>(modelFactory.GetAnimatedModel(filePath));

        if (animatedModel->IsValid()) //If the model has an animation
        {
            GlobalRenderData.myAnimatedModel = animatedModel;
            GlobalRenderData.myModel = nullptr;
        }
        else
        {
            std::shared_ptr<ModelInstance> currentModelToPreview = std::make_shared<ModelInstance>(modelFactory.GetModel(filePath));
            if (currentModelToPreview->IsValid())
            {
                GlobalRenderData.myModel = currentModelToPreview;
                GlobalRenderData.myAnimatedModel = nullptr;
            }
        }
    }
    else if (!hasMesh && GlobalRenderData.myAnimatedModel)
    {
        std::vector<std::wstring> animationPaths;
        animationPaths.push_back(filePath);
        GlobalRenderData.myAnimatedModel->InitAnimations(animationPaths);
    }
    //assert(!hasMesh && !GlobalRenderData.myAnimatedModel, "Error - No valid model was found");
    // error!
}

bool IsFBX(std::wstring aPath) //Return true if the filepath ends with .fbx (this is for the drag-and-drop model viewer)
{
    if (aPath.substr(aPath.find_last_of(L".") + 1) == L"fbx")
    {
        return true;
    }
    else
    {
        return false;
    }
}
// This is where the application starts of for real. By keeping this in it's own file
// we will have the same behaviour for both console and windows startup of the
// application.
//
void Go(void)
{
	Tga2D::EngineCreateParameters createParameters;

	createParameters.myWinProcCallback = [](HWND, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DROPFILES:
		{
			HDROP hDropInfo = (HDROP)wParam;

			UINT num_paths = DragQueryFileW(hDropInfo, 0xFFFFFFFF, 0, 512);

			wchar_t* filename = nullptr;
			UINT max_filename_len = 0;

			for (UINT i = 0; i < num_paths; ++i) {
				UINT filename_len = DragQueryFileW(hDropInfo, i, nullptr, 512) + 1;
				if (filename_len > max_filename_len) {
					max_filename_len = filename_len;
					wchar_t* tmp = (wchar_t*)realloc(filename, max_filename_len * sizeof(*filename));
					if (tmp != nullptr) {
						filename = tmp;
					}
				}
				DragQueryFileW(hDropInfo, i, filename, filename_len);
			}
			if (filename > 0 && IsFBX(filename))
                CreatePreviewForAsset(filename);
			free(filename);
			DragFinish(hDropInfo);
		}
		}
		SInputManager->UpdateEvents(message, wParam, lParam);
		return 0;
	};

    createParameters.myApplicationName = L"TGA 2D Tutorial 19";
    createParameters.myActivateDebugSystems = Tga2D::DebugFeature::Fps |
        Tga2D::DebugFeature::Mem |
        Tga2D::DebugFeature::Drawcalls |
        Tga2D::DebugFeature::Cpu |
        Tga2D::DebugFeature::Filewatcher |
        Tga2D::DebugFeature::OptimizeWarnings;

    if (!Tga2D::Engine::Start(createParameters))
    {
        ERROR_PRINT("Fatal error! Engine could not start!");
        system("pause");
        return;
    }

    {
        Vector2ui resolution = Tga2D::Engine::GetInstance()->GetRenderSize();
        bool bShouldRun = true;

        //RenderData renderData;
        {
            GlobalRenderData.myBackBuffer = DX11::BackBuffer;
        }

        HWND windowHandle = *Tga2D::Engine::GetInstance()->GetHWND();
        DragAcceptFiles(windowHandle, TRUE);
        Tga2D::InputManager myInputManager(windowHandle);
        SInputManager = &myInputManager;
        bool isMouseTrapped = false;

        // TODO: ModelFactory needs to spit out shared ptrs.
        GlobalRenderData.myPlane = std::make_shared<ModelInstance>(ModelFactory::GetInstance().GetModel(L"Plane"));
        GlobalRenderData.myPlane->SetScale({ 10 });
        std::wstring cubeMap = L"../source/tutorials/Tutorial19ModelViewer/data/cube_1024_preblurred_angle3_Skansen3.dds";

        auto aLight = AmbientLight(
            cubeMap,
            Color{ 1, 1, 1 },
            0.f
        );

        auto dLight = DirectionalLight(
            Transform{
                Vector3f(0, 0, 0),
                Rotator(225, 0, 0)
            },
            Color{ 1, 1, 1 },
            0.f
        );
        GlobalRenderData.myDirectionalLight = std::make_shared<DirectionalLight>(dLight);
        GlobalRenderData.myAmbientLight = std::make_shared<AmbientLight>(aLight);
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());

        float camSpeed = 1000.f;
        float camRotSpeed = 1000.f;
        camera->SetPerspectiveProjection(
            90,
            {
                (float)resolution.x,
                (float)resolution.y
            },
            0.1f,
            50000.0f)
            ;

        camera->SetPosition(Vector3f(0.0f, 500.0f, -550.0f));
        camera->SetRotation(Rotator(45, 0, 0));

        GlobalRenderData.myMainCamera = camera;
        
        MSG msg = { 0 };

        Timer timer;

        while (bShouldRun)
        {
            timer.Update();
            myInputManager.Update();

            if (GlobalRenderData.myAnimatedModel) //If the current model has an animation
            {
                if(GlobalRenderData.myAnimatedModel->GetAnimationCount() > 0)
                { 
                GlobalRenderData.myAnimatedModel->SetCurrentAnimationIndex(GlobalRenderData.myAnimatedModel->GetAnimationCount() - 1);
                GlobalRenderData.myAnimatedModel->Update(timer.GetDeltaTime());
                }
            }
            // Only read 3D navigation input if
            // the mouse is currently trapped.
#pragma region Camera
            Transform camTransform = camera->GetTransform();
            Vector3f camMovement = Vector3f::Zero;
            Rotator camRotation = Rotator::Zero;
            if (isMouseTrapped)
            {
                if (myInputManager.IsKeyHeld(0x57)) // W
                {
                    camMovement += camTransform.GetMatrix().GetForward() * 1.0f;
                }
                if (myInputManager.IsKeyHeld(0x53)) // S
                {
                    camMovement += camTransform.GetMatrix().GetForward() * -1.0f;
                }
                if (myInputManager.IsKeyHeld(0x41)) // A
                {
                    camMovement += camTransform.GetMatrix().GetRight() * -1.0f;
                }
                if (myInputManager.IsKeyHeld(0x44)) // D
                {
                    camMovement += camTransform.GetMatrix().GetRight() * 1.0f;
                }

                camera->SetPosition(camera->GetTransform().GetPosition() + camMovement * camSpeed * timer.GetDeltaTime());

                const Vector2f mouseDelta = myInputManager.GetMouseDelta();

                camRotation.X = mouseDelta.Y;
                camRotation.Y = mouseDelta.X;

                camTransform.AddRotation(camRotation * camRotSpeed * timer.GetDeltaTime());

                camera->SetRotation(camTransform.GetRotation());
            }

            if (myInputManager.IsKeyPressed(VK_RBUTTON))
            {
                // Capture mouse.
                if (!isMouseTrapped)
                {
                    myInputManager.HideMouse();
                    myInputManager.CaptureMouse();
                    isMouseTrapped = true;
                }
            }

            if (myInputManager.IsKeyReleased(VK_RBUTTON))
            {
                // When we let go, recapture.
                if (isMouseTrapped)
                {
                    myInputManager.ShowMouse();
                    myInputManager.ReleaseMouse();
                    isMouseTrapped = false;
                }
            }

            if (myInputManager.IsKeyPressed(VK_SHIFT))
            {
                // When we hold shift, "sprint".
                camSpeed /= 4;
            }

            if (myInputManager.IsKeyReleased(VK_SHIFT))
            {
                // When we let go, "walk".
                camSpeed *= 4;
            }

            if (myInputManager.IsKeyPressed(VK_ESCAPE))
            {
                PostQuitMessage(0);
            }

            if (!Tga2D::Engine::GetInstance()->BeginFrame())
            {
                break;
            }
#pragma endregion


            Render(GlobalRenderData, Tga2D::Engine::GetInstance()->GetGraphicsEngine());

            Tga2D::Engine::GetInstance()->EndFrame();
        }
    }

    Tga2D::Engine::GetInstance()->Shutdown();

    return;
}
