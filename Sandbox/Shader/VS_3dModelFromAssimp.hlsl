cbuffer CBuffer : register(b0)
{
	matrix WorldViewProjMatrix[1000];
};

//struct Transform
//{
//	matrix WVPMat;
//};
//StructuredBuffer<Transform> transforms; //: register(t0);

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

VS_OUTPUT main(VS_INPUT Input, uint InstanceId : SV_InstanceID)
{
	VS_OUTPUT Output;
	Output.pos = mul(float4(Input.pos, 1.0f), WorldViewProjMatrix[InstanceId]);
	Output.tex = Input.tex;
	Output.normal = Input.normal;
	Output.InstanceId = InstanceId;
	//output.normal = (float3)mul(float4(input.normal, 0.0f), World);
	return Output;
}