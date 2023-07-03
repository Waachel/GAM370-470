#include "DXApp.h"
#include <windows.h>
#include <sstream>
#include <assert.h>
#include "d3dUtil.h"

// needed to load shaders from file
#include <d3dcompiler.h>

#include "Model.h"
#include "ShaderColor.h"
#include "DirectXTex.h"
#include "Texture.h"
#include "FlatPlane.h"
#include "TerrainModel.h"
#include "GraphicObject_Color.h"

void DXApp::InitDemo()
{
	HRESULT hr = S_OK;

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

	hr = md3dDevice->CreateRasterizerState(&rd, &FrontFaceAsClockWiseRS);
	assert(SUCCEEDED(hr));
	
	//Skybox
	pShaderTex = new ShaderTexture(md3dDevice);
	pShaderTextureLight = new ShaderTextureLight(md3dDevice);
	pShaderTextureMultiLight = new ShaderColorMultiLightTexture(md3dDevice);

	mTex1 = new Texture(md3dDevice, L"../Assets/Textures/Space6.tga", D3D11_FILTER_MIN_MAG_MIP_POINT);

	mTex1->SetToContext(md3dImmediateContext);

	pSkyBox = new SkyBox(md3dDevice, 1000);
	mWorld1 = Matrix(IDENTITY);

	mWorld35 = mWorld3 * Matrix(TRANS, -50, 0, 0);
	mWorld3 = Matrix(SCALE, 0.1, 0.1, 0.1) * Matrix(TRANS, -150, 255, -30) * Matrix(ROT_X, 92.7);

	pShaderTextureMultiLight->SetDirectionalLightParameters(Vect(-1, -1, 1).getNorm(),
		.5 * Vect(1, 1, 1), .5 * Vect(1, 1, 1), Vect(1, 1, 1));

	//light from door of building
	posSL1 = mWorld3.get(MatrixRowType::ROW_3);
	pShaderTextureMultiLight->SetSpotLightParameters(0, posSL1, 200, 1 * Vect(1, 0, 0),
		-posSL1.getNorm() * Matrix(TRANS, 0, 0, 50), 100,
		1 * Vect(1, 1, 1), 3 * Vect(1, 1, 1), 1 * Vect(1, 1, 1, 150));

	pShaderTextureLight->SetToContext(md3dImmediateContext);
	//pShaderTextureLight->SendWorldColor(planeMat, Colors::DarkGreen);

	//Terrain
	mTexPlane = new Texture(md3dDevice, L"../Assets/Textures/RedPlanet3.tga", D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	pTerrainModel = new TerrainModel(md3dDevice, L"../Assets/Textures/crater_heightmap.tga", 1500, 100, 0, 1, 1);
	mWorldT = Matrix(IDENTITY) * Matrix(TRANS, -500, -50, 0);
	//mWorldT = Matrix(SCALE, 1, 1, 1) * Matrix(TRANS, 0, -15, 0);

	//Crate - repeated texture cube
	mTexBlock[0] = new Texture(md3dDevice, L"../Assets/Textures/orange-crate.tga", D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	pModel2 = new Model(md3dDevice, Model::UnitBoxRepeatedTexture);
	GraphObjTL1 = new GraphicObject_TextureLight(pShaderTextureMultiLight, pModel2, mCam, mTexBlock);
	mWorldT2 = Matrix(SCALE, 10, 10, 10) * Matrix(TRANS, -105, 0, -225);
	GraphObjTL1->SetWorld(mWorldT2);

	//Crate - repeated texture cube
	pModel5 = new Model(md3dDevice, Model::UnitBoxRepeatedTexture);
	GraphObjTL5 = new GraphicObject_TextureLight(pShaderTextureMultiLight, pModel5, mCam, mTexBlock);
	mWorld5 = Matrix(SCALE, 10, 10, 10) * Matrix(TRANS, -125, 0, -205);
	GraphObjTL5->SetWorld(mWorld5);

	//Crate - 6-sided cube
	mTexBlock6[0] = new Texture(md3dDevice, L"../Assets/Textures/crate-6-face.tga", D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	pModel6 = new Model(md3dDevice, Model::UnitBoxSixFacesTexture);
	GraphObjTL6 = new GraphicObject_TextureLight(pShaderTextureMultiLight, pModel6, mCam, mTexBlock6);
	mWorld6 = Matrix(SCALE, 10, 10, 10) * Matrix(TRANS, -105, 10, -225);
	GraphObjTL6->SetWorld(mWorld6);

	//Pyramid Light #1
	mTexBlock7[0] = new Texture(md3dDevice, L"../Assets/Textures/triangle2.tga", D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	pModel7 = new Model(md3dDevice, Model::UnitPyramidRepeatedTexture);
	GraphObjTL7 = new GraphicObject_TextureLight(pShaderTextureMultiLight, pModel7, mCam, mTexBlock7);
	mWorld7 = Matrix(SCALE, 15, 15, 15) * Matrix(TRANS, 115, -12, -220);
	GraphObjTL7->SetWorld(mWorld7);

	//Pyramid Light #2
	//pModel8 = new Model(md3dDevice, Model::UnitPyramidRepeatedTexture);
	GraphObjTL8 = new GraphicObject_TextureLight(pShaderTextureMultiLight, pModel7, mCam, mTexBlock7);
	mWorld8 = Matrix(SCALE, 15, 15, 15) * Matrix(TRANS, -80, -20, -80);
	GraphObjTL8->SetWorld(mWorld8);

	//Pyramid Light #3
	//pModel8 = new Model(md3dDevice, Model::UnitPyramidRepeatedTexture);
	GraphObjTL9 = new GraphicObject_TextureLight(pShaderTextureMultiLight, pModel7, mCam, mTexBlock7);
	mWorld9 = Matrix(SCALE, 15, 15, 15) * Matrix(TRANS, -250, 10, -300);
	GraphObjTL9->SetWorld(mWorld9);

	//Sphere
	pModel10 = new Model(md3dDevice, Model::UnitSphere);
	mWorld10 = Matrix(SCALE, 20, 20, 20) * Matrix(TRANS, -160, 200, -250);

	//Shuttle
	mTexBlock2[0] = new Texture(md3dDevice, L"../Assets/Textures/Freighter_S.tga", D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	mTexBlock2[1] = new Texture(md3dDevice, L"../Assets/Textures/Freighter_S.tga", D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	pModel1 = new Model(md3dDevice, "../Assets/Models/freighter.azul");
	GraphObjTL2 = new GraphicObject_TextureLight(pShaderTextureMultiLight, pModel1, mCam, mTexBlock2);
	mWorld2 = Matrix(SCALE, 5, 5, 5) * Matrix(TRANS, 0, 255, -70) * Matrix(ROT_X, 92.8);
	GraphObjTL2->SetWorld(mWorld2);

	//Building
	mTexBlock3[0] = new Texture(md3dDevice, L"../Assets/Textures/metal.tga", D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	pModel3 = new Model(md3dDevice, "../Assets/Models/beamEmitter.azul");
	GraphObjTL3 = new GraphicObject_TextureLight(pShaderTextureMultiLight, pModel3, mCam, mTexBlock3);
	GraphObjTL3->SetWorld(mWorld3);

	//Astronaut
	mTexBlock4[0] = new Texture(md3dDevice, L"../Assets/Textures/astro2.tga", D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	pModel4 = new Model(md3dDevice, "../Assets/Models/astronaut3.azul");
	GraphObjTL4 = new GraphicObject_TextureLight(pShaderTextureMultiLight, pModel4, mCam, mTexBlock4);
	mWorld4 = Matrix(SCALE, 4, 4, 4) * Matrix(TRANS, -150, 190, -35) * Matrix(ROT_X, 92.7);
	GraphObjTL4->SetWorld(mWorld4);

	// Initialize the projection matrix
	mCam.setPerspective( 3.14159/3, mClientWidth / (float)mClientHeight, 1.0f, 1000.0f);
	mCam.setOrientAndPosition(Vect(0, 1, 0), Vect(-140, 60, -250), Vect(-280, 70, 40));

	mTimer.Reset();
}

void DXApp::UpdateScene()
{
	mWorld2 *= Matrix(ROT_Y, 0.0003);
	//GraphObj2->SetWorld(mWorld2);

	float camSpeed = 0.4f;
	if (GetKeyState('W') & 0x08000)
	{
		mCam.TranslateFwdBack(camSpeed);
	}
	else if (GetKeyState('S') & 0x08000)
	{
		mCam.TranslateFwdBack(-camSpeed);
	}

	if (GetKeyState('A') & 0x08000)
	{
		mCam.TranslateLeftRight(-camSpeed);
	}
	else if (GetKeyState('D') & 0x08000)
	{
		mCam.TranslateLeftRight(camSpeed);
	}

	float rotSpeed = 0.05f;
	if (GetKeyState(VK_LEFT) & 0x08000)
	{
		mCam.TurnLeftRight(rotSpeed);
	}
	else if (GetKeyState(VK_RIGHT) & 0x08000)
	{
		mCam.TurnLeftRight(-rotSpeed);
	}

	if (GetKeyState(VK_UP) & 0x08000)
	{
		mCam.TiltUpDown(rotSpeed);
	}
	else if (GetKeyState(VK_DOWN) & 0x08000)
	{
		mCam.TiltUpDown(-rotSpeed);
	}

	mCam.updateCamera();
}

void DXApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, VasA(BackgroundColor));
	md3dImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//Render SkyBox
	md3dImmediateContext->RSSetState(FrontFaceAsClockWiseRS);
	mTex1->SetToContext(md3dImmediateContext);
	pShaderTex->SetToContext(md3dImmediateContext);
	pShaderTex->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());

	pShaderTex->SendWorld(mWorld1);

	pSkyBox->Render(md3dImmediateContext);

	md3dImmediateContext->RSSetState(FrontFaceAsCCWRS);

	count++;
	if (count == 50)
	{
		count1 = 70.0f;
	}

	if (count == 150)
	{
		count1 = 95.0f;
		count = 0;
	}

	if (count == 250)
	{
		count1 = 120.0f;
		count = 0;
	}

	//light from top of building
	pShaderTextureMultiLight->SetSpotLightParameters(1, posSL1, 4000, 1 * Vect(1, 0, 0),
		-posSL1.getNorm() * Matrix(TRANS, 0, 50, 0), count1,
		2 * Vect(0.541176498f, 0.168627456f, 0.886274576f), 3 * Vect(0.541176498f, 0.168627456f, 0.886274576f), 1 * Vect(0.541176498f, 0.168627456f, 0.886274576f, 150));

	mWorld75 = mWorld7 * Matrix(TRANS, 0, 2 * cosf(mTimer.TotalTime()), 0);
	pos75 = mWorld75.get(MatrixRowType::ROW_3);
	pShaderTextureMultiLight->SetPointLightParameters(0, pos75, 15, .2 * Vect(0, 1, 0),
		(2 * cosf(mTimer.TotalTime()) + 2.5f) * Vect(1, 1, 1), 2 * Vect(1, 1, 1), 1 * Vect(1, 1, 1));

	mWorld85 = mWorld8 * Matrix(TRANS, 0, 2 * cosf(mTimer.TotalTime()), 0);
	pos85 = mWorld85.get(MatrixRowType::ROW_3);
	pShaderTextureMultiLight->SetPointLightParameters(1, pos85, 12, .2 * Vect(0, 1, 0),
		(2 * cosf(mTimer.TotalTime()) + 2) * Vect(1, 1, 1), 2 * Vect(1, 1, 1), 1 * Vect(1, 1, 1));

	mWorld95 = mWorld9 * Matrix(TRANS, 0, 2 * cosf(mTimer.TotalTime()), 0);
	pos95 = mWorld95.get(MatrixRowType::ROW_3);
	pShaderTextureMultiLight->SetPointLightParameters(2, pos95, 12, .2 * Vect(0, 1, 0),
		(2 * cosf(mTimer.TotalTime()) + 2) * Vect(1, 1, 1), 2 * Vect(1, 1, 1), 1 * Vect(1, 1, 1));

	pShaderTextureMultiLight->SetFogParameters(3000, 1000, Vect(0.745098066f, 0.270588249f, 0.074509807f, 1));
	//(0.745098066f, 0.270588249f, 0.074509807f, 1)

	pShaderTextureMultiLight->SetToContext(md3dImmediateContext);
	pShaderTextureMultiLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());

	Vect eyepos;
	mCam.getPos(eyepos);

	pShaderTextureMultiLight->SendLightParameters(eyepos);

	//Terrain
	mTexPlane->SetToContext(md3dImmediateContext);
	pShaderTextureMultiLight->SetToContext(md3dImmediateContext);
	pShaderTextureMultiLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	pShaderTextureMultiLight->SendWorldAndMaterial(mWorldT, Colors::Tan, 1 * Colors::Tan, Vect(.7, .7, .7, 150));
	pTerrainModel->Render(md3dImmediateContext);

	//Station platform
	
	
	//Crates
	//pShaderTextureLight->SetToContext(md3dImmediateContext);
	GraphObjTL1->Render();
	GraphObjTL5->Render();

	//Crate 6-sided
	GraphObjTL6->Render();

	//Pyramid Light #1
	GraphObjTL7->SetWorld(mWorld7 * Matrix(TRANS, 0, 2 * cosf(mTimer.TotalTime()), 0));
	GraphObjTL7->Render();
	GraphObjTL8->SetWorld(mWorld8 * Matrix(TRANS, 0, 2 * cosf(mTimer.TotalTime()), 0));
	GraphObjTL8->Render();
	GraphObjTL9->SetWorld(mWorld9 * Matrix(TRANS, 0, 2 * cosf(mTimer.TotalTime()), 0));
	GraphObjTL9->Render();

	//freighter
	GraphObjTL2->Render();

	//building
	GraphObjTL3->Render();

	//Astronaut
	GraphObjTL4->Render();

	//Sphere
	pModel10->SetToContext(md3dImmediateContext);
	pShaderTextureMultiLight->SendWorldAndMaterial(mWorld10 * Matrix(TRANS, 0, 2 * cosf(mTimer.TotalTime()), 0), Colors::LimeGreen, Colors::LimeGreen, Vect(1, 1, 1, 100));
	pModel10->Render(md3dImmediateContext);

	// Switches the display to show the now-finished back-buffer
	mSwapChain->Present(SyncInterval, 0);
}


DXApp::DXApp(HWND hwnd)
{
	assert(hwnd);
	mhMainWnd = hwnd;

	BackgroundColor = Colors::MidnightBlue;

	md3dDevice = nullptr;
	md3dImmediateContext = nullptr;
	mSwapChain = nullptr;
	mRenderTargetView = nullptr;

	// Get window data through the window handle
	RECT rc;
	BOOL err = GetClientRect(mhMainWnd, &rc);  // Seriously MS: Redifining BOOL as int? Confusing much?
	assert(err);

	// get width/hight
	mClientWidth = rc.right - rc.left;
	mClientHeight = rc.bottom - rc.top;

	// Get window caption
	const int MAX_LABEL_LENGTH = 100; // probably overkill...
	CHAR str[MAX_LABEL_LENGTH];
	GetWindowText(mhMainWnd, str, MAX_LABEL_LENGTH);
	mMainWndCaption = str;

	// Initialize DX11
	this->InitDirect3D();

	// Demo initialization
	this->InitDemo();
}

DXApp::~DXApp()
{

	delete pShaderTex;
	delete pShaderTextureLight;
	delete pShaderTextureMultiLight;
	delete pModel1;
	delete pModel2;
	delete pModel3;
	delete pModel4;
	delete pModel5;
	delete pModel6;
	delete pModel7;
	delete pModel10;
	delete pTerrainModel;
	delete GraphObjTL1;
	delete GraphObjTL2;
	delete GraphObjTL3;
	delete GraphObjTL4;
	delete GraphObjTL5;
	delete GraphObjTL6;
	delete GraphObjTL7;
	delete GraphObjTL8;
	delete GraphObjTL9;
	delete mTex1;
	delete mTexBlock[0];
	delete mTexBlock2[0];
	delete mTexBlock2[1];
	delete mTexBlock3[0];
	delete mTexBlock4[0];
	delete mTexBlock6[0];
	delete mTexBlock7[0];
	delete mTexPlane;




	delete pSkyBox;

	ReleaseAndDeleteCOMobject(mRenderTargetView);
	ReleaseAndDeleteCOMobject(mpDepthStencilView);
	ReleaseAndDeleteCOMobject(mSwapChain);
	ReleaseAndDeleteCOMobject(md3dImmediateContext);

	// Must be done BEFORE the device is released
	ReportLiveDXObjects();		// See http://masterkenth.com/directx-leak-debugging/

	ReleaseAndDeleteCOMobject(md3dDevice);
}

// See http://masterkenth.com/directx-leak-debugging/
void DXApp::ReportLiveDXObjects()
{
#ifdef _DEBUG
	HRESULT hr = S_OK;

	// Now we set up the Debug interface, to be queried on shutdown
	ID3D11Debug* debugDev;
	hr = md3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));

	debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	ReleaseAndDeleteCOMobject(debugDev);
#endif
}

void DXApp::InitDirect3D()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// This is a *greatly* simplified process to create a DX device and context:
	// We force the use of DX11 feature level since that's what CDM labs are limited to.
	// For real-life applications would need to test what's the best feature level and act accordingly
	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0, D3D11_SDK_VERSION, &md3dDevice, nullptr, &md3dImmediateContext);
	assert(SUCCEEDED(hr));

	// Now we obtain the associated DXGIfactory1 with our device 
	// Many steps...
	IDXGIDevice* dxgiDevice = nullptr;
	hr = md3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	assert(SUCCEEDED(hr));

	IDXGIAdapter* adapter = nullptr;
	hr = dxgiDevice->GetAdapter(&adapter);
	assert(SUCCEEDED(hr));

	IDXGIFactory1* dxgiFactory1 = nullptr;
	hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory1));
	assert(SUCCEEDED(hr));
	// See also note on weird stuff with factories and swap chains (1s and 2s)
	// https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

	// We are done with these now...
	ReleaseAndDeleteCOMobject(adapter);
	ReleaseAndDeleteCOMobject(dxgiDevice);

	// Controls MSAA option:
	// - 4x count level garanteed for all DX11 
	// - MUST be the same for depth buffer!
	// - We _need_ to work with the depth buffer because reasons... (see below)
	DXGI_SAMPLE_DESC sampDesc;
	sampDesc.Count = 1;
	sampDesc.Quality = static_cast<UINT>(D3D11_CENTER_MULTISAMPLE_PATTERN);  // MS: what's with the type mismtach?

	DXGI_MODE_DESC buffdesc;				// https://msdn.microsoft.com/en-us/library/windows/desktop/bb173064(v=vs.85).aspx
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Next we create a swap chain. 
	// Useful thread: http://stackoverflow.com/questions/27270504/directx-creating-the-swapchain
	// Note that this is for a DirectX 11.0: in a real app, we should test the feature levels and act accordingly

	DXGI_SWAP_CHAIN_DESC sd;				// See MSDN: https://msdn.microsoft.com/en-us/library/windows/desktop/bb173075(v=vs.85).aspx
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;						// Much confusion about this number... see http://www.gamedev.net/topic/633807-swap-chain-buffer-count/
	sd.BufferDesc = buffdesc;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = mhMainWnd;
	sd.SampleDesc = sampDesc;
	sd.Windowed = TRUE;

	hr = dxgiFactory1->CreateSwapChain(md3dDevice, &sd, &mSwapChain);
	assert(SUCCEEDED(hr));
	ReleaseAndDeleteCOMobject(dxgiFactory1);

	// Create a render target view		https://msdn.microsoft.com/en-us/library/windows/desktop/ff476582(v=vs.85).aspx
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	assert(SUCCEEDED(hr));;

	hr = md3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetView);
	ReleaseAndDeleteCOMobject(pBackBuffer);
	assert(SUCCEEDED(hr));

	/**********************************************************/

	// First we fix what it means for triangles to be front facing.
	// Requires setting a whole new rasterizer state
	//*
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;  // Also: D3D11_FILL_WIREFRAME
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = true; // true for RH forward facing
	rd.DepthBias = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;  // Does not in fact turn on/off multisample: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476198(v=vs.85).aspx
	rd.AntialiasedLineEnable = true;

	md3dDevice->CreateRasterizerState(&rd, &FrontFaceAsCCWRS);

	md3dImmediateContext->RSSetState(FrontFaceAsCCWRS);

	//ID3D11RasterizerState* rs;
	//md3dDevice->CreateRasterizerState(&rd, &rs);

	//md3dImmediateContext->RSSetState(rs);
	//ReleaseAndDeleteCOMobject(rs); // we can release this resource since we won't be changing it any further
	//*/

	// We must turn on the abilty to process depth during rendering.
	// Done through depth stencils (see https://msdn.microsoft.com/en-us/library/windows/desktop/bb205074(v=vs.85).aspx)
	// Below is a simplified version
	//*
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = mClientWidth;
	descDepth.Height = mClientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc = sampDesc;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* pDepthStencil;
	hr = md3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	assert(SUCCEEDED(hr));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;;

	hr = md3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &mpDepthStencilView);
	assert(SUCCEEDED(hr));
	ReleaseAndDeleteCOMobject(pDepthStencil);
	//*/

	/**********************************************************/

	//md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);  // to use without depth stencil
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mpDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)mClientWidth;
	vp.Height = (FLOAT)mClientHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	md3dImmediateContext->RSSetViewports(1, &vp);
}

void DXApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::ostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << mspf << " (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void DXApp::FrameTick()
{
	mTimer.Tick();
	CalculateFrameStats();

	this->UpdateScene();
	this->DrawScene();
}

void DXApp::OnMouseDown(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void DXApp::OnMouseUp(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void DXApp::OnMouseMove(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void  DXApp::OnMouseWheel(short delta)
{
	UNREFERENCED_PARAMETER(delta);
}


// Shader loading utility. Will be moved elsewhere later...
// Needs to be moved. Requires
HRESULT DXApp::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}