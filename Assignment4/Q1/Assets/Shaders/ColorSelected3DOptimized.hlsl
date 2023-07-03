#pragma pack_matrix( row_major )

cbuffer WVPData : register(b1)
{
    float4x4 WVP;
    float4 SelectedColor;
};

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION)
{
    VS_OUTPUT output;
    output.Pos = mul(Pos, WVP);
    output.Color = SelectedColor;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.Color;
}
