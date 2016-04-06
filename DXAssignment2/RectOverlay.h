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
	static const TCHAR * const shaders_[];
	int shaderIndex_;
	IDirect3DVertexBuffer9* vertexBuffer_; //pointer to buffer within the Direct3D device.
	D3DMATERIAL9 mat_;
	IDirect3DTexture9* tex_;
	IDirect3DSurface9* texSurface_;
	IDirect3DVertexDeclaration9* vertDecl_;
	LPD3DXEFFECT shader_;
	D3DXHANDLE shTechnique_;
	D3DXHANDLE shWorld_;
	D3DXHANDLE shTex_;

	void initVertices(IDirect3DDevice9* device);
	void initTexture(IDirect3DDevice9* device);
	void initPixelShader(IDirect3DDevice9* device);
public:
	void initializeResources(LPDIRECT3DDEVICE9& device) override;
	void releaseResources() override;
	void draw(LPDIRECT3DDEVICE9& device, const D3DXMATRIX* xform) override;
	void setTexture(LPDIRECT3DTEXTURE9 tex) { tex_ = tex; }
	void updateTexture(IDirect3DDevice9* device, IDirect3DSurface9* backBuffer);
	void cycleShader(IDirect3DDevice9* device);
	
	RectOverlay();
	~RectOverlay();
};

