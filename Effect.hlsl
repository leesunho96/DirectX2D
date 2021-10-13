struct VertexInput
{
    float4 Position : POSITION0;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.Position = input.Position;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return float4(0, 0, 0, 1);
}
