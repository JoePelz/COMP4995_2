#pragma once

#include <d3d9.h>
#include "Transform3D.h"
#include "MathUtilities.h"

/* This class is a common interface for things that can be drawn in 3D,
such as cubes, tigers, and airplanes. */

class Drawable3D : public Transform3D {
	bool bSelectable;
public:
	Drawable3D() : Transform3D(), bSelectable{ true } {}
	virtual void initializeResources(LPDIRECT3DDEVICE9& device) = 0;
	virtual void releaseResources() = 0;
	virtual void draw(LPDIRECT3DDEVICE9& device, const D3DXMATRIX* xform) = 0;

	virtual bool isSelectable() { return bSelectable; }
	virtual void setSelectable(bool selectable) { bSelectable = selectable; }
};

