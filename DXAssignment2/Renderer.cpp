#include "Renderer.h"

int Renderer::startEngine(HWND hwnd, const Model& model) {
	HRESULT r = 0;//return values

	//4th argument is TRUE or FALSE, where FALSE means fullscreen.
	r = InitDirect3DDevice(hwnd, model.getWidth(), model.getHeight(), !model.getFullscreen(), D3DFMT_X8R8G8B8, &pDevice_);
	Errors::ErrorCheck(r, TEXT("Initialization of the device failed"));

	r = pDevice_->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer_);
	Errors::ErrorCheck(r, TEXT("Couldn't get backbuffer"));

	return S_OK;
}

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
	parameters.AutoDepthStencilFormat = D3DFMT_D16;
	parameters.FullScreen_RefreshRateInHz = 0;//default refresh rate
	parameters.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	parameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	r = pD3D_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, ppDevice);
	Errors::ErrorCheck(r, TEXT("Could not create the render device"));

	return S_OK;
}

Renderer::Renderer() {
	pD3D_ = Direct3DCreate9(D3D_SDK_VERSION);//COM object
	if (pD3D_ == NULL) {
		throw(TEXT("Could not create IDirect3D9 object"));
	}
}

Renderer::~Renderer() {
	stopEngine();

	if (pD3D_) {
		pD3D_->Release();
		pD3D_ = NULL;
	}
}

//Accessor for the render device
LPDIRECT3DDEVICE9& Renderer::getDevice() {
	return pDevice_;
}

int Renderer::render(Model& model) {
	if (!pDevice_) {
		Errors::SetError(TEXT("Cannot render because there is no device"));
		return E_FAIL;
	}

	pDevice_->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, CLEAR_COLOR, 1.0f, 0);

	PreScene2D(model);

	pDevice_->BeginScene();
	const Camera& cam = model.getCamera();
	pDevice_->SetTransform(D3DTS_VIEW, &cam.getViewMatrix());
	pDevice_->SetTransform(D3DTS_PROJECTION, &cam.getProjectionMatrix());
	Scene3D(model);
	pDevice_->EndScene();

	PostScene2D(model);

	pDevice_->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
	return S_OK;
}

void Renderer::PreScene2D(Model& model) {
	//draw BG elements
	for (auto d : model.getBG()) {
		d->draw(pBackBuffer_);
	}
}

void Renderer::Scene3D(Model& model) {
	for (auto& obj : model.get3D()) {
		obj->draw(pDevice_);
	}
}

void Renderer::PostScene2D(Model& model) {
	//draw foreground elements
	for (auto d : model.getFG()) {
		d->draw(pBackBuffer_);
	}
}