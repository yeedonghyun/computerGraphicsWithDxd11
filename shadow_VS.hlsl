
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix MVP;
};

struct vertexInput
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct pixelInput
{ 
	float4 position : SV_POSITION;
	float4 positionNDC : TEXCOOR0;

	float2 tex : TEXCOORD1;
	float3 normal : NORMAL;
};

pixelInput main(vertexInput input)
{
	pixelInput output;

    output.position = float4(input.position, 1.f);

    output.position = mul(output.position, MVP);
	output.positionNDC = output.position;

    output.tex = input.tex;

    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

    return output;
}