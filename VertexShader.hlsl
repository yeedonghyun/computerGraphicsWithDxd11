float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}


//matrix lightworldToNDCMatrix;
//matrix inverseMatrix;
//
////ndc������ vertex�� ����
//PixelInputType depthVertexShader(VertexInputType input)
//{
//    PixelInputType output = (PixelInputType)0;
//
//    // ������ ��� ����� ���� ��ġ ���͸� 4 ������ �����մϴ�.
//    input.position.w = 1.0f;
//
//    // ����, �� �� ���� ��Ŀ� ���� ������ ��ġ�� ??����մϴ�.
//
//    output.position = mul(input.position, worldMatrix);
//    output.position = mul(output.position, viewMatrix);
//    output.position = mul(output.position, projectionMatrix);
//
//    // �ȼ� ���̴��� �ؽ�ó ��ǥ�� �����Ѵ�.
//    output.tex = input.tex;
//
//    // ���� ��Ŀ� ���ؼ��� ���� ���͸� ����մϴ�.
//    output.normal = mul(input.normal, (float3x3)worldMatrix);
//
//    // ���� ���͸� ����ȭ�մϴ�.
//    output.normal = normalize(output.normal);
//
//    return output;
//}
//
////ī�޶󿡼��� vertex�� LVP�� inverseMatrix�� ���ؼ� vertex�� ����
//PixelInputType shadowVertexShader(VertexInputType input)
//{
//    PixelInputType output = (PixelInputType)0;
//
//
//    // ������ ��� ����� ���� ��ġ ���͸� 4 ������ �����մϴ�.
//    input.position.w = 1.0f;
//
//    // ����, �� �� ���� ��Ŀ� ���� ������ ��ġ�� ??����մϴ�.
//    output.position = mul(input.position, worldMatrix);
//    output.position = mul(output.position, viewMatrix);
//    output.position = mul(output.position, projectionMatrix);
//    output.position = mul(output.position, NDCToWolrdMatrix);
//    output.position = mul(output.position, lightworldToNDCMatrix);
//
//    // �ȼ� ���̴��� �ؽ�ó ��ǥ�� �����Ѵ�.
//    output.tex = input.tex;
//
//    // ���� ��Ŀ� ���ؼ��� ���� ���͸� ����մϴ�.
//    output.normal = mul(input.normal, (float3x3)worldMatrix);
//
//    // ���� ���͸� ����ȭ�մϴ�.
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
//	// �� �ؽ�ó ��ǥ ��ġ���� ���÷��� ����Ͽ� �ؽ�ó���� �ȼ� ������ ���ø��մϴ�.
//	textureColor = shaderTexture.Sample(SampleType, input.tex);
//
//	// ����� ���� �� ������ ������ŵ�ϴ�.
//	lightDir = -lightDirection;
//
//	// �� �ȼ��� ���� ���� ����մϴ�.
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

