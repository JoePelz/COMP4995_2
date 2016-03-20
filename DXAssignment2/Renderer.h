#pragma once

#include "Model.h"
#include "Errors.h"
#include "Defines.h"
#include <d3d9.h>
#include <d3dx9.h>

/*
The Renderer class is meant to interface with the Direct3D COM object by managing device context, backbuffers, and rendering.
*/

class Renderer {
	D3DPRESENT_PARAMETERS parameters; //Direct3D Device parameters
	LPDIRECT3D9 pD3D_; //COM object
	LPDIRECT3DDEVICE9 pDevice_; //graphics device
	LPDIRECT3DSURFACE9 pBackBuffer_; //back buffer
	D3DXMATRIX reflection_; //reflection matrix
	D3DXPLANE clipPlane_; //clip plane for reflections

	// Set up the Direct3D display, with a specific width, height, fullscreen mode, and more.
	int InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3DDEVICE9* ppDevice);

	// Handles 2D drawing on the background before the main 3D scene.
	void PreScene2D(Model&);

	// Render the 3D scene provided in Model
	void Scene3D(Model&, D3DXMATRIX* xform);
	void RenderMirrors(Model&);

	// Handles 2D drawing on the frame after the 3D work is complete.
	void PostScene2D(Model&);
public:
	Renderer();
	~Renderer();
	
	// Create the Direct3D COM object and initialize the device.
	int startEngine(HWND hwnd, const Model& model);
	
	// Release the device context, and the Direct3D COM object.
	int stopEngine();

	void ChangeDisplayMode(const Model& model);

	/* Main rendering function, called directly on every frame.
	Engine::render delegates to PreScene2D, Scene3D, and PostScene2D to draw.
	*/
	int render(Model& model);

	/* Getter for the DX device. */
	LPDIRECT3DDEVICE9& getDevice();
};

