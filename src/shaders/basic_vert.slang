struct PushData {
    float2x2 transform;
    float2 offset;
    float3 color;
};

[[push_constant]]
PushData push;

struct VSInput {
    [[vk::location(0)]] float2 position;
    [[vk::location(1)]] float3 inColor;
};

struct VSOutput {
    float4 position : SV_Position;
    [[vk::location(0)]] float3 color;
};

[shader("vertex")]
VSOutput main(VSInput input)
{
    VSOutput output;
    float2 transformed = mul(push.transform, input.position) + push.offset;
    output.position = float4(transformed, 0.0, 1.0);
    output.color = input.inColor;

    return output;
}
