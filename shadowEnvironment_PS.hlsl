
Texture2D shaderTexture : register(t0);
Texture2D sphereMapTexture : register(t1);
Texture2D depthMapTexture : register(t2);

SamplerState SampleType;

cbuffer shadowBuffer
{
	float4 diffuseColor;
	float3 lightPosition;

	matrix LVP;
	matrix NDCToWolrdMatrix;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 positionNDC : TEXCOOR0;

	float2 tex : TEXCOORD1;
	float4 RVector : TEXCOOR2;

	float3 normal : NORMAL;
};

float2 EnvironmentUV(float4 RVector) {
	float2 st;
	float z;

	z = sqrt(RVector.x * RVector.x + RVector.y * RVector.y + (RVector.z +1) * (RVector.z + 1));
	
	RVector.y *= -1;
	st.x = ((RVector.x / z) + 1) / 2;
	st.y = ((RVector.y / z) + 1) / 2;
	
	return st;
}

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
	float2 st;
	float lightIntensity;
	
	pos = lightPos(input.positionNDC);
	st = EnvironmentUV(input.RVector);

	textureColor = shaderTexture.Sample(SampleType, input.tex);
	depth = depthMapTexture.Sample(SampleType, pos.xy);
	sphereColor = sphereMapTexture.Sample(SampleType, st);

	lightVector = normalize(lightPosition - input.positionNDC.xyz);
	lightIntensity = saturate(dot(input.normal, lightVector));
	color = saturate(diffuseColor * lightIntensity);

	color = color * textureColor + sphereColor - 0.3;

	//return float4(input.RVector.xy, 0.f, 1.f);
	if (pos.z > depth.x + 0.002f) {
		return color * float4(0.2f, 0.2f, 0.2f, 1.f);
	}

	else {
		return color;
	}
}