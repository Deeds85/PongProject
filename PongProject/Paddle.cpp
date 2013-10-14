#include "Paddle.h"

Paddle::Paddle()
{
	//Default constructor. Do nothing
}

Paddle::Paddle(float x, float y, float rotation, bool enableAI)
{
	m_AIControlled = enableAI;
	m_img = LoadTexture("PongPaddle.png");
	m_paddleSprite.width = 16;
	m_paddleSprite.height = 133;
	m_paddleSprite.rotation = rotation;
	m_paddleSprite.x = x;
	m_paddleSprite.y = y;
}

void Paddle::Update(float aiTarget)
{
	if(!m_AIControlled)
	{
		if(KeyDown(DIK_UPARROW) && m_paddleSprite.y > 0)
		{
			m_paddleSprite.y -= 5.0f;
		}
		if(KeyDown(DIK_DOWNARROW) && m_paddleSprite.y < SCREENH - m_paddleSprite.height - 5)
		{
			m_paddleSprite.y += 5.0f;
		}
	}
	else
	{
		if (m_paddleSprite.y > aiTarget - (m_paddleSprite.height / 2) && m_paddleSprite.y > 0)
			m_paddleSprite.y -= 5.0f;
		if (m_paddleSprite.y < aiTarget - (m_paddleSprite.height / 2) && m_paddleSprite.y < SCREENH - m_paddleSprite.height - 5)
			m_paddleSprite.y += 5.0f;
	}
}

void Paddle::Render()
{
	SpriteTransformDraw(m_img, m_paddleSprite.x, m_paddleSprite.y, m_paddleSprite.width, m_paddleSprite.height,
		m_paddleSprite.frame, m_paddleSprite.columns, m_paddleSprite.rotation);
}

void Paddle::HitBall(float intersect, float *velx, float *vely)
{
	float paddleMiddle = m_paddleSprite.y + (m_paddleSprite.height / 2);
	float ballVelocity = 50;
	float x = abs(*velx); 
	float y = abs(*vely);
	
	if(intersect > paddleMiddle)
	{
		y = (1 - (paddleMiddle / intersect)) * ballVelocity;
		x = sqrt(ballVelocity - y);
		y = sqrt(y);
	}
	else if (intersect < paddleMiddle)
	{
		y = (intersect / paddleMiddle) * ballVelocity;
		x = sqrt(ballVelocity - y);
		y = sqrt(y) * -1;
	}
	else
	{
		y = 0.0f;
		x = sqrt(ballVelocity);
	}

	if(*velx > 0)
		x *= -1;

	*velx = x;
	*vely = y;
}