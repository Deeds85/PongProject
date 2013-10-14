#pragma once

//header files
#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

//libraries
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")

//program values
extern const string APPTITLE;
extern const int SCREENW;
extern const int SCREENH;
extern bool g_GameOver;

//macro to detect key presses
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

//Direct3D objects
extern LPDIRECT3D9 g_d3d; 
extern LPDIRECT3DDEVICE9 g_d3ddev; 
extern LPDIRECT3DSURFACE9 g_backBuffer;
extern LPD3DXSPRITE g_spriteObj;

//sprite structure
struct SPRITE
{
	float x,y;
	int frame, columns;
	int width, height;
	float scaling, rotation;
	int startframe, endframe;
	int starttime, delay;
	int direction;
	float velx, vely;
	D3DCOLOR color;

	SPRITE()
	{
		frame = 0;
		columns = 1;
		width = height = 0;
		scaling = 1.0f;
		rotation = 0.0f;
		startframe = endframe = 0;
		direction = 1;
		starttime = delay = 0;
		velx = vely = 0.0f;
		color = D3DCOLOR_XRGB(255,255,255);
	}
};

//Bounding box collision detection
int Collision(SPRITE sprite1, SPRITE sprite2);

//Direct3D functions
bool Direct3DInit(HWND hwnd, int width, int height, bool fullscreen);
void Direct3DShutdown();
LPDIRECT3DSURFACE9 LoadSurface(string filename);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0,0,0));

//Sprite Functions
void SpriteDrawFrame(LPDIRECT3DTEXTURE9 texture, int destx, int desty, 
    int framenum, int framew, int frameh, int columns);
void SpriteAnimate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay);
void SpriteTransformDraw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height, 
    int frame = 0, int columns = 1, float rotation = 0.0f, float scaling = 1.0f, 
    D3DCOLOR color = D3DCOLOR_XRGB(255,255,255));

//Font Functions
LPD3DXFONT MakeFont(string name, int size);
void FontPrint(LPD3DXFONT font, int x, int y, string text, D3DCOLOR color = D3DCOLOR_XRGB(255,255,255));

//DirectInput objects, devices, and states
extern LPDIRECTINPUT8 g_dinput;
extern LPDIRECTINPUTDEVICE8 g_mouse;
extern LPDIRECTINPUTDEVICE8 g_keyboard;
extern DIMOUSESTATE g_mouseState;

//DirectInput functions
bool DirectInputInit(HWND);
void DirectInputUpdate();
void DirectInputShutdown();
bool KeyDown(int);
int MouseButton(int);
int MouseX();
int MouseY();

//game functions
bool GameInit(HWND window);
void GameUpdate(HWND window);
void GameRender(HWND window);
void GameEnd();


