#pragma once
#include "Drawable3D.h"
#include "Errors.h"

#define ColouredVertex_FLAGS D3DFVF_XYZ
#define ColouredVertex_STRIDE 12

struct ColouredVertex {
	float x, y, z;
};

#define TexturedVertex_FLAGS D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE1(1)
#define TexturedVertex_STRIDE 20

struct TexturedVertex {
	float x, y, z;
	float u, v;
};

class RectOverlay : public Drawable3D {
	IDirect3DVertexBuffer9* vertexBuffer_; //pointer to buffer within the Direct3D device.
	D3DMATERIAL9 mat_;
	LPDIRECT3DTEXTURE9 tex_;
public:
	void initializeResources(LPDIRECT3DDEVICE9& device) override;
	void releaseResources() override;
	void draw(LPDIRECT3DDEVICE9& device, const D3DXMATRIX* xform) override;
	void drawLite(LPDIRECT3DDEVICE9& device, const D3DXMATRIX* xform);
	void setupMaterial();
	void setTexture(LPDIRECT3DTEXTURE9 tex) { tex_ = tex; }
	
	RectOverlay();
	~RectOverlay();
};

