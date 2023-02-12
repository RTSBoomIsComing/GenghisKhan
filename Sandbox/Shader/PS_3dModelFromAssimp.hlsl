struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	uint InstanceId : SV_InstanceID;
};

struct Light
{
	float3 dir;
	float3 ambient;
	float3 diffuse;
};

//cbuffer CBuffer
//{
//	Light light;
//};

Texture2D g_MeshTexture : register(t0);
SamplerState g_Sampler;

float4 main(VS_OUTPUT Input) : SV_TARGET
{
	//Light light;
	//light.dir     = float3(0.0F, -1.0F, 0.0F);
	//light.ambient = float3(0.5F, 0.5F, 0.5);
	//light.diffuse = float3(0.5F, 0.5F, 0.5);
	//Input.normal = normalize(Input.normal);

	float4 base = g_MeshTexture.Sample(g_Sampler, Input.uv);
	//clip(base.a - .25);

	//float3 color = base * light.ambient;
	//color += saturate(/*dot(-light.dir, input.normal) * */light.diffuse * base);

	//return float4(color, base.a);
	return base;
}