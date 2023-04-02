cbuffer MatrixBuffer
{
	matrix worldMatrix;
    matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInput
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct PixelOutput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};
	
	
PixelOutput main( VertexInput input)
{
	PixelOutput output;

	input.position.w = 1.0f;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.texcoord = input.texcoord;
	
	return output;
}