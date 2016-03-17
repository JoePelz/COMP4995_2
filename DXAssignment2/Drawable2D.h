#pragma once

#include <d3d9.h>

/* This class is a common interface for things that can be drawn in 2D, 
such as frame rate counters, background images, and lines. */
class Drawable2D {
public:
	virtual void initializeResources(LPDIRECT3DDEVICE9& device) = 0;
	/* Draw this 2D element onto the provided backBuffer. */
	virtual int draw(LPDIRECT3DDEVICE9& device, LPDIRECT3DSURFACE9 pBackSurf) { OutputDebugString(TEXT("Draw: Base implementation\n")); return S_OK; }
	virtual void releaseResources() = 0;

};

