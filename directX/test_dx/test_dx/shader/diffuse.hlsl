// File: diffuse.txt
// Desc: Vertex shader that does diffuse light ing.
// Global variables we use to hold the view matrix, projection matrix,
// ambient material, diffuse material, and the light vector that
// describes the direction to the light source. These variables are
// init ialized from the applicat ion.

matrix ViewMatrix : WorldView;
matrix ViewProjMatrix : WorldViewProjection;
vector AmbientMtrl;
vector DiffuseMtrl;
vector LightDirection;

vector DiffuseLightIntensity = {0.0f, 0.0f, 1.0f, 1.0f};
vector AmbientLightIntensity = {0.0f, 0.0f, 0.2f, 1.0f};

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

//Main
VS_OUTPUT ShaderMain( VS_INPUT input)
{

	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(input.position, ViewProjMatrix);

	LightDirection.w = 0.0f;
	input.normal.w = 0.0f;
	LightDirection = mul(LightDirection, ViewMatrix);
	input.normal = mul(input.normal, ViewMatrix);

	float s = dot(LightDirection, input.normal);
	if ( s < 0.0f )
		s = 0.0f;
	output.diffuse = (AmbientMtrl * AmbientLightIntensity) + ((DiffuseMtrl * DiffuseLightIntensity) * s);
	return output;
}