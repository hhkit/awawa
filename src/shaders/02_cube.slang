struct VSInput
{
    float3 Pos : ATTRIB0;
    float4 Color : ATTRIB1;
};

struct PSInput
{
    float4 Pos : SV_POSITION;
    float4 Color;
    float4 CamPos;
    // float4x4 mvp;
};

struct PSOutput
{
    float4 Color;
};

cbuffer Constants
{
    float4x4 g_WorldView;
    float4x4 g_Proj;
};

[shader("vertex")]
void vertexMain(in  VSInput VSIn,
  out PSInput PSIn)
{
    float4x4 mvp = g_WorldView * g_Proj;
    PSIn.CamPos = mul(g_WorldView, float4(VSIn.Pos, 1.0));
    PSIn.Pos   = mul(mvp, float4(VSIn.Pos, 1.0));
    PSIn.Color = VSIn.Color;
    // PSIn.mvp = mul(g_Proj, g_WorldView);
}

[shader("pixel")]
void fragmentMain(in  PSInput  PSIn,
          out PSOutput PSOut)
{
    float4 Color = PSIn.Color;
    PSOut.Color = Color;
}