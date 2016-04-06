//=============================================================================
// transform.fx by Frank Luna (C) 2004 All Rights Reserved.
//
// Basic FX that simply transforms geometry from local space to 
// homogeneous clip space, and draws the geometry in wireframe mode.
//=============================================================================

uniform extern float4x4 gWorld;
uniform extern texture gTex;

sampler TexS = sampler_state {
	Texture = <gTex>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU = WRAP;
	AddressV = WRAP;
};

// Define a vertex shader output structure; that is, a structure
// that defines the data we output from the vertex shader.  Here,
// we only output a 4D vector in homogeneous clip space.  The
// semantic ": POSITION0" tells Direct3D that the data returned
// in this data member is a vertex position.
struct OutputVS {
	float4 posH : POSITION0;
	float2 tex0 : TEXCOORD0;
};

OutputVS TransformVS(float3 posL : POSITION0, float2 tex0 : TEXCOORD0) {
	// Zero out our output.
	OutputVS outVS = (OutputVS)0;
	outVS.posH.x = -3;
	outVS.posH.y = -2;
	outVS.posH.z = -1;
	outVS.tex0 = tex0;

	return outVS;
}

float4 TransformPS(float2 tex0 : TEXCOORD0) : COLOR
{
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

technique TransformTech {
	pass P0 {
		// Specify the vertex and pixel shader associated with this pass.
		vertexShader = compile vs_2_0 TransformVS();
		pixelShader = compile ps_2_0 TransformPS();
	}
}
