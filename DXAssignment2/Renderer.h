#pragma once

#include "Model.h"
#include "Errors.h"
#include "Defines.h"
#include <d3d9.h>
#include <d3dx9.h>

#define DRVERTEX_FLAGS (D3DFVF_XYZ | D3DFVF_DIFFUSE)
struct DRVERTEX {
	float x, y, z;
	DWORD DiffuseColor;
};

// This is a Transformed vertex with a Diffuse colour component.
struct TransformedColouredVertex {
	float x, y, z, rhw;
	DWORD colour;

	static const DWORD FORMAT = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
	static const int STRIDE_SIZE = 20;
};

// Logically, we'll call our new untransformed vertex format UntransformedColouredVertex. An
// Untransformed vertex only holds a logical x, y, z point in space. It does not hold rhw.
struct UntransformedColouredVertex {
	float x, y, z;
	DWORD colour;

	// The format is ever so slightly different, and the stride size will be just 16 in
	// this format.
	static const DWORD FORMAT = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	static const int STRIDE_SIZE = 16;
};


class Renderer {
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer_;
	LPDIRECT3D9 pD3D_; //COM object
	LPDIRECT3DDEVICE9 pDevice_; //graphics device
	LPDIRECT3DSURFACE9 pBackBuffer_; //back buffer

	// Set up the Direct3D display, with a specific width, height, fullscreen mode, and more.
	int InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice);

	// Handles 2D drawing on the background before the main 3D scene.
	void PreScene2D(Model&);

	// Render the 3D scene provided in Model
	void Scene3D(Model&);

	// Handles 2D drawing on the frame after the 3D work is complete.
	void PostScene2D(Model&);
public:
	Renderer();
	~Renderer();
	
	// Create the Direct3D COM object and initialize the device.
	int startEngine(HWND hwnd, Model& model);
	
	// Release the device context, and the Direct3D COM object.
	int stopEngine();

	/* Main rendering function, called directly on every frame.
	Engine::render delegates to PreScene2D, Scene3D, and PostScene2D to draw.
	*/
	int render(Model& model);

	/* Getter for the DX device. */
	LPDIRECT3DDEVICE9& getDevice();
};

