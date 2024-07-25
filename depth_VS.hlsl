
cbuffer MatrixBuffer 
{ 
	matrix  MVP;
};

struct vertexInput
{
	float3 position : POSITION;
};

struct vertexOutput
{
	float4 position : SV_POSITION;
	float4 positionNDC : TEXCOORD;
};

vertexOutput main(vertexInput input)
{
	vertexOutput res;

	res.position = float4(input.position, 1.f);

	res.position = mul(res.position, MVP);

	res.positionNDC = res.position;
	
	return res;
}