struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	uint InstanceId : SV_InstanceID;
};


float4 main(PS_INPUT input) : SV_TARGET
{
	float4 color[4] = {{1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}};
	return color[input.InstanceId % 4];

}