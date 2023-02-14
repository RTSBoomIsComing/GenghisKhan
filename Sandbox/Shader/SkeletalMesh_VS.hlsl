static const int MAX_INSTSANCES = 1000;
static const int MAX_BONES = 1000;

cbuffer ConstantsForInstancing : register(b0)
{
	matrix Worlds[MAX_INSTSANCES];
};

cbuffer ConstantsOther : register(b1)
{
	matrix ViewProjection;
}

cbuffer ConstantsForBlending : register(b2)
{
	matrix Bones[MAX_BONES];
}

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 tex : TEXCOORD;
	float3 normal : NORMAL;
	uint4  blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	uint InstanceId : SV_InstanceID;
};

VS_OUTPUT main(VS_INPUT input, uint vertexID : SV_VertexID, uint InstanceId : SV_InstanceID)
{
	VS_OUTPUT output;
	float3 accPosition = float3(0.0F, 0.0F, 0.0F); // position,  w = 1
	float3 accNormal = float3(0.0F, 0.0F, 0.0F);   // driection, w = 0
	int i = 0;
	for (; i < 4; i++)
	{
		const uint boneIndex = input.blendIndices[i];
		if (boneIndex >= MAX_BONES) { break; }
		const matrix boneTransform = Bones[boneIndex];

		const float4 localPosition = mul(float4(input.pos, 1.0F), boneTransform);
		accPosition += (input.blendWeights[i] * localPosition).xyz;

		const float4 localNormal = mul(float4(input.normal, 0.0F), boneTransform);
		accNormal += (input.blendWeights[i] * localNormal).xyz;
	}

	// when disable bone transform
	//accPosition = float3(input.pos);
	//accNormal = float3(input.normal);


	if (i == 0)
	{
		accPosition = input.pos;
		accNormal   = input.normal;
	}

	const matrix World = Worlds[InstanceId];

	output.pos		  = mul(float4(accPosition, 1.0F), mul(World, ViewProjection)); // mul pos * W * V * P
	output.tex		  = input.tex.xy;
	output.normal     = mul(float4(normalize(accNormal), 0.0F), World).xyz;
	output.InstanceId = InstanceId;
	return output;
}