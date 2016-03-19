#include "Renderer.h"

/*
Summary:
	Initialize the Direct3D engine to be ready to begin rendering.
Params: 
	hwnd: Handle to the Win32 window that will draw the Direct3D data.
	model: The program state model containing resolution information.
Return: S_OK on success.
*/
int Renderer::startEngine(HWND hwnd, const Model& model) {
	HRESULT r = 0;//return values

	//4th argument is TRUE or FALSE, where FALSE means fullscreen.
	r = InitDirect3DDevice(hwnd, model.getWidth(), model.getHeight(), !model.getFullscreen(), D3DFMT_A8R8G8B8, &pDevice_);
	Errors::ErrorCheck(r, TEXT("Initialization of the device failed"));

	r = pDevice_->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer_);
	Errors::ErrorCheck(r, TEXT("Couldn't get backbuffer"));

	return S_OK;
}

/*
Summary:
	Release resources and shutdown the Direct3D device in preparation for termination, or a reset of the device.
Params: -
Return: -
*/
int Renderer::stopEngine() {
	if (pBackBuffer_) {
		pBackBuffer_->Release();
		pBackBuffer_ = NULL;
	}
	if (pDevice_) {
		pDevice_->Release();
		pDevice_ = NULL;
	}
	return S_OK;
}

/*
Summary:
	Resets the device to switch between fullscreen and windowed modes.  All Direct3D resources must be released before this function is called.
Params: 
	model: The program state model containing the new display information.
Return: -
*/
void Renderer::ChangeDisplayMode(const Model& model) {
	if (model.getFullscreen())
		OutputDebugString(TEXT("ChangeDisplayMode -- going to fullscreen\n"));
	else
		OutputDebugString(TEXT("ChangeDisplayMode -- going to windowed mode\n"));
	HRESULT r = 0;

	parameters.BackBufferWidth = model.getWidth();
	parameters.BackBufferHeight = model.getHeight();
	parameters.Windowed = !model.getFullscreen();
	parameters.PresentationInterval = !model.getFullscreen() ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;


	if (pBackBuffer_) {
		pBackBuffer_->Release();
		pBackBuffer_ = NULL;
	}

	r = pDevice_->Reset(&parameters);
	Errors::ErrorCheck(r, TEXT("ChangeDisplayMode failed on device reset!"));

	r = pDevice_->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer_);
	Errors::ErrorCheck(r, TEXT("ChangeDisplayMode couldn't reacquire the backbuffer"));
}

/*
Summary:
	Creates a new Direct3D Device based on Renderer::parameters
Params: 
	hWndTarget: Handle to the win32 window that will be rendered into
	Width: the width of the buffer in pixels
	Height: the height of the buffer in pixels
	bWindowed: true for windowed mode, false for fullscreen mode
	FullScreenFormat: The buffer format to use
	ppDevice: the output pointer representing the Direct3D device.
Return: S_OK on success
*/
int Renderer::InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3DDEVICE9* ppDevice) {
	HRESULT r = 0;

	if (*ppDevice)
		(*ppDevice)->Release();

	parameters.BackBufferWidth = Width;
	parameters.BackBufferHeight = Height;
	parameters.BackBufferFormat = FullScreenFormat;
	parameters.BackBufferCount = 1;
	parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;//D3DSWAPEFFECT_COPY;
	parameters.hDeviceWindow = hWndTarget;
	parameters.Windowed = bWindowed;
	parameters.EnableAutoDepthStencil = TRUE;
	parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	parameters.FullScreen_RefreshRateInHz = 0;//default refresh rate
	parameters.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	parameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	r = pD3D_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, ppDevice);
	Errors::ErrorCheck(r, TEXT("Could not create the render device"));

	return S_OK;
}


/*
Summary:
	Constructor: creates a renderer by creating a COM object representing Direct3D 9.
Params: -
Return: -
*/
Renderer::Renderer() {
	pD3D_ = Direct3DCreate9(D3D_SDK_VERSION);//COM object
	if (pD3D_ == NULL) {
		throw(TEXT("Could not create IDirect3D9 object"));
	}


	D3DXVECTOR3 pos = { 0, 10, 0 };
	D3DXVECTOR3 lookat = { 0, -1, 0 };
	D3DXVECTOR3 up = { 0, 0, 1 };
	D3DXMatrixLookAtLH(&tempView_, &pos, &lookat, &up);

	// D3DXPLANE {x, y, z, d} 
	// x, y, z are the normal of the plane
	// d is negative distance from origin
	// 0, 1, 0, 100 is a plane that passes through (0, -100, 0)
	D3DXPLANE plane = { 0, 1, 0, 0 }; 
	D3DXMatrixReflect(&tempXForm_, &plane);
	D3DXMATRIX tempXForm_;
}

/*
Summary:
	Destructor: releases the COM object for Direct3D 9.
Params: -
Return: -
*/
Renderer::~Renderer() {
	stopEngine();

	if (pD3D_) {
		pD3D_->Release();
		pD3D_ = NULL;
	}
}

/*
Summary:
	Accessor for the Direct3D device.
Params: -
Return: The active Direct3D device.
*/
LPDIRECT3DDEVICE9& Renderer::getDevice() {
	return pDevice_;
}

/*
Summary:
	Draws all background, 3d, and foreground objects in the program model.
Params: 
	model: the program model containing information about the scene to render.
Return: S_OK on success
*/
int Renderer::render(Model& model) {
	if (!pDevice_) {
		Errors::SetError(TEXT("Cannot render because there is no device"));
		return E_FAIL;
	}

	pDevice_->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, CLEAR_COLOR, 1.0f, 0);

	PreScene2D(model);

	pDevice_->BeginScene();
	Camera& cam = model.getCamera();
	pDevice_->SetTransform(D3DTS_VIEW, &cam.getViewMatrix());
	pDevice_->SetTransform(D3DTS_PROJECTION, &cam.getProjectionMatrix());
	Scene3D(model);

	// Enable stencil testing
	pDevice_->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice_->Clear(0, NULL, D3DCLEAR_STENCIL, 0xff000000, 1.0f, 0); //stencil buffer is now all zeroes.
	

	//Always pass. Keep the existing zero unless it passes.
	//In the event it passes, replace it with the reference value of 1.
	pDevice_->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	//pDevice_->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP); //not needed. default behaviour
	//pDevice_->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP); //not needed. default behaviour
	pDevice_->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice_->SetRenderState(D3DRS_STENCILREF, 1); //reference value for stencil tests
	pDevice_->SetRenderState(D3DRS_STENCILMASK, 1); //mask value for stencil test
	
	for (auto& obj : model.getMirror()) {
		obj->draw(pDevice_);
	}
	//The stencil is now a 0/1 mask of this cube.
	//For this next trick, do not modify the stencil buffer.
	//pDevice_->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP); //default behaviour
	//pDevice_->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP); //default behaviour
	pDevice_->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	pDevice_->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice_->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0); //Depthbuffer is now all reset.
	D3DXMatrixMultiply(&tempView_, &model.getCamera().getViewMatrix(), &tempXForm_);
	pDevice_->SetTransform(D3DTS_VIEW, &tempView_);
	//pDevice_->SetTransform(D3DTS_VIEW, &cam.getReflectedView(tempXForm_));
	Scene3D(model);
	pDevice_->SetRenderState(D3DRS_STENCILENABLE, FALSE);


	pDevice_->EndScene();

	PostScene2D(model);

	pDevice_->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
	return S_OK;
}

/*
Summary:
	Draws 2D background objects straight onto the back buffer
Params: 
	model: the program model containing information about the scene to render.
Return: -
*/
void Renderer::PreScene2D(Model& model) {
	//draw BG elements
	for (auto d : model.getBG()) {
		d->draw(pDevice_, pBackBuffer_);
	}
}

/*
Summary:
	Adds 3D objects to the render scene for rendering.
Params:
	model: the program model containing information about the scene to render.
Return: -
*/
void Renderer::Scene3D(Model& model) {
	for (auto& obj : model.get3D()) {
		obj->draw(pDevice_);
	}
}

/*
Summary:
	Draws 2D foreground objects straight onto the back buffer
Params:
	model: the program model containing information about the scene to render.
Return: -
*/
void Renderer::PostScene2D(Model& model) {
	//draw foreground elements
	for (auto d : model.getFG()) {
		d->draw(pDevice_, pBackBuffer_);
	}
}