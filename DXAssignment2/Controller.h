#pragma once

#include <Windows.h>
#include <windowsx.h>
#include "Renderer.h"
#include "Background.h"
#include "FrameRate.h"
#include "Camera.h"
#include <d3d9.h>
#include <memory>

/* The Controller class manages interactions between the model and the renderer.  
	It handles input from the user and Windows messages. 
	If this program did anything, this class would also ensure that the thing happened.
*/
class Controller {
private:
	//Handle to the application instance.
	const HINSTANCE hInstance;
	//Handle to the top-level window.
	HWND hWnd;
	//The renderer that sets up the device context and draws pixels.
	Renderer renderEngine;
	//The current state of the program. (game data)
	Model gameModel;

public:
	//Simple constructor, just saves the hInstance.
	Controller(const HINSTANCE hInstance);
	~Controller(); //default destructor

	//Sets the handle to the top-level window to house the DirectX view.
	void setHWnd(const HWND newHWnd);

	//win32 window loop, to handle mouse, keyboard, and other windows messages.
	static long CALLBACK windowLoop(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

	/* handle mouse actions: starts new line. */
	void MouseDown(LPARAM pos);
	/* handle mouse actions: drags line endpoint. */
	void MouseMove(LPARAM pos);
	/* handle mouse actions: finalizes a new line. */
	void MouseUp(LPARAM pos);


	//Starts the renderer (initializes DirectX. See Renderer::startEngine(HWND, Model))
	// and sets the background.
	int GameStartup();

	//Main function that executes every frame.  Updates the model and calls the renderer.
	int GameLoop();

	//Stops the renderer and closes the DirectX context.  See Renderer::stopEngine()
	int GameShutdown();
};

