#pragma once

#include <Windows.h>
#include <windowsx.h>
#include "Renderer.h"
#include "Background.h"
#include "FrameRate.h"
#include "Camera.h"
#include "Mesh.h"
#include "Input.h"
#include "ITransform.h"
#include "Cube.h"
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
	//State of user input (keys or buttons down)
	Input input;
	//TODO: hide this within mesh object.
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer_;

	void initializeResources();
	void releaseResources();
	void updateModel(const Input& input, Model& model);
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
	/* Handles keyboard interaction. Called on key presses. Returns true if key is handled. */
	bool KeyDown(WPARAM key);
	/* Handles keyboard interaction. Called on key releases. Returns true if key is handled. */
	bool KeyUp(WPARAM key);

	void Abort(int errorCode);

	//Starts the renderer (initializes DirectX. See Renderer::startEngine(HWND, Model))
	void GameStartup();

	//Main function that executes every frame.  Updates the model and calls the renderer.
	void GameLoop();

	//Stops the renderer and closes the DirectX context.  See Renderer::stopEngine()
	void GameShutdown();
};

