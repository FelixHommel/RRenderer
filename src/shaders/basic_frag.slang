struct PushData {
    float2x2 transform;
    float2 offset;
    float3 color;
};

[[push_constant]]
PushData push;

struct FSInput {
    [[vk::location(0)]] float3 inColor; // NOTE: currently unused, maybe remove later
};

struct FSOutput {
    [[vk::location(0)]] float4 outColor;
};

[shader("pixel")]
FSOutput main(FSInput input)
{
    FSOutput output;
    output.outColor = float4(input.inColor, 1.0);
    output.outColor = float4(push.color, 1.0);

    return output;
}
