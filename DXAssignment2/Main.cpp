
#include "Defines.h"
#include <windows.h>
#include "Controller.h"

/* Program entry point (win32)
	Registers and instantiates a window to contain the DirectX viewport.
	Runs the main message loop, but hands control off to the Controller class.
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
		WS_OVERLAPPEDWINDOW,
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
	if (FAILED(controller.GameStartup())) {
		return E_FAIL;
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