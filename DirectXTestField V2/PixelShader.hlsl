Texture2D textureFile;
sampler SamplerType : register(s0);

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{
	//return textureFile.Sample(SamplerType, input.texcoord);
    return float4(1, 1, 1, 1);
}