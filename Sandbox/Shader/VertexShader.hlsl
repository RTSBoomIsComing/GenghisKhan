struct VS_INPUT
{
	float3 pos : POSITION;
	//float2 uv : TEXCOORD;
	//float3 normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	//float3 normal : NORMAL;
	//float2 uv : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = float4(input.pos, 1.0f);
	//output.pos = mul(float4(input.pos, 1.0f), WVP);
	//output.normal = (float3)mul(float4(input.normal, 0.0f), World);
	//output.uv = input.uv;
	return output;
}