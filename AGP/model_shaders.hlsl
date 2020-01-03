cbuffer CBM
{
	matrix WVPMatrix; //64 bytes
	float4 directional_light_vector;
	float4 directional_light_colour;
	float4 ambient_light_colour;
	float4 point_light_position;
}; // total 112

Texture2D texture0;
SamplerState sampler0;

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};


VOut ModelVS(float4 position : POSITION, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output;

	float4 default_color = { 1.0, 1.0, 1.0, 1.0 };
	
	//light stuff
	float diffuse_amount = dot(directional_light_vector, normal);
	diffuse_amount = saturate(diffuse_amount);
	float4 lightvector = point_light_position - position;
	float point_amount = dot(normalize(lightvector), normal);

	point_amount = saturate(point_amount);

	output.color = ambient_light_colour + (directional_light_colour * diffuse_amount);

	output.position = mul(WVPMatrix, position);
	output.texcoord = texcoord;
	//output.color = default_color;

	return output;
}

float4 ModelPS(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return texture0.Sample(sampler0, texcoord) * color;
}