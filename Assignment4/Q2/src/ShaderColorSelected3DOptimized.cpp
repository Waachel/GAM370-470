#include "ShaderColorSelected3DOptimized.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>


ShaderColorSelected3DOptimized::ShaderColorSelected3DOptimized(ID3D11Device* device)
	: ShaderBase(device, L"../Assets/Shaders/ColorSelected3DOptimized.hlsl")
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);  
	this->CreateInputLayout(layout, numElements);

	HRESULT hr;  

	// Color buffer
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_WorldColorSelected);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBuffWordColor);
	assert(SUCCEEDED(hr));

}

ShaderColorSelected3DOptimized::~ShaderColorSelected3DOptimized()
{
	ReleaseAndDeleteCOMobject(mpBuffWordColor);
}


void ShaderColorSelected3DOptimized::SendWVPColor(const Matrix& world, const Vect& col)
{
	Data_WorldColorSelected wc;
	wc.WVP = world;
	wc.Color = col;

	this->GetContext()->UpdateSubresource(mpBuffWordColor, 0, nullptr, &wc, 0, 0);
}

void ShaderColorSelected3DOptimized::SetToContext(ID3D11DeviceContext* devcon)
{
	ShaderBase::SetContext(devcon);
	ShaderBase::SetToContext_VS_PS_InputLayout();

	devcon->VSSetConstantBuffers(1, 1, &mpBuffWordColor);
}


