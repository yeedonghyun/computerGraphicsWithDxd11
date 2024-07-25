
cbuffer EnvironmentBufferType
{
	float3 cameraPosition;
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
	float4 RVector : TEXCOOR2;

	float3 normal : NORMAL;
};

float4 RVector(float4 normal, float4 v) {
	float cos;
	float4 s, r;

	cos = dot(normal, v);
	s = normal * cos;
	r = (s * 2) - v;

	return r;
}

pixelInput main(vertexInput input)
{
	float4 v, e, wrdNormal, wrdVertex;
	pixelInput output;

	wrdVertex = float4(input.position, 1.f);
	wrdVertex = mul(wrdVertex, worldMatrix);

	output.position = float4(input.position, 1.f);

	output.position = mul(output.position, MVP);
	output.positionNDC = output.position;

    output.tex = input.tex;

	wrdNormal = float4(input.normal, 1.f);
	wrdNormal = mul(wrdNormal, worldMatrix);
	wrdNormal = normalize(wrdNormal);
	output.normal = wrdNormal.xyz;

	e = float4(cameraPosition, 1.f);
	v = normalize(e - wrdVertex);

	output.RVector = RVector(wrdNormal, v);

    return output;
}
