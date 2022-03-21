#pragma once

namespace Tga2D
{
	class Texture;
}

class GameWorld
{
public:
	GameWorld(); 
	~GameWorld();

	void Init();
	void Update(float aTimeDelta); 
	void Render();
private:
	Tga2D::Texture* myTga2dLogoTexture;
};