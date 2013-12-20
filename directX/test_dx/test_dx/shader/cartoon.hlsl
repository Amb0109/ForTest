// File: cartoon.hlsl
// Desc: Vertex shader that lights geometry so it appears to be
// drawn in a cartoon style.

// Globals
extern matrix g_world_view_matrix;
extern matrix g_world_view_proj_matrix;
extern vector g_color;
extern vector g_light_direction;
static vector COLOR_BLACK = {0.0f, 0.0f, 0.0f, 0.0f};


struct VetrixShaderInput
{
	vector position	: POSITION;
	vector normal	: NORMAL;
};

struct VetrixShaderOutput
{
	vector position		: POSITION;
	float2 uv_coords	: TEXCOORD;
	vector diffuse		: COLOR;
};

VetrixShaderOutput ShaderMain(VetrixShaderInput input)
{
	VetrixShaderOutput output = (VetrixShaderOutput)0;

	// transform vertex position to homogenous clip space
	output.position = mul(input.position, g_world_view_proj_matrix);
	// Transform lights and normals to view space. Set w
	// components to zero since we're transforming vectors.
	// Assume there are no scalings in the world
	// matrix as well.
	g_light_direction.w = 0.0f;
	g_light_direction = mul(g_light_direction, g_world_view_matrix);
	input.normal.w = 0.0f;
	input.normal = mul(input.normal, g_world_view_matrix);

	// Compute the 1D texture coordinate for toon rendering.
	float u = dot(g_light_direction, input.normal);

	// Clamp to zero if u is negative because u
	// negative implies the angle between the light
	// and normal is greater than 90 degrees. And
	// if that is true then the surface receives no light.
	if (u < 0.0f)
		u = 0.0f;

	// Set other tex coord to middle.
	float v = 0.5f;
	output.uv_coords.x = u;
	output.uv_coords.y = v;

	// save color
	output.diffuse = g_color;

	return output;
}