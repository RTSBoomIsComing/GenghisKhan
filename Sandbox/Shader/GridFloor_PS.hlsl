cbuffer ConstantsOthers : register(b0)
{
	matrix InverseViewProjection; // hlsl does not provide function to invert a matrix
}

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
	// note : this uv range is (-1, 1), not (0, 1)
	// this uv is not used for texcoord 
	float4 near = float4(input.uv, 0.0F, 1.0F);		// (x, y, 0.0, 1.0)
	float4 far  = float4(input.uv, 1.0F, 1.0F);	// (x, y, 1.0, 1.0)

	// calculate near(start) point of ray 
	near = mul(near, InverseViewProjection);
	near = near / near.w;

	// calculate far(end) point of ray
	far = mul(far, InverseViewProjection);
	far = far / far.w;

	// equation, y = near.y + t * (far.y - near.y)
	// t is weight for interpolation
	// if y is 0, t = -near.y / (far.y - near.y)
	// the condition (y is 0) means that a point interpolated by t would be on a floor

	float t = -near.y / (far.y - near.y);
	float4 intersection = near + t * (far - near);
	
	float scale = 0.01F;
	float2 floorCoords = intersection.xz * scale;
	float2 derivative = fwidth(floorCoords);

	// talk bouat frac(x - 0.5F) - 0.5F,
	// frac function calculate x - floor(x),
	// if x < 0, e.g x = -1.4F, floor(-1.4F) = -2.0F, so frac(-1.4F) = -1.4F - (-2.0F) = 0.6F
	// that is not we expected, we want -0.4F for frac(-1.4F)
	// then frac(x - 0.5F) - 0.5F do what we want
	// if x = -1.4F, frac(-1.4F - 0.5F) = frac(-1.9F) = -1.9F - (-2.0F) = 0.1F
	// frac(-1.4F - 0.5F) - 0.5F = 0.1F - 0.5F = -0.4F, that is exactly we expected
	float2 grid = abs(frac(floorCoords - 0.5F) - 0.5F) / derivative;
	float gridLine = min(grid.x, grid.y);

	float4 color = float4(0.2, 0.2, 0.2, 1.0 - min(gridLine, 1.0));
	return color * float(t > 0.0F);
}