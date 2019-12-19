cbuffer CBuffer0
{
	matrix WVPMatrix; // 64 bytes
	float red_fraction; // 4 bytes
	float scale; // 4 bytes
	float2 packing; // 2x4 bytes = 8 bytes
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut Output;

	color.r *= red_fraction;

	// this isnt needed as we are using world view projection now to change positioning and scale
	//position.xy *= scale;
	
	Output.position = mul(WVPMatrix, position);
	Output.color = color;

	return Output;

}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}