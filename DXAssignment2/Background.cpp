#include "Background.h"

/*
Summary:
	Constructor.  Creates a new background object based on a file
Params: 
	path: the path to the bitmap file to use.
Return: -
*/
Background::Background(const TCHAR* path) : path_{ path } {}

/*
Summary:
	Destructor: Releases the direct3D resources.
Params: -
Return: -
*/
Background::~Background() {
	releaseResources();
}

/*
Summary:
	Draws the image onto the surface specified, overwriting all pixels.
Params: 
	pBackSurf: The backbuffer to draw onto.
Return: S_OK on success, E_FAIL on error.
*/
int Background::draw(LPDIRECT3DDEVICE9& device, LPDIRECT3DSURFACE9 pBackSurf) {
	HRESULT hr = device->UpdateSurface(background_, NULL, pBackSurf, NULL);
	if (FAILED(hr)) {
		Errors::SetError(TEXT("Could not copy surface"));
		return E_FAIL;
	}
	return S_OK;
}

/*
Summary:
	Load the image for this background to use. 
Params: 
	path: the file path to the image to use.
Return: S_OK on success, E_FAIL on error.
*/
int Background::LoadBackground(LPDIRECT3DDEVICE9& device, const TCHAR* path) {
	LPDIRECT3DSURFACE9 picture_full = 0;
	HRESULT r;

	LoadImageToSurface(device, path, picture_full);

	int w = 0;
	LPDIRECT3DSURFACE9 backBuffer;
	D3DSURFACE_DESC description;

	r = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	if (FAILED(r) || FAILED(backBuffer->GetDesc(&description))) {
		Errors::SetError(TEXT("Couldn't obtain backBuffer information"));
		return E_FAIL;
	}
	backBuffer->Release();
	backBuffer = 0;


	//Create an empty surface the size of the display device
	r = device->CreateOffscreenPlainSurface(description.Width, description.Height, description.Format, D3DPOOL_SYSTEMMEM, &background_, NULL);
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

/*
Summary:
	Loads an image from file and return it via the given surface. 
Params: 
	pathname: the path to the background file
	surface: Output parameter. Points to new surface of the image.
Return: S_OK on success, E_FAIL on error.
*/
int Background::LoadImageToSurface(LPDIRECT3DDEVICE9& device, const TCHAR* const pathname, LPDIRECT3DSURFACE9& surface) const {
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
	r = device->CreateOffscreenPlainSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, NULL);

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

/*
Summary:
	Load the background image file into memory.
Params:
	device: the DX device to load textures for
Return: -
*/
void Background::initializeResources(LPDIRECT3DDEVICE9& device) {
	LoadBackground(device, path_);
}

/*
Summary:
	Release the font image file from memory
Params: -
Return: -
*/
void Background::releaseResources() {
	if (background_) {
		background_->Release();
		background_ = 0;
	}
}