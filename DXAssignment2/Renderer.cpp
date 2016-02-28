#include "Renderer.h"

int Renderer::startEngine(HWND hwnd, Model& model) {
	HRESULT r = 0;//return values

	//4th argument is TRUE or FALSE, where FALSE means fullscreen.
	r = InitDirect3DDevice(hwnd, model.getWidth(), model.getHeight(), !model.getFullscreen(), D3DFMT_X8R8G8B8, pD3D_, &pDevice_);
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
	if (pD3D_) {
		pD3D_->Release();
		pD3D_ = NULL;
	}
	return S_OK;
}

int Renderer::InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice) {
	D3DPRESENT_PARAMETERS d3dpp;//rendering info
	D3DDISPLAYMODE d3ddm;//current display mode info
	HRESULT r = 0;

	if (*ppDevice)
		(*ppDevice)->Release();

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	r = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	Errors::ErrorCheck(r, TEXT("Could not get display adapter information"));

	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = bWindowed ? d3ddm.Format : FullScreenFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//D3DSWAPEFFECT_COPY;
	d3dpp.hDeviceWindow = hWndTarget;
	d3dpp.Windowed = bWindowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = 0;//default refresh rate
	d3dpp.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	r = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, ppDevice);
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
}

//Accessor for the render device
LPDIRECT3DDEVICE9& Renderer::getDevice() {
	return pDevice_;
}

int Renderer::render(Model & model) {
	if (!pDevice_) {
		Errors::SetError(TEXT("Cannot render because there is no device"));
		return E_FAIL;
	}

	pDevice_->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, CLEAR_COLOR, 1.0f, 0);

	//PreScene2D(model);

	pDevice_->BeginScene();
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
	Camera& cam = model.getCamera();
	pDevice_->SetTransform(D3DTS_VIEW, &cam.getViewMatrix());
	pDevice_->SetTransform(D3DTS_PROJECTION, &cam.getProjectionMatrix());

	pDevice_->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

}

void Renderer::PostScene2D(Model& model) {
	//draw foreground elements
	for (auto d : model.getFG()) {
		d->draw(pBackBuffer_);
	}
}