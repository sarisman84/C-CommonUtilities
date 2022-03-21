#include <string>

#include <tga2d/text/text.h>
#include "../../TutorialCommon/TutorialCommon.h"
#include <tga2d/drawers/DebugDrawer.h>
#include <tga2d/shaders/SpriteShader.h>

// Making sure that DX2DEngine lib is linked
//
#ifdef _DEBUG
#pragma comment(lib,"tga2dcore_Debug.lib")
#pragma comment(lib,"External_Debug.lib")
#else
#pragma comment(lib,"tga2dcore_Release.lib")
#pragma comment(lib,"External_Release.lib")
#endif // _DEBUG

void Go( void );
int main( const int /*argc*/, const char * /*argc*/[] )
{
    Go();

    return 0;
}

std::string myRandomTexts[3]
{
	"Enkel text",
	"Lite svårare text!",
	"Lagom svår text",
};
std::string GetRandomText()
{
	return myRandomTexts[rand() % 3];
}

void Go( void )
{
	{
		TutorialCommon::Init(L"TGA2D: Tutorial 4");

		Tga2D::Text myTextBombardiere(L"Text/BarlowSemiCondensed-Regular.ttf", Tga2D::FontSize_18);
		myTextBombardiere.SetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris rutrum dolor \ntristique suscipit ultricies. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.\nUt ornare bibendum lacus, vel ultrices ipsum semper ut. Sed tempus consectetur lectus in malesuada. Curabitur tortor magna, euismod eu mi ac, maximus posuere arcu. \nProin bibendum libero metus, sit amet ornare magna pharetra a. Curabitur hendrerit arcu vel mi scelerisque, non congue elit sagittis. Duis placerat orci sed a\nnte luctus efficitur. Sed feugiat libero eget blandit volutpat. Proin leo enim, porta ut metus eget, maximus ultrices erat.");
		myTextBombardiere.SetPosition({ 0.1f, 0.5f });
		myTextBombardiere.SetColor(Tga2D::Color(1.0f, 0.6f, 0.8f, 0.8f));

		Tga2D::Text myPlainText(L"Text/arial.ttf", Tga2D::FontSize_48);
		myPlainText.SetText(GetRandomText());
		myPlainText.SetPosition({ 0.2f, 0.3f });

		Tga2D::Text myPlainTextInfo(L"Text/arial.ttf", Tga2D::FontSize_18);
		myPlainTextInfo.SetText("");
		myPlainTextInfo.SetPosition({ 0.2f, 0.38f });

		Tga2D::Text myTextArial(L"Text/arial.ttf", Tga2D::FontSize_48);
		myTextArial.SetText("Text med egen shader!");
		myTextArial.SetPosition({ 0.6f, 0.7f });

		// Create a new shader
		Tga2D::SpriteShader customShader;
		customShader.Init(L"shaders/instanced_sprite_shader_VS.cso", L"Tutorial4/Shaders/custom_sprite_pixel_shader_PS.cso");

		const float deltaTime = 1.0f / 60.0f;
		float myTimer = 0;
		float changeTextTimer = 1.0f;
		while (true)
		{
			myTimer += deltaTime;
			if (!Tga2D::Engine::GetInstance()->BeginFrame())
			{
				break;
			}

			myTextBombardiere.SetRotation(static_cast<float>(cos(myTimer) * 0.1f));
			myTextBombardiere.Render();
			myTextArial.Render(&customShader);

			changeTextTimer -= deltaTime;
			if (changeTextTimer <= 0)
			{
				myPlainText.SetText(GetRandomText());
				changeTextTimer = 1.0f;
			}

			myPlainText.Render();
			Tga2D::Engine::GetInstance()->GetDebugDrawer().DrawArrow(myPlainText.GetPosition() + Tga2D::Vector2f(0, 0.03f), myPlainText.GetPosition() + Tga2D::Vector2f(myPlainText.GetWidth(), 0.03f));

			std::string textInfo = "Size: " + std::to_string(myPlainText.GetWidth());
			myPlainTextInfo.SetText(textInfo);
			myPlainTextInfo.Render();
			Tga2D::Engine::GetInstance()->EndFrame();
		}
	}

	Tga2D::Engine::GetInstance()->Shutdown();
}
