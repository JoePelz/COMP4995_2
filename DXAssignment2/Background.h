#pragma once
#include "Drawable2D.h"
#include "d3dx9.h"
#include "Errors.h"

/* This class holds an image to display on a surface via the Drawable2D interface. */
class Background : public Drawable2D {
	/* Surface to hold the image to draw. */
	LPDIRECT3DSURFACE9 background_;
	/* Reference to the device to allow surface copying. */
	const LPDIRECT3DDEVICE9& device_;

	/* Initializes the background_ to hold the image at the given path. Returns S_OK on success. */
	int LoadImageToSurface(const TCHAR* const pathname, LPDIRECT3DSURFACE9& surface) const;
public:
	/* ctor, saves reference to device. */
	Background(LPDIRECT3DDEVICE9& D3DDevice);
	~Background();
	/* copies the full image to the surface buffer provided. */
	virtual int draw(LPDIRECT3DSURFACE9 pBackSurf) override;
	/* Loads an image from the given path onto this instance's buffer. */
	int setImage(const TCHAR* path);
};

