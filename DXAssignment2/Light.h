#pragma once

#include "Drawable3D.h"

/*
The Light class represents a source of illumination in a scene.
*/

class Light : public Drawable3D {
	static int si_light; //The next available index in the Direct3D device
	int index; //The index of a specific light instance
	D3DLIGHT9 light; //A struct containing lighting information
public:
	Light(D3DLIGHTTYPE);
	inline int getIndex() { return index; }
	inline void setDirection(const D3DXVECTOR3& dir) { D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &dir); }
	inline void setPosition(const D3DXVECTOR3& dir) { light.Position.x = dir.x; light.Position.y = dir.y; light.Position.z = dir.z; }
	inline void setPosition(float x, float y, float z) { light.Position.x = x; light.Position.y = y; light.Position.z = z; }
	inline void setColor(byte r, byte g, byte b) { light.Diffuse.a = 255; light.Diffuse.r = r; light.Diffuse.g = g; light.Diffuse.b = b; }

	void initializeResources(LPDIRECT3DDEVICE9& device) override;
	void releaseResources() override;
	virtual void draw(LPDIRECT3DDEVICE9& device, D3DXMATRIX* xform) {};
};

