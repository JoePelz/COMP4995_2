#pragma once
#include "Drawable2D.h"
#include "d3dx9.h"
#include "Errors.h"

/* This class holds an image to display on a surface via the Drawable2D interface. */

class Background : public Drawable2D {
	/* Surface to hold the image to draw. */
	LPDIRECT3DSURFACE9 background_;
	/* Path to background image file */
	const TCHAR* path_;

	/* Initializes the background_ to hold the image at the given path. Returns S_OK on success. */
	int LoadImageToSurface(LPDIRECT3DDEVICE9& device, const TCHAR* const pathname, LPDIRECT3DSURFACE9& surface) const;
	/* Loads an image from the given path onto this instance's buffer. */
	int LoadBackground(LPDIRECT3DDEVICE9& device, const TCHAR* path);
public:
	/* ctor, saves reference to device. */
	Background(const TCHAR* path);
	~Background();
	/* copies the full image to the surface buffer provided. */
	virtual int draw(LPDIRECT3DDEVICE9& device, LPDIRECT3DSURFACE9 pBackSurf) override;
	/* Set the path to the image to use. Requires release/initialize resources to take effect. */
	inline void setPath(const TCHAR* path) { path_ = path; }

	virtual void initializeResources(LPDIRECT3DDEVICE9& device);
	virtual void releaseResources();
};

