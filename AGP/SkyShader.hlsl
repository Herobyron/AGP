cbuffer ConstantBufferSky
{
	matrix WVPMatrix; // 64 bytes
};


struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};


TextureCube cube0;
SamplerState sampler0;


VOut SkyVS(float4 position : POSITION, float4 color : COLOR, float3 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output;

	output.position = mul(WVPMatrix, position);

	output.color = color;

	output.texcoord = position.xyz;

	output.normal = normal;
	output.normal = normalize(normal);

	return output;
}

float4 SkyPS(float4 position : SV_POSITION, float4 color : COLOR, float texcoord : TEXCOORD) : SV_TARGET
{
	return color * cube0.Sample(sampler0, texcoord);
}