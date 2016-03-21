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
	
	Scene3D(model, NULL);
	RenderMirrors(model);

	pDevice_->EndScene();

	PostScene2D(model);

	pDevice_->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
	return S_OK;
}

void Renderer::RenderMirrors(Model& model) {
	auto mirror = model.getMirror();
	const D3DXMATRIX* R;
	const D3DXPLANE* clip; //clip plane for reflections
	for (int face = 2; face < 3; face++) {
		//
		// Draw Mirror quad to stencil buffer ONLY.  In this way
		// only the stencil bits that correspond to the mirror will
		// be on.  Therefore, the reflected teapot can only be rendered
		// where the stencil bits are turned on, and thus on the mirror 
		// only.
		//
		pDevice_->Clear(0, 0, D3DCLEAR_STENCIL, 0, 0, 0);
		pDevice_->SetRenderState(D3DRS_STENCILENABLE, true);
		pDevice_->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		pDevice_->SetRenderState(D3DRS_STENCILREF, 1);
		pDevice_->SetRenderState(D3DRS_STENCILMASK, 1);
		pDevice_->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		pDevice_->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		pDevice_->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

		// disable writes to the depth and back buffers
		pDevice_->SetRenderState(D3DRS_ZWRITEENABLE, false);
		//pDevice_->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		//pDevice_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		//pDevice_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// draw the mirror to the stencil buffer
		mirror->setFace(face);
		R = &mirror->getFaceReflection();
		clip = &mirror->getFacePlane();
		mirror->draw(pDevice_, false, face);

		// re-enable depth and backbuffer writes
		pDevice_->SetRenderState(D3DRS_ZWRITEENABLE, true);
		//pDevice_->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

		// only draw reflected teapot to the pixels where the mirror was drawn to.
		pDevice_->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		pDevice_->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

		// clear depth buffer and blend the reflected teapot with the mirror
		pDevice_->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

		//reflect the lights
		for (auto& light : model.getLights()) {
			light->reflectLight(pDevice_, R);
		}

		// Finally, draw the reflected scene
		pDevice_->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		pDevice_->SetClipPlane(0, (float*)clip);
		pDevice_->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0); //enable clip plane
		Scene3D(model, R);

		//unreflect the lights
		for (auto& light : model.getLights()) {
			light->reflectLight(pDevice_, R);
		}
		pDevice_->SetRenderState(D3DRS_CLIPPLANEENABLE, 0); //disable clip plane again

		// Restore render states.
		pDevice_->SetRenderState(D3DRS_STENCILENABLE, false);
		pDevice_->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
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
void Renderer::Scene3D(Model& model, const D3DXMATRIX* xform) {
	for (auto& obj : model.get3D()) {
		obj->draw(pDevice_, xform);
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