#include "MyDirectX.h"
using namespace std;

bool g_GameOver = false;

const int FRAMES_PER_SECOND = 50;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
const int MAX_FRAMESKIP = 10;

LRESULT WINAPI WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            g_GameOver = true;
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //initialize window settings
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX); 
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinProc;
    wc.cbClsExtra	 = 0;
    wc.cbWndExtra	 = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = APPTITLE.c_str();
    wc.hIconSm       = NULL;
    RegisterClassEx(&wc);

    //create a new window
    HWND window = CreateWindow( APPTITLE.c_str(), APPTITLE.c_str(),
       WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
       SCREENW, SCREENH, NULL, NULL, hInstance, NULL);
    if (window == 0)
	{
		return 0;
	}

    //display the window
    ShowWindow(window, nCmdShow);
    UpdateWindow(window);
	
	//initialize the game
    if (!GameInit(window))
	{
		MessageBox(window, "Error initializing game.", "Error", 0);		
		return 0;
	}

	DWORD nextGameTick = GetTickCount();
	//int loops;

    //main message loop
	MSG message;
	while (!g_GameOver)
    {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) 
	    {
		    TranslateMessage(&message);
		    DispatchMessage(&message);
	    }

		//loops = 0;
		//while( GetTickCount() > nextGameTick && loops < MAX_FRAMESKIP)
		//{
		//	//Update game objects
		//	GameUpdate(window);

		//	nextGameTick += SKIP_TICKS;
		//	loops++;
		//}
  //      
		////Render game objects
		//GameRender(window);

		do 
		{
			GameRender(window);
		} while (GetTickCount() < nextGameTick);

		GameUpdate(window);
		nextGameTick += SKIP_TICKS;
    }

    //shutdown
    GameEnd();
	return message.wParam;
}

