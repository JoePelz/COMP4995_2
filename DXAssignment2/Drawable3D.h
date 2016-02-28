#pragma once

#include <d3d9.h>

class Drawable3D {
public:
	virtual void initializeResources(LPDIRECT3DDEVICE9& device) = 0;
	virtual void releaseResources() = 0;
	virtual void draw(LPDIRECT3DDEVICE9& device) = 0;
};

