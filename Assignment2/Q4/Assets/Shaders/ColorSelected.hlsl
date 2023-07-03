struct VertexData
{
	float4 Pos : POSITION;
	uint ColorNum: COLOR;
};

struct PixelData
{
	float4 Pos : SV_POSITION;
	uint ColorNum: COLOR;
};

cbuffer bufColor
{
	//float4 ActiveCol;
	float4 ActiveCol[2];
}

// Vertex Shader
PixelData VS(VertexData inVert)
{
	return inVert;
}

// Pixel Shader
float4 PS(PixelData inPix) : SV_Target
{
	return ActiveCol[inPix.ColorNum];
}