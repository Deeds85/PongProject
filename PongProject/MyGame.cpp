#include "MyDirectX.h"
#include "Ball.h"
#include "Paddle.h"
using namespace std;

const string APPTITLE = "Pong Project";
const int SCREENW = 1024;
const int SCREENH = 768;
int m_leftScore, m_rightScore = 0;

Ball m_ball;
Paddle m_paddles[2];
LPD3DXFONT m_font = NULL;

bool GameInit(HWND window)
{
    //initialize Direct3D
    Direct3DInit(window, SCREENW, SCREENH, false);

    //initialize DirectInput
    DirectInputInit(window);

	m_leftScore = m_rightScore = 0;
	m_ball = Ball();
	m_paddles[0] = Paddle(50, SCREENH / 2, 0, false);
	m_paddles[1] = Paddle(SCREENW - 50 - 16, SCREENH / 2, 3.14159, true);
	m_font = MakeFont("Times New Roman", 48);
	
	return true;
}

void GameUpdate(HWND window)
{
    //make sure the Direct3D device is valid
    if (!g_d3ddev)
	{
		MessageBox(window, "Direct3D device is not valid", "Error", 0);
		return;
	}

    //update input devices
    DirectInputUpdate();

    //clear the scene
    g_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

    //exit when escape key is pressed
    if (KeyDown(DIK_ESCAPE))
		g_GameOver = true;

	//update objects in the world
	m_ball.Update(m_paddles, &m_leftScore, &m_rightScore);
	float aiTarget = m_ball.GetBallHeight();
	m_paddles[0].Update(aiTarget);
	m_paddles[1].Update(aiTarget);
}

void GameRender(HWND window)
{
	//start rendering
    if (g_d3ddev->BeginScene())
    {
        g_spriteObj->Begin(D3DXSPRITE_ALPHABLEND);

		//draw the score
		ostringstream left, right;
		left << m_leftScore;
		right << m_rightScore;
		string score = left.str() + ':' + right.str();
		RECT rect = {480,50,600,700};
		m_font->DrawText(NULL, score.c_str(), score.length(), &rect, DT_NOCLIP|DT_SINGLELINE, D3DCOLOR_XRGB(255,255,255));

		//draw the ball
		m_ball.Render();

		//draw the paddles
		m_paddles[0].Render();
		m_paddles[1].Render();		

        //stop rendering
		g_spriteObj->End();
        g_d3ddev->EndScene();
        g_d3ddev->Present(NULL, NULL, NULL, NULL);
    }
}

void GameEnd()
{
    DirectInputShutdown();
    Direct3DShutdown();
}