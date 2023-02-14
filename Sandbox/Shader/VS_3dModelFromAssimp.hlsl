cbuffer CBuffer : register(b0)
{
	matrix WorldViewProjections[1000];
};

// this code enable structured buffer, if you do not want to use cbuffer
//struct Transform
//{
//	matrix WVPMat;
//};
//StructuredBuffer<Transform> transforms : register(t0);

struct VS_INPUT
{
	float3 pos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
	uint InstanceId : SV_InstanceID;
};

VS_OUTPUT main(VS_INPUT input, uint InstanceId : SV_InstanceID)
{
	VS_OUTPUT output;
	output.pos = mul(float4(input.pos, 1.0f), WorldViewProjections[InstanceId]);
	output.tex = input.tex;
	output.normal = input.normal;
	output.InstanceId = InstanceId;
	//output.normal = mul(float4(input.normal, 0.0f), World).xyz;
	return output;
}