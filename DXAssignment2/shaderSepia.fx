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


// Define the vertex shader program.  The parameter posL 
// corresponds to a data member in the vertex structure.
// Specifically, it corresponds to the data member in the 
// vertex structure with usage D3DDECLUSAGE_POSITION and 
// index 0 (as specified by the vertex declaration).
OutputVS TransformVS(float3 posL : POSITION0, float2 tex0 : TEXCOORD0) {
	// Zero out our output.
	OutputVS outVS = (OutputVS)0;

	// Transform to homogeneous clip space.
	//outVS.posH = float4(posL, 1.0f);
	outVS.posH = mul(float4(posL, 1.0f), gWorld);

	outVS.tex0 = tex0;

	// Done--return the output.
	return outVS;
}

// Define the pixel shader program.  Just return a 4D color
// vector (i.e., first component red, second component green,
// third component blue, fourth component alpha).  Here we
// specify black to color the lines black. 
float4 TransformPS(float2 tex0 : TEXCOORD0) : COLOR
{
	//sample the center of the texel, instead of the top left corner.
	float2 offset = float2(1.0f / 1280.0f, 1.0f / 960.0f);

	float3 color = tex2D(TexS, tex0 + offset);
	float3 outColor = color;
	outColor.r = (color.r * 0.393) + (color.g * 0.769) + (color.b * 0.189);
	outColor.g = (color.r * 0.349) + (color.g * 0.686) + (color.b * 0.168);
	outColor.b = (color.r * 0.272) + (color.g * 0.534) + (color.b * 0.131);

	return float4(outColor, 1.0f);
}

technique TransformTech {
	pass P0 {
		// Specify the vertex and pixel shader associated with this pass.
		vertexShader = compile vs_2_0 TransformVS();
		pixelShader = compile ps_2_0 TransformPS();
	}
}