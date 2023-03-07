struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;
	output.pos = float4(input.pos.xy, 0.999F, 1.0F);
	output.uv = input.uv;

	return output;
}