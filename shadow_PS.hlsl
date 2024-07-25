
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState SampleType;

cbuffer shadowBuffer
{
	float4 diffuseColor;
	float3 lightDirection;

	matrix LVP;
	matrix NDCToWolrdMatrix;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 positionNDC : TEXCOOR0;

	float2 tex : TEXCOORD1;
	float3 normal : NORMAL;
};

float4 lightPos(float4 position) {
	float4 pos = position;

	pos = mul(pos, NDCToWolrdMatrix);
	pos = mul(pos, LVP);
	pos /= pos.w;
	pos.y *= -1;
	pos.xy += float2(1, 1);
	pos.xy /= 2;

	return pos;
}

float4 main(PixelInput input) : SV_TARGET
{
	float4 textureColor, depth, sphereColor, pos, color;
	float3 lightVector;
	float lightIntensity;

	pos = lightPos(input.positionNDC);

	textureColor = shaderTexture.Sample(SampleType, input.tex);
	depth = depthMapTexture.Sample(SampleType, pos.xy);

	lightVector = normalize(lightDirection);

	lightIntensity = saturate(dot(input.normal, lightVector));
	color = saturate(diffuseColor * lightIntensity);

	color = color * textureColor;

	if (pos.z > depth.x + 0.002f) {
		return float4(0.f, 0.f, 0.f, 1.f);
	}

	else {
		return color;
	}
}