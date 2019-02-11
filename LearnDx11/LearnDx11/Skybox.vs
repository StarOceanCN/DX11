cbuffer MatrixBuffer{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal: NORMAL;
};

struct PixelInputType{
    float4 position:SV_POSITION;
    float2 texCoord:TEXCOORD0;
    float3 normal:NORMAL;
};

PixelInputType SkyboxVertexShader(VertexInputType input){
    PixelInputType output;

    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.position.xyzw = output.position.xyww;

    output.texCoord = input.texCoord;
    output.normal = input.position;

    return output;
}