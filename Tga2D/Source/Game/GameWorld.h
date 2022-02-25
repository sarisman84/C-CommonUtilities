#pragma once

namespace Tga2D
{
	class CTexture;
}

class CGameWorld
{
public:
	CGameWorld(); 
	~CGameWorld();

	void Init();
	void Update(float aTimeDelta); 
	void Render();
private:
	Tga2D::CTexture* myTga2dLogoTexture;
};