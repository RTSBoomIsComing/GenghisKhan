struct VS_INPUT
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	//float3 normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	//float3 normal : NORMAL;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}