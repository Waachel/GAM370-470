#include "Mirror.h"
#include <d3dUtil.h>

Mirror::Mirror(ID3D11Device* device, ID3D11RasterizerState* rState, FlatPlane* pMirror, Matrix mMatrix, Vect mColor)
	: mDevice(device), pMir(pMirror), mirMatrix(mMatrix), mirCol(mColor)

{
	// ***** Mirror Preliminaries ************
	HRESULT hr = S_OK;

	pMir = pMirror;

	FrontFaceAsCCWRS = rState;

	D3D11_DEPTH_STENCIL_DESC mirrorDesc;
	ZeroMemory(&mirrorDesc, sizeof(mirrorDesc));
	mirrorDesc.DepthEnable = true;								// Use the depth test
	mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;	// but don't write to the depth buffer
	mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS;
	mirrorDesc.StencilEnable = true;							// Use the stencil test
	mirrorDesc.StencilReadMask = 0xff;
	mirrorDesc.StencilWriteMask = 0xff;

	mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		// When passing the test (Always) replace
	mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;	// the color value with the reference value (int 1)


	// Back faces are culled, so these don't matter 
	mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = mDevice->CreateDepthStencilState(&mirrorDesc, &MarkMirrorDSS);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
	ZeroMemory(&drawReflectionDesc, sizeof(drawReflectionDesc));
	drawReflectionDesc.DepthEnable = true;								// Use the depth test
	drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// normally
	drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
	drawReflectionDesc.StencilEnable = true;							// Use the stencil test
	drawReflectionDesc.StencilReadMask = 0xff;
	drawReflectionDesc.StencilWriteMask = 0xff;

	drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// When passing the stencil comparison test,
	drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		// do not modify the stencil

	// Back faces are culled, so these don't matter
	drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	hr = mDevice->CreateDepthStencilState(&drawReflectionDesc, &DrawReflectionDSS);
	assert(SUCCEEDED(hr));

	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;   // D3D11_FILL_WIREFRAME
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = false;
	rd.DepthBias = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;
	rd.AntialiasedLineEnable = false;

	hr = mDevice->CreateRasterizerState(&rd, &MirrorFrontFaceAsClockWiseRS);
	assert(SUCCEEDED(hr));

	// These settings implement the standard alpha blending: c_src*(a_src) + c_dst*(1-a_src) 
	D3D11_BLEND_DESC bd;
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = mDevice->CreateBlendState(&bd, &TransparentBS);
	assert(SUCCEEDED(hr));

	// These setting prevent all writings to the render target 
	// (the key is that RenderTargetWriteMask = 0, so nothing is allowed)
	// see https://msdn.microsoft.com/en-us/library/windows/desktop/bb204901(v=vs.85).aspx
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = false;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = 0;

	hr = mDevice->CreateBlendState(&bd, &NoWriteToRenderTargetBS);
	assert(SUCCEEDED(hr));
}

ShaderColorLight* Mirror::beginMirror(ID3D11DeviceContext* context, Camera cam, ShaderColorLight* pShader)
{
	//*
// BLEND STATE: Stop writing to the render target (but keep depth writing to depth buffer)
	context->OMSetBlendState(NoWriteToRenderTargetBS, nullptr, 0xffffffff);
	// STENCIL: Set up the stencil for marking ('1' for all pixels that passed the test. See comment at line 35)
	context->OMSetDepthStencilState(MarkMirrorDSS, 1);
	//FlatPlane* pMir;
	//Matrix mirMatrix;
	//Matrix reflectMatrix;
	//Vect mirCol;

	// Render the mirror 
	pShader->SetToContext(context);
	pShader->SendCamMatrices(cam.getViewMatrix(), cam.getProjMatrix());

	pShader->SendWorldAndMaterial(mirMatrix, Colors::DarkGray, Colors::DarkGray, Vect(1, 1, 1, 100)); // The color is irrelevant here
	pMir->Render(pShader->GetContext());

	// STENCIL: stop using the stencil
	context->OMSetDepthStencilState(0, 0);
	// BLEND STATE: Return the blend state to normal (writing to render target)
	context->OMSetBlendState(0, nullptr, 0xffffffff);
	//*/

// STEP 3:  Render objects (to render target) that should be reflected in the mirror
	//*
	// WINDINGS: face winding will appear inside out after reflection. Switching to CW front facing
	context->RSSetState(MirrorFrontFaceAsClockWiseRS);
	// STENCIL: Use the stencil test (reference value 1) and only pass the test if the stencil already had a one present
	context->OMSetDepthStencilState(DrawReflectionDSS, 1);

	return pShader;
}

ShaderColorLight* Mirror::finishMirror(ID3D11DeviceContext* context, Camera cam, ShaderColorLight* pShader)
{
	//STENCIL: Stop using the stencil
	context->OMSetDepthStencilState(0, 0);
	// WINDING: back to normal windings
	context->RSSetState(FrontFaceAsCCWRS);
	//*/

	// STEP 4: Render the mirror again, using alpha blending to 'see' the reflection 'through' it
	//		   (this assumes your mirror has a tint/texture to it. If perfectly 'clear' then this isn't needed)
		//*
		// BLENDING: Sets standard alpha blending: c_src*(a_src) + c_dst*(1-a_src)
	context->OMSetBlendState(TransparentBS, nullptr, 0xffffffff);

	pShader->SetToContext(context);
	pShader->SendCamMatrices(cam.getViewMatrix(), cam.getProjMatrix());

	pShader->SendWorldAndMaterial(mirMatrix, Vect(0, .5, 1, .3f), Vect(0, .5, 1, .3f), Vect(1, 1, 1, 100)); // CAUTION!  You _must_ have alpha < 1 to have transparency
	pMir->Render(context);

	// BLENDING: Back to normal blending (turn off transparent blending)
	context->OMSetBlendState(0, nullptr, 0xffffffff);
	//*/
	return pShader;
}

Matrix Mirror::getReflectionMatrix()
{
	return reflectMatrix;
}

Mirror::~Mirror()
{
		ReleaseAndDeleteCOMobject(MarkMirrorDSS);
	ReleaseAndDeleteCOMobject(DrawReflectionDSS);
	ReleaseAndDeleteCOMobject(MirrorFrontFaceAsClockWiseRS);
	ReleaseAndDeleteCOMobject(TransparentBS);
	ReleaseAndDeleteCOMobject(NoWriteToRenderTargetBS);
}