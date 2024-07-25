
struct PixelInput
{
	float4 position : SV_POSITION;
	float4 positionNDC : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{
	float depth;
	float4 color;

	depth = input.positionNDC.z / input.positionNDC.w;

	color = float4(depth, depth, depth, 1);

	return color;
}