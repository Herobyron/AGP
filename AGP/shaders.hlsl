struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut Output;

	Output.position = position;
	Output.color = color;

	return Output;

}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}