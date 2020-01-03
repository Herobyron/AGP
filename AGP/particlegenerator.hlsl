cbuffer CBP
{
	matrix WVPMatrix; //64 bytes
	float4 color;

}; // total 64

Texture2D texture0;
SamplerState sampler0;

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;

};


VOut ParticleVS(float4 position : POSITION)
{
	VOut output;

	output.position = mul(WVPMatrix, position);
	output.texcoord = position.xy;
	output.color = color;

	return output;
}

float4 ParticlePS(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	float4 distsq = texcoord.x * texcoord.x + texcoord.y * texcoord.y;
	clip(1.0f - distsq);
	return color;
}