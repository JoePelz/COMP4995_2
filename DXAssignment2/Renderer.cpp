#include "Renderer.h"

int Renderer::startEngine(HWND hwnd, Model& model) {
	HRESULT r = 0;//return values

	pD3D_ = Direct3DCreate9(D3D_SDK_VERSION);//COM object
	if (pD3D_ == NULL) {
		Errors::SetError(TEXT("Could not create IDirect3D9 object"));
		return E_FAIL;
	}

	//4th argument is TRUE or FALSE, where FALSE means fullscreen.
	r = InitDirect3DDevice(hwnd, model.getWidth(), model.getHeight(), WINDOWED_MODE, D3DFMT_X8R8G8B8, pD3D_, &pDevice_);
	if (FAILED(r)) {//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		Errors::SetError(TEXT("Initialization of the device failed"));
		return E_FAIL;
	}

	r = pDevice_->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer_);
	if (FAILED(r)) {
		Errors::SetError(TEXT("Couldn't get backbuffer"));
	}

	// Time to make this an UntransformedColouredVertex. As you can see, the points are
	// more abstract now, rather than being in screen coordinates, they're in world
	// coordinates.
	UntransformedColouredVertex vertices[] = {
		{ 0.0f, 1.0f, 0.0f, 0xffff0000 },
		{ 1.0f, -1.0f, 0.0f, 0xff00ff00 },
		{ -1.0f, -1.0f, 0.0f, 0xffffff00 }
	};

	// Nothing changes at all in how we make the vertex buffer. You can thank all that
	// abstract code we set up for that.
	r = pDevice_->CreateVertexBuffer(sizeof(vertices),
		D3DUSAGE_WRITEONLY, vertices[0].FORMAT, D3DPOOL_DEFAULT, &vertexBuffer_, NULL);
	if (FAILED(r)) {
		Errors::SetError(TEXT("initializeResources - Failed to CreateVertexBuffer!"));
	}

	// Move vertices into the buffer.
	void* bufferMemory;
	r = vertexBuffer_->Lock(0, sizeof(vertices), &bufferMemory, NULL);
	if (FAILED(r)) {
		Errors::SetError(TEXT("initializeResources - Failed to get a vertexBuffer lock!"));
	}
	memcpy(bufferMemory, vertices, sizeof(vertices));
	vertexBuffer_->Unlock();

	// Tell D3D what vertex format we're using, and to use our new buffer as the stream source.
	pDevice_->SetFVF(vertices[0].FORMAT);
	pDevice_->SetStreamSource(0, vertexBuffer_, 0, vertices[0].STRIDE_SIZE);
	pDevice_->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice_->SetRenderState(D3DRS_LIGHTING, FALSE);
	/*
	DRVERTEX DRVertices[] = {
		{ -2.0f,-2.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0), },
		{ 2.0f,-2.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 0), },
		{ 0.0f, 2.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255), },
	};


	r = pDevice_->CreateVertexBuffer(
		sizeof(struct DRVERTEX) * 3, 
		0, 
		DRVERTEX_FLAGS, 
		D3DPOOL_DEFAULT, 
		&vertexBuffer_, 
		NULL);

	if (FAILED(r)) {//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		Errors::SetError(TEXT("Could not create vertex buffer"));
		return E_FAIL;
	}

	//Pointer to vertex buffer
	BYTE* pVertices = 0;

	//Lock buffer
	//	r = g_pVB->Lock(0, sizeof(DRVertices), &pVertices,0);
	r = vertexBuffer_->Lock(0, sizeof(DRVertices), (void **)&pVertices, 0);

	if (FAILED(r)) {//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		Errors::SetError(TEXT("Could not lock buffer"));
		return E_FAIL;
	}

	// Turn off culling so the back of the triangle does not disapear
	pDevice_->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// Turn off lighting because we have are own colors for the vertices
	pDevice_->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Fill buffer
	CopyMemory(pVertices, &DRVertices, sizeof(DRVertices));

	//Unlock buffer
	vertexBuffer_->Unlock();
	*/

	return S_OK;
}

int Renderer::stopEngine() {
	if (vertexBuffer_) {
		vertexBuffer_->Release();
		vertexBuffer_ = 0;
	}

	if (pBackBuffer_) {
		pBackBuffer_->Release();//release lock
		pBackBuffer_ = 0;
	}
	if (pDevice_) {
		pDevice_->Release();
		pDevice_ = 0;
	}
	if (pD3D_) {
		pD3D_->Release();
		pD3D_ = 0;
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
	if (FAILED(r)) {
		Errors::SetError(TEXT("Could not get display adapter information"));
		return E_FAIL;
	}

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
	if (FAILED(r)) {
		Errors::SetError(TEXT("Could not create the render device"));
		return E_FAIL;
	}

	return S_OK;
}

Renderer::Renderer() : vertexBuffer_(0), pD3D_(0), pDevice_(0), pBackBuffer_(0) {
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

	pDevice_->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000044, 1.0f, 0);

	//PreScene2D(model);


	pDevice_->BeginScene();
	Scene3D(model);
	pDevice_->EndScene();

	//PostScene2D(model);

	pDevice_->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
	return S_OK;
}

void Renderer::PreScene2D(Model& model) {
	//clear the display area with colour almost-black, ignore stencil buffer
	//pDevice->Clear(0, 0, D3DCLEAR_TARGET, CLEAR_COLOR, 1.0f, 0);

	//draw BG elements
	for (auto d : model.getBG()) {
		d->draw(pBackBuffer_);
	}
}

void Renderer::Scene3D(Model& model) {

	//draw from stream
	pDevice_->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

}

void Renderer::PostScene2D(Model& model) {
	//draw foreground elements
	for (auto d : model.getFG()) {
		d->draw(pBackBuffer_);
	}
}