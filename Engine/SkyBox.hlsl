#include "Common.hlsl"

VPS_INOUTPUT vsmain(VS_INPUT input)
{
	VPS_INOUTPUT output = (VPS_INOUTPUT) 0;
	
	output.position = mul(input.position, world);
	output.position = mul(output.position, camera.view);
	output.position = mul(output.position, camera.projection);
	output.texcoord = input.texcoord;
	
	return output;
}

Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

float4 psmain(VPS_INOUTPUT input) : SV_TARGET
{
	return Color.Sample(ColorSampler, input.texcoord);
}