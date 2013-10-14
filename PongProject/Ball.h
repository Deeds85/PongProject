#pragma once
#include "MyDirectX.h"
#include "Paddle.h"

class Ball{
public:
	Ball();
	void Update(Paddle paddles[]);
	void Reset();
	void Render();
	float GetBallHeight();

private:
	SPRITE m_ballSprite;
	LPDIRECT3DTEXTURE9 m_img;
};