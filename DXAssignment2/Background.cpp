#include "Background.h"

Background::Background(LPDIRECT3DDEVICE9& D3DDevice) 
	: device_(D3DDevice) {

}

Background::~Background() {
	background_->Release();
}

int Background::draw(LPDIRECT3DSURFACE9 pBackSurf) {
	HRESULT hr = device_->UpdateSurface(background_, NULL, pBackSurf, NULL);
	if (FAILED(hr)) {
		Errors::SetError(TEXT("Could not copy surface"));
		return E_FAIL;
	}
	return S_OK;
}

int Background::setImage(const TCHAR* path) {
	LPDIRECT3DSURFACE9 picture_full = 0;
	HRESULT r;

	LoadImageToSurface(path, picture_full);

	int w = 0;
	LPDIRECT3DSURFACE9 backBuffer;
	D3DSURFACE_DESC description;

	r = device_->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	if (FAILED(r) || FAILED(backBuffer->GetDesc(&description))) {
		Errors::SetError(TEXT("Couldn't obtain backBuffer information"));
		return E_FAIL;
	}
	backBuffer->Release();
	backBuffer = 0;


	//Create an empty surface the size of the display device
	r = device_->CreateOffscreenPlainSurface(description.Width, description.Height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &background_, NULL);
	if (FAILED(r)) {
		Errors::SetError(TEXT("couldn't create empty surface."));
		return E_FAIL;
	}

	//Copy-scale the image onto the empty surface
	r = D3DXLoadSurfaceFromSurface(background_, NULL, NULL, picture_full, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
	if (FAILED(r)) {
		Errors::SetError(TEXT("did not copy surface"));
		return E_FAIL;
	}

	picture_full->Release();

	return S_OK;
}

int Background::LoadImageToSurface(const TCHAR* const pathname, LPDIRECT3DSURFACE9& surface) const {
	HRESULT r;
	HBITMAP hBitmap;
	BITMAP Bitmap;

	hBitmap = (HBITMAP)LoadImage(NULL, pathname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap == NULL) {
		Errors::SetError(TEXT("Unable to load bitmap"));
		return E_FAIL;
	}

	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	DeleteObject(hBitmap);//we only needed it for the header info to create a D3D surface

	//create surface for bitmap
	//r=device_->CreateOffscreenPlainSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_X8R8G8B8, D3DPOOL_SCRATCH,   &surface, NULL);
	r = device_->CreateOffscreenPlainSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, NULL);

	if (FAILED(r)) {
		Errors::SetError(TEXT("Unable to create surface for bitmap load"));
		return E_FAIL;
	}
	//load bitmap onto surface
	r = D3DXLoadSurfaceFromFile(surface, NULL, NULL, pathname, NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(r)) {
		Errors::SetError(TEXT("Unable to load file to surface"));
		return E_FAIL;
	}

	return S_OK;
}
