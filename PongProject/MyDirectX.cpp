#include "MyDirectX.h"
#include <iostream>
using namespace std;

//Direct3D variables
LPDIRECT3D9 g_d3d = NULL; 
LPDIRECT3DDEVICE9 g_d3ddev = NULL; 
LPDIRECT3DSURFACE9 g_backBuffer = NULL;
LPD3DXSPRITE g_spriteObj;

//DirectInput variables
LPDIRECTINPUT8 g_dinput = NULL;
LPDIRECTINPUTDEVICE8 g_mouse = NULL;
LPDIRECTINPUTDEVICE8 g_keyboard = NULL;
DIMOUSESTATE g_mouseState;
char g_keys[256];


bool Direct3DInit(HWND window, int width, int height, bool fullscreen)
{
    //initialize Direct3D
    g_d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!g_d3d) return false;

    //set Direct3D presentation parameters
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.hDeviceWindow = window;
    d3dpp.Windowed = (!fullscreen);
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.EnableAutoDepthStencil = 1;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;

    //create Direct3D device
    g_d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_d3ddev);
    if (!g_d3ddev) return false;


    //get a pointer to the back buffer surface
    g_d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &g_backBuffer);


    //create sprite object
    D3DXCreateSprite(g_d3ddev, &g_spriteObj);

    return 1;
}

void Direct3DShutdown()
{
    if (g_spriteObj) g_spriteObj->Release();

    if (g_d3ddev) g_d3ddev->Release();
    if (g_d3d) g_d3d->Release();
}

void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source)
{
    //get width/height from source surface
    D3DSURFACE_DESC desc;
    source->GetDesc(&desc);

    //create rects for drawing
    RECT source_rect = {0, 0, (long)desc.Width, (long)desc.Height };
    RECT dest_rect = { (long)x, (long)y, (long)x+desc.Width, (long)y+desc.Height};
    
    //draw the source surface onto the dest
    g_d3ddev->StretchRect(source, &source_rect, dest, &dest_rect, D3DTEXF_NONE);

}

LPDIRECT3DSURFACE9 LoadSurface(string filename)
{
    LPDIRECT3DSURFACE9 image = NULL;
    
    //get width and height from bitmap file
    D3DXIMAGE_INFO info;
    HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
    if (result != D3D_OK)
        return NULL;

    //create surface
    result = g_d3ddev->CreateOffscreenPlainSurface(
        info.Width,         //width of the surface
        info.Height,        //height of the surface
        D3DFMT_X8R8G8B8,    //surface format
        D3DPOOL_DEFAULT,    //memory pool to use
        &image,             //pointer to the surface
        NULL);              //reserved (always NULL)

    if (result != D3D_OK) return NULL;

    //load surface from file into newly created surface
    result = D3DXLoadSurfaceFromFile(
        image,                  //destination surface
        NULL,                   //destination palette
        NULL,                   //destination rectangle
        filename.c_str(),       //source filename
        NULL,                   //source rectangle
        D3DX_DEFAULT,           //controls how image is filtered
        D3DCOLOR_XRGB(0,0,0),   //for transparency (0 for none)
        NULL);                  //source image info (usually NULL)

    //make sure file was loaded okay
    if (result != D3D_OK) return NULL;

    return image;
}


LPDIRECT3DTEXTURE9 LoadTexture(std::string filename, D3DCOLOR transcolor)
{  
    LPDIRECT3DTEXTURE9 texture = NULL;

    //get width and height from bitmap file
    D3DXIMAGE_INFO info;
    HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
    if (result != D3D_OK) return NULL;

    //create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx( 
        g_d3ddev,                //Direct3D device object
        filename.c_str(),      //bitmap filename
        info.Width,            //bitmap image width
        info.Height,           //bitmap image height
        1,                     //mip-map levels (1 for no chain)
        D3DPOOL_DEFAULT,       //the type of surface (standard)
        D3DFMT_UNKNOWN,        //surface format (default)
        D3DPOOL_DEFAULT,       //memory class for the texture
        D3DX_DEFAULT,          //image filter
        D3DX_DEFAULT,          //mip filter
        transcolor,            //color key for transparency
        &info,                 //bitmap file info (from loaded file)
        NULL,                  //color palette
        &texture );            //destination texture

    //make sure the bitmap textre was loaded correctly
    if (result != D3D_OK) return NULL;

	return texture;
}


void SpriteDrawFrame(LPDIRECT3DTEXTURE9 texture, int destx, int desty, int framenum, int framew, int frameh, int columns)
{
	D3DXVECTOR3 position( (float)destx, (float)desty, 0 );
	D3DCOLOR white = D3DCOLOR_XRGB(255,255,255);

	RECT rect;
 	rect.left = (framenum % columns) * framew;
	rect.top = (framenum / columns) * frameh;
	rect.right = rect.left + framew;
	rect.bottom = rect.top + frameh;

	g_spriteObj->Draw( texture, &rect, NULL, &position, white);
}

void SpriteAnimate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay)
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		frame += direction;
		if (frame > endframe) frame = startframe;
		if (frame < startframe) frame = endframe;
	}	
}
void SpriteTransformDraw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height, 
    int frame, int columns, float rotation, float scaling, D3DCOLOR color)
{
    //create a scale vector
    D3DXVECTOR2 scale( scaling, scaling );

    //create a translate vector
    D3DXVECTOR2 trans( x, y );

    //set center by dividing width and height by two
    D3DXVECTOR2 center( (float)( width * scaling )/2, (float)( height * scaling )/2);

    //create 2D transformation matrix
    D3DXMATRIX mat;
    D3DXMatrixTransformation2D( &mat, NULL, 0, &scale, &center, rotation, &trans );
    
    //tell sprite object to use the transform
    g_spriteObj->SetTransform( &mat );

    //calculate frame location in source image
    int fx = (frame % columns) * width;
    int fy = (frame / columns) * height;
    RECT srcRect = {fx, fy, fx + width, fy + height};

    //draw the sprite frame
    g_spriteObj->Draw( image, &srcRect, NULL, NULL, color );
}

LPD3DXFONT MakeFont(string name, int size)
{
	LPD3DXFONT font = NULL;
	D3DXFONT_DESC desc = {
		size,				//height
		0,					//width
		0,					//weight
		0,					//miplevels
		false,				//italic
		DEFAULT_CHARSET,	//charset
		OUT_TT_PRECIS,		//output precision
		CLIP_DEFAULT_PRECIS,//quality
		DEFAULT_PITCH,		//pitch family
		""};				//font name
	
	strcpy(desc.FaceName, name.c_str());
	D3DXCreateFontIndirect(g_d3ddev, &desc, &font);
	return font;
}

void FontPrint(LPD3DXFONT font, int x, int y, string text, D3DCOLOR color)
{
	//figure out the text boundary
	RECT rect = {x, y, 0, 0};
	font->DrawText(NULL, text.c_str(), text.length(), &rect, DT_CALCRECT, color);

	//print the text
	font->DrawText(g_spriteObj, text.c_str(), text.length(), &rect, DT_LEFT, color);
}

bool DirectInputInit(HWND hwnd)
{
    //initialize DirectInput object
    DirectInput8Create(
        GetModuleHandle(NULL), 
        DIRECTINPUT_VERSION, 
        IID_IDirectInput8,
        (void**)&g_dinput,
        NULL);

    //initialize the keyboard
    g_dinput->CreateDevice(GUID_SysKeyboard, &g_keyboard, NULL);
    g_keyboard->SetDataFormat(&c_dfDIKeyboard);
    g_keyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    g_keyboard->Acquire();

    //initialize the mouse
    g_dinput->CreateDevice(GUID_SysMouse, &g_mouse, NULL);
    g_mouse->SetDataFormat(&c_dfDIMouse);
    g_mouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    g_mouse->Acquire();
    g_d3ddev->ShowCursor(false);

    return true;
}

void DirectInputUpdate()
{
    //update mouse
    g_mouse->Poll();
    if (!SUCCEEDED(g_mouse->GetDeviceState(sizeof(DIMOUSESTATE),&g_mouseState)))
    {
        //mouse device lose, try to re-acquire
        g_mouse->Acquire();
    }

    //update keyboard
    g_keyboard->Poll();
    if (!SUCCEEDED(g_keyboard->GetDeviceState(256,(LPVOID)&g_keys)))
    {
        //keyboard device lost, try to re-acquire
        g_keyboard->Acquire();
    }
}


int MouseX()
{
    return g_mouseState.lX;
}

int MouseY()
{
    return g_mouseState.lY;
}

int MouseButton(int button)
{
    return g_mouseState.rgbButtons[button] & 0x80;
}

bool KeyDown(int key) 
{
    return (bool)(g_keys[key] & 0x80);
}


void DirectInputShutdown()
{
    if (g_keyboard) 
    {
        g_keyboard->Unacquire();
        g_keyboard->Release();
        g_keyboard = NULL;
    }
    if (g_mouse) 
    {
        g_mouse->Unacquire();
        g_mouse->Release();
        g_mouse = NULL;
    }
}

//bounding box collision detection
int Collision(SPRITE sprite1, SPRITE sprite2)
{
	RECT rect1;
	rect1.left = (long)sprite1.x;
	rect1.top = (long)sprite1.y;
	rect1.right = (long)sprite1.x + sprite1.width * sprite1.scaling;
	rect1.bottom = (long)sprite1.y + sprite1.height * sprite1.scaling;
	

	RECT rect2;
	rect2.left = (long)sprite2.x;
	rect2.top = (long)sprite2.y;
	rect2.right = (long)sprite2.x + sprite2.width * sprite2.scaling;
	rect2.bottom = (long)sprite2.y + sprite2.height * sprite2.scaling;
	

	RECT dest; //ignored
	return IntersectRect(&dest, &rect1, &rect2);
}
