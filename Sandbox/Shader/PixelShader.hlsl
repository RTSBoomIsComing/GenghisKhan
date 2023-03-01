struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	//float3 normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float dx = ddx(input.uv.x);
	float dy = ddy(input.uv.y);
	float x1 = input.uv.x;
	float y1 = input.uv.y;

	if (x1 < dx || x1 > 1.0F - dx || y1 < dy || y1 > 1.0F - dy)
	{
		return float4(0.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		return float4(0.0f, 0.0f, 1.0f, 0.1f);
	}
}