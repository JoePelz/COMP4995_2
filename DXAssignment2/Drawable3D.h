#pragma once

#include <d3d9.h>
#include "ITransform.h"

/* This class is a common interface for things that can be drawn in 3D,
such as cubes, tigers, and airplanes. */

class Drawable3D : public Transform3D {
public:
	virtual void initializeResources(LPDIRECT3DDEVICE9& device) = 0;
	virtual void releaseResources() = 0;
	virtual void draw(LPDIRECT3DDEVICE9& device) = 0;
};

