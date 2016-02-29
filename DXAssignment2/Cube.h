#pragma once

#include "Drawable3D.h"
#include "Mesh.h"
#include "Errors.h"


class Cube :
	public Drawable3D{
	IDirect3DVertexBuffer9* vertexBuffer_;
	D3DMATERIAL9 material_;
public:
	void initializeResources(LPDIRECT3DDEVICE9& device) override;
	void releaseResources() override;
	void draw(LPDIRECT3DDEVICE9& device) override;
	Cube();
	~Cube();
};

