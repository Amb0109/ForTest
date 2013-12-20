// File: diffuse.fx
// Desc: Vertex shader that does diffuse light ing.
// Global variables we use to hold the view matrix, projection matrix,
// ambient material, diffuse material, and the light vector that
// describes the direction to the light source. These variables are
// init ialized from the applicat ion.

float4x4 gWV	: WorldView;
float4x4 gWVP	: WorldViewProjection;

float4 AmbientMtrl;
float4 DiffuseMtrl;
float4 LightDirection;

float4 DiffuseLightIntensity = {0.0f, 0.0f, 1.0f, 1.0f};
float4 AmbientLightIntensity = {0.0f, 0.0f, 0.2f, 1.0f};

// Input and Output structures.
struct VS_INPUT
{
	vector position : POSITION;
	vector normal : NORMAL;
};
struct VS_OUTPUT
{
	vector position : POSITION;
	vector diffuse : COLOR;
};

// Vertex Shader Main Func
VS_OUTPUT VS_MAIN( VS_INPUT input)
{

	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(input.position, gWVP);

	LightDirection.w = 0.0f;
	input.normal.w = 0.0f;
	LightDirection = mul(LightDirection, gWV);
	input.normal = mul(input.normal, gWV);

	float s = dot(LightDirection, input.normal);
	if ( s < 0.0f )
		s = 0.0f;
	output.diffuse = (AmbientMtrl * AmbientLightIntensity) + ((DiffuseMtrl * DiffuseLightIntensity) * s);
	return output;
}

technique TechMain
{
	pass P0
    {
		// Specify the vertex and pixel shader associated with this pass.
		vertexShader = compile vs_2_0 VS_MAIN();
		//pixelShader  = compile ps_2_0 PS_MAIN();

		// Specify the render/device states associated with this pass.
		FillMode = Solid;
	}
}