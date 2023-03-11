struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	uint InstanceId : SV_InstanceID;
};

struct Light
{
	float3 dir;
	float3 ambient;
	float3 diffuse;
};

Texture2D g_MeshTexture : register(t0);
SamplerState g_Sampler;

float4 main(PS_IN input) : SV_TARGET
{
	Light light;
	light.dir     = float3(0.0F, -1.0F, 0.0F);
	light.ambient = float3(0.5F, 0.5F, 0.5F);
	light.diffuse = float3(0.5F, 0.5F, 0.5F);
	input.normal = normalize(input.normal);

	float4 base = g_MeshTexture.Sample(g_Sampler, input.uv);
	
	float3 color = base.xyz * light.ambient;
	color += saturate(dot(-light.dir, input.normal) * light.diffuse * base.xyz);

	return float4(color, 1.0F);
}