float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}


//matrix lightworldToNDCMatrix;
//matrix inverseMatrix;
//
////ndc공간의 vertex를 리턴
//PixelInputType depthVertexShader(VertexInputType input)
//{
//    PixelInputType output = (PixelInputType)0;
//
//    // 적절한 행렬 계산을 위해 위치 벡터를 4 단위로 변경합니다.
//    input.position.w = 1.0f;
//
//    // 월드, 뷰 및 투영 행렬에 대한 정점의 위치를 ??계산합니다.
//
//    output.position = mul(input.position, worldMatrix);
//    output.position = mul(output.position, viewMatrix);
//    output.position = mul(output.position, projectionMatrix);
//
//    // 픽셀 쉐이더의 텍스처 좌표를 저장한다.
//    output.tex = input.tex;
//
//    // 월드 행렬에 대해서만 법선 벡터를 계산합니다.
//    output.normal = mul(input.normal, (float3x3)worldMatrix);
//
//    // 법선 벡터를 정규화합니다.
//    output.normal = normalize(output.normal);
//
//    return output;
//}
//
////카메라에서본 vertex에 LVP와 inverseMatrix를 곱해서 vertex를 리턴
//PixelInputType shadowVertexShader(VertexInputType input)
//{
//    PixelInputType output = (PixelInputType)0;
//
//
//    // 적절한 행렬 계산을 위해 위치 벡터를 4 단위로 변경합니다.
//    input.position.w = 1.0f;
//
//    // 월드, 뷰 및 투영 행렬에 대한 정점의 위치를 ??계산합니다.
//    output.position = mul(input.position, worldMatrix);
//    output.position = mul(output.position, viewMatrix);
//    output.position = mul(output.position, projectionMatrix);
//    output.position = mul(output.position, NDCToWolrdMatrix);
//    output.position = mul(output.position, lightworldToNDCMatrix);
//
//    // 픽셀 쉐이더의 텍스처 좌표를 저장한다.
//    output.tex = input.tex;
//
//    // 월드 행렬에 대해서만 법선 벡터를 계산합니다.
//    output.normal = mul(input.normal, (float3x3)worldMatrix);
//
//    // 법선 벡터를 정규화합니다.
//    output.normal = normalize(output.normal);
//
//    return output;
//}



//float4 LightPixelShader(PixelInputType input) : SV_TARGET
//{
//	float4 textureColor;
//	float3 lightDir;
//	float lightIntensity;
//	float4 color;
//
//	// 이 텍스처 좌표 위치에서 샘플러를 사용하여 텍스처에서 픽셀 색상을 샘플링합니다.
//	textureColor = shaderTexture.Sample(SampleType, input.tex);
//
//	// 계산을 위해 빛 방향을 반전시킵니다.
//	lightDir = -lightDirection;
//
//	// 이 픽셀의 빛의 양을 계산합니다.
//	lightIntensity = saturate(dot(input.normal, lightDir));
//
//	// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
//	color = saturate(diffuseColor * lightIntensity);
//
//	// Multiply the texture pixel and the input color to get the final result.
//	color = color * textureColor;
//
//	return color;
//}

