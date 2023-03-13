#pragma pack_matrix( row_major )

cbuffer DynConstBuffer : register(b0)
{
	matrix ViewProjMat;
}

float4 main( float3 pos : POSITION ) : SV_POSITION
{
	return mul(float4(pos, 1.0F), ViewProjMat);
}