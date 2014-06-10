#include "Ball.h"

Ball::Ball()
{
	m_img = LoadTexture("PongBall.png");
	m_ballSprite.width = m_ballSprite.height = 16;
	m_ballSprite.x = SCREENW / 2;
	m_ballSprite.y = SCREENH / 2;
	m_ballSprite.velx = 5.0f;
	m_ballSprite.vely = 5.0f;
}

void Ball::Update(Paddle paddles[], int *leftScore, int *rightScore)
{
	
	
	RECT intersection;
	float displacement;

	if(Collision(paddles[0].m_paddleSprite, m_ballSprite, &intersection))
	{
		paddles[0].HitBall(intersection, &m_ballSprite.velx, &m_ballSprite.vely, &displacement);
		m_ballSprite.x += displacement;
	}

	if(Collision(paddles[1].m_paddleSprite, m_ballSprite, &intersection))
	{
		paddles[1].HitBall(intersection, &m_ballSprite.velx, &m_ballSprite.vely, &displacement);
		m_ballSprite.x += displacement;
	}

	if(m_ballSprite.x > SCREENW - m_ballSprite.width)
	{
		*leftScore += 1;
		Reset();
	}

	if(m_ballSprite.x < 0)
	{
		*rightScore += 1;
		Reset();
	}

	if(m_ballSprite.y > SCREENH - m_ballSprite.height || m_ballSprite.y < 0)
		m_ballSprite.vely *= -1;

	m_ballSprite.x += m_ballSprite.velx;
	m_ballSprite.y += m_ballSprite.vely;
}

void Ball::Reset()
{
	m_ballSprite.x = SCREENW / 2;
	m_ballSprite.y = SCREENH / 2;
	m_ballSprite.velx = 5.0f;
	m_ballSprite.vely = 5.0f;
}

void Ball::Render()
{
	SpriteTransformDraw(m_img, m_ballSprite.x, m_ballSprite.y, m_ballSprite.width, m_ballSprite.height);
}

float Ball::GetBallHeight()
{
	return m_ballSprite.y;
}