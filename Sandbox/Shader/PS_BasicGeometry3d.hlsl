struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{

	return float4(1.0f, 1.0f, 1.0f, 1.0f);

}