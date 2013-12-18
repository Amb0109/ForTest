

matrix g_view_proj_matrix : WorldViewProjection;

vector COLOR_BLUE = { 0.0f, 0.0f, 1.0f, 1.0f};

struct VetrixShaderInput
{
	vector position : POSITION;
};

struct VetrixShaderOutput
{
	vector position	: POSITION;
	vector diffuse	: COLOR;
};

VetrixShaderOutput ShaderMain(VetrixShaderInput input)
{
	VetrixShaderOutput output = (VetrixShaderOutput)0;
	output.position	= mul(input.position, g_view_proj_matrix);
	output.diffuse	= COLOR_BLUE;
	return output;
}