cbuffer CBuffer
{
	float4 size;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	//float3 normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 color = float4(0.0f, 1.0f, 0.0f, 0.0f);
	//float dx = 2.0f / size.x;
	//float dy = 2.0f / size.y;
	//color.x = (input.uv.x < dx || input.uv.x > 1.0f - dx || input.uv.y < dy || input.uv.y > 1.0f - dy) ? 1.0f : 0.0f;
	return color;
}