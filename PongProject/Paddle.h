#pragma once
#include "MyDirectX.h"

class Paddle{
public:
	SPRITE m_paddleSprite;
	Paddle();	
	Paddle(float x, float y, float rotation, bool enableAI = false);
	void Update(float aiTarget);
	void Render();
	void HitBall(RECT intersect, float *velx, float *vely, float *displacement);

private:
	bool m_AIControlled;	
	LPDIRECT3DTEXTURE9 m_img;
};