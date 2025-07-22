struct PushData {
    float2x2 transform;
    float2 offset;
    float3 color;
};

[[push_constant]]
PushData push;

struct FSOutput {
    [[vk::location(0)]] float4 outColor;
};

[shader("pixel")]
FSOutput main()
{
    FSOutput output;
    output.outColor = float4(push.color, 1.0);

    return output;
}
