#pragma once

#include "Drawable3D.h"

class Light : public Drawable3D {
	static int si_light;
	int index;
	D3DLIGHT9 light;
public:
	Light(D3DLIGHTTYPE);
	~Light();
	inline void setDirection(const D3DXVECTOR3& dir) { D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &dir); }
	inline void setPosition(const D3DXVECTOR3& dir) { light.Position.x = dir.x; light.Position.y = dir.y; light.Position.z = dir.z; }
	inline void setPosition(float x, float y, float z) { light.Position.x = x; light.Position.y = y; light.Position.z = z; }
	inline void setColor(byte r, byte g, byte b) { light.Diffuse.a = 255; light.Diffuse.r = r; light.Diffuse.g = g; light.Diffuse.b = b; }

	void initializeResources(LPDIRECT3DDEVICE9& device) override;
	void releaseResources() override;
	virtual void draw(LPDIRECT3DDEVICE9& device) {};
};

