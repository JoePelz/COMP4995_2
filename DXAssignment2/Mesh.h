#pragma once

#include <Windows.h>
#include <d3d9.h>

// Logically, we'll call our new untransformed vertex format UntransformedColouredVertex. An
// Untransformed vertex only holds a logical x, y, z point in space. It does not hold rhw.
struct UntransformedColouredVertex {
	float x, y, z;
	DWORD colour;

	// The format is ever so slightly different, and the stride size will be just 16 in
	// this format.
	static const DWORD FORMAT = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	static const int STRIDE_SIZE = 16;
};

// This is a Transformed vertex with a Diffuse colour component.
struct TransformedColouredVertex {
	float x, y, z, rhw;
	DWORD colour;

	static const DWORD FORMAT = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
	static const int STRIDE_SIZE = 20;
};

class Mesh {
public:
	Mesh();
	~Mesh();
};

