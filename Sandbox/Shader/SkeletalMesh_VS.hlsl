const int MAX_INSTSANCES = 500;
const int MAX_BONES = 100;

cbuffer ConstantsForBlending : register(b0)
{
	matrix Bones[MAX_BONES];
}

cbuffer ConstantsForInstancing : register(b1)
{
	matrix Worlds[MAX_INSTSANCES];
	matrix ViewProjections[MAX_INSTSANCES];
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
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
	float4 accPosition = float4(0.0F, 0.0F, 0.0F, 1.0F); // position,  w = 1
	float4 accNormal = float4(0.0F, 0.0F, 0.0F, 0.0F);   // driection, w = 0
	for (int i = 0; i < 4; i++)
	{
		const uint boneIndex = input.blendIndices[i];
		if (boneIndex == -1) { break; }
		const matrix boneTransform = Bones[boneIndex];

		const float4 transformedPosition = mul(float4(input.pos, 0.0F), boneTransform);
		accPosition += input.blendWeights[i] * transformedPosition;

		const float4 transformedNormal = mul(float4(input.normal, 0.0F), boneTransform);
		accNormal += input.blendWeights[i] * transformedNormal;
	}

	const matrix world = Worlds[InstanceId];
	const matrix viewProjection = ViewProjections[InstanceId];
	output.pos		= mul(accPosition, mul(world, viewProjection)); // mul pos * W * V * P
	output.tex		= input.tex;
	output.normal	= mul(accNormal, world).xyz;
	output.InstanceId = InstanceId;
	return output;
}