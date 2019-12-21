cbuffer CBuffer0
{
	matrix WVPMatrix; // 64 bytes
	//float red_fraction; // 4 bytes
	float4 directional_light_vector; //  16 bytes
	float4 directional_light_colour; // 16 bytes
	float4 ambient_light_colour; // 16 bytes
}; //total size if 112

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

Texture2D texture0;
SamplerState sampler0;

VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut Output;

	//color.r *= red_fraction;
	// this isnt needed as we are using world view projection now to change positioning and scale
	//position.xy *= scale;

	float diffuse_amount = dot(directional_light_vector, normal);
	diffuse_amount = saturate(diffuse_amount);
	Output.color = ambient_light_colour + (directional_light_colour * diffuse_amount);

	Output.texcoord = texcoord;
	Output.position = mul(WVPMatrix, position);
	//Output.color = color;

	return Output;

}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return color * texture0.Sample(sampler0, texcoord);
}