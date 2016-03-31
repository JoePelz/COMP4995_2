
#include "Defines.h"
#include <windows.h>
#include "Controller.h"

/*
Summary:
	Program entry point.  
	Registers and instantiates a window to contain the DirectX viewport. 
	Triggers the main message loop and hands control off to the controller class.
Params: 
	hInstance: The running program instance
	hPrevInstance: ignored.
	pstrCmdLine: Any command line arguments as a unicode string.
	iCmdShow: flag determining (minimized|maximized|shown)
Return: Exit code of the program.
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pstrCmdLine, int iCmdShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wc;

	static TCHAR strAppName[] = TEXT("Joe Pelz - DX Assignment 2");

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.cbClsExtra = sizeof(void*);
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = Controller::windowLoop;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_HAND);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCWSTR) strAppName;

	RegisterClassEx(&wc);

	Controller controller(hInstance);

	hWnd = CreateWindowEx(NULL,
		(LPCWSTR)strAppName,
		(LPCWSTR)strAppName,
		USE_FULLSCREEN ? WS_POPUP | WS_SYSMENU | WS_VISIBLE : WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		657, 520,
		NULL,
		NULL,
		hInstance,
		&controller);

	controller.setHWnd(hWnd);
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	try {
		controller.GameStartup();
	} catch (LPCTSTR error) {
		_tprintf_s(TEXT("%s"), error);
		controller.GameShutdown();
		return 0; //shutdown the program
	}


	while (TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			controller.GameLoop();
		}
	}

	controller.GameShutdown();// clean up the game

	return msg.wParam;
}