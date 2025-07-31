
#include "stdafx.h"
#include "Game.h"

//#define MAX_LOADSTRING  100
//
//
//WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
//WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
//
//
//HINSTANCE hInst;
Game game;


//bool InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//    hInst = hInstance; // Store instance handle in our global variable
//
//    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
//        0, 0, WIDTH, HEIGHT, nullptr, nullptr, hInstance, nullptr);
//
//    if (!hWnd)
//    {
//        return FALSE;
//    }
//
//    ShowWindow(hWnd, nCmdShow);
//    UpdateWindow(hWnd);
//
//    game.Init(hWnd, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);
//
//    return TRUE;
//}


int main() {
    bool running = true;
    if (!game.Init())
    {
        return EXIT_FAILURE;
    }
    while (running)
    {
        try {
            if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
                running = false;
            game.ProcessInput();
            game.Update();
            game.DrawFrame();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    game.ShutDown();
    return EXIT_SUCCESS;
}