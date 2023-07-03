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

void DXApp::InitDemo()
{


	// *********************************

	pShaderColLight = new ShaderColorLight(md3dDevice);

	pShaderTexLight = new ShaderTextureLight(md3dDevice);

	pTex = new Texture(md3dDevice, L"../Assets/Textures/CrateFace.tga");

	pTex->SetToContext(md3dImmediateContext);

	pShaderTexLight->SetDirectionalLightParameters(Vect(-1, -1, 1).getNorm(),
		.5 * Vect(1, 1, 1), .5 * Vect(1, 1, 1), Vect(1, 1, 1));
	//*/
	//*
	pShaderTexLight->SetPointLightParameters(Vect(0, 10, 0), 100, .2 * Vect(0, 1, 0),
		.1 * Vect(1, 1, 1), 2 * Vect(1, 1, 1), 1 * Vect(1, 1, 1));
	//*/

	pShaderTexLight->SetToContext(md3dImmediateContext);

	float scale = 30;
	pPlane = new FlatPlane(md3dDevice, scale, 1, 1);
	planeMat = Matrix(TRANS, 0, -4, 0);
	planeMat2 = Matrix(ROT_X, -3.1415 / 2) * Matrix(ROT_Y, 3.1415 / 4) * Matrix(TRANS, scale / 4, 0, scale / 4);

	pModel1 = new Model(md3dDevice, Model::PreMadeModels::UnitBoxRepeatedTexture);
	mWorld1 = Matrix(SCALE, 1, 1, 1) * Matrix(TRANS, 0, 0, 0);

	pModel2 = new Model(md3dDevice, Model::PreMadeModels::UnitPyramid);
	pos2 = Vect(0, 0, -4);
	mWorld2 = Matrix(SCALE, .52f, .52f, .52f) * Matrix(TRANS, pos2);

	// Mirror object
	pMirror = new FlatPlane(md3dDevice, 7, 1, 1);
	mirrorMat = planeMat2 * Matrix(TRANS, 0, 0, -0.1); // Slight offset from the 'wall' to avoid z-fighting

	mirror = new Mirror(md3dDevice, FrontFaceAsCCWRS, pMirror, mirrorMat, mirrorColor);

	// Create Reflection matrix for mirror plane.
	// Uses the mirror's normal N and position
	Vect N = mirrorMat.get(ROW_1); // Mirror normal
	Vect p = mirrorMat.get(ROW_3); // point on mirror
	float d = -p.dot(N);
	Matrix NTN(N[x] * N, N[y] * N, N[z] * N, d * N);  // Set the 4 rows
	NTN[m3] = 0;	// Zero out the #$#@$! 1 that's defaulted in the w position
	NTN[m7] = 0;
	NTN[m11] = 0;
	ReflectionMat = Matrix(IDENTITY) - 2 * NTN;
	ReflectionMat[m15] = 1; // correcting the corner entry
	// See also: https://en.wikipedia.org/wiki/Transformation_matrix
	// and http://ami.ektf.hu/uploads/papers/finalpdf/AMI_40_from175to186.pdf
	// And of course http://facweb.cs.depaul.edu/andre/GAM325/Week3.htm

	// Initialize the projection matrix
	mCam.setPerspective(3.14159 / 3, mClientWidth / (float)mClientHeight, 1.0f, 1000.0f);
	mCam.setOrientAndPosition(Vect(0, 1, 0), Vect(0, 0, 0), Vect(-5, 2, -8));

	mTimer.Reset();
}

void DXApp::UpdateScene()
{
	mWorld2 *= Matrix(ROT_Y, .03f);  // Simple slow rotation about y axis
	pos2 = mWorld2.get(MatrixRowType::ROW_3);

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
	// Clear the back buffer
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, VasA(BackgroundColor));
	// Clear the depth stencil;
	md3dImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Uncomment to show the depth buffer writes despite no rendertarget writes
	//md3dImmediateContext->OMSetBlendState(NoWriteToRenderTargetBS, nullptr, 0xffffffff);
//	pShaderColLight->SetToContext(md3dImmediateContext);
	//pShaderColLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());


	// STEP 1: Render everything that is in _front_ of the mirror
		//*
		// Plane/box display
	pShaderTexLight->SetSpotLightParameters(pos2, 4000, .5 * Vect(1, 0, 0),
		-pos2.getNorm(), 50,
		.2 * Vect(1, 1, 1), 3 * Vect(1, 1, 1), 1 * Vect(1, 1, 1, 150));

	pShaderTexLight->SetToContext(md3dImmediateContext);
	pShaderTexLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());

	Vect eyepos;
	mCam.getPos(eyepos);
	pShaderTexLight->SendLightParameters(eyepos);

	// --- Render the scene minus mirror 
	// 

	// Planes
	pShaderTexLight->SendWorldAndMaterial(planeMat, Colors::DarkGreen, Colors::DarkGreen, Vect(1, 1, 1, 100));
	pPlane->Render(md3dImmediateContext);

	// If you uncomment, the mirror isn't working. Why?
	/*
	pShaderTexLight->SendWorldColor(planeMat2, Colors::DarkMagenta);
	pPlane->Render(md3dImmediateContext);
	//*/

	// Object 1
	pShaderTexLight->SendWorldAndMaterial(mWorld1 * Matrix(TRANS, 0, 2 * cosf(mTimer.TotalTime()), 0), Colors::Firebrick, Colors::Firebrick, Vect(1, 1, 1, 100));
	pModel1->SetToContext(md3dImmediateContext);
	pModel1->Render(md3dImmediateContext);

	// Object 2
	pShaderTexLight->SendWorldAndMaterial(mWorld2, Colors::Yellow, Colors::Yellow, Vect(1, 1, 1, 100));
	pModel2->SetToContext(md3dImmediateContext);
	pModel2->Render(md3dImmediateContext);

	// **** mirror effect ***** See also http://richardssoftware.net/Home/Post/18
	// STEP 2: Marking the mirror pixels and beginning Step 3
	pShaderColLight = mirror->beginMirror(md3dImmediateContext, mCam, pShaderColLight);
	

	//STEP 3
	pShaderTexLight->SetToContext(md3dImmediateContext);
	pShaderTexLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());

	//* Reflected objects (same worldmat, but multiplied by mirror's reflection matrix)
	Vect pos3 = pos2 * ReflectionMat;
	pShaderTexLight->SetSpotLightParameters(pos3, 14, .5 * Vect(1, 0, 0),
		pos2.getNorm(), 50,
		.2 * Vect(1, 1, 1), 3 * Vect(1, 1, 1), 1 * Vect(1, 1, 1, 150));

	//pShaderTexLight->SetToContext(md3dImmediateContext);
	//pShaderTexLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	pShaderTexLight->SendLightParameters(eyepos * ReflectionMat);

	pShaderTexLight->SendWorldAndMaterial(mWorld1 * Matrix(TRANS, 0, 2 * cosf(mTimer.TotalTime()), 0) * ReflectionMat, Colors::Firebrick, Colors::Firebrick, Vect(1, 1, 1, 100));
	pModel1->SetToContext(md3dImmediateContext);
	pModel1->Render(md3dImmediateContext);
	
	pShaderTexLight->SendWorldAndMaterial(planeMat * ReflectionMat, Colors::DarkGreen, Colors::DarkGreen, Vect(1, 1, 1, 100));
	pPlane->Render(md3dImmediateContext);
	
	pShaderTexLight->SendWorldAndMaterial(mWorld2 * ReflectionMat, Colors::Yellow, Colors::Yellow, Vect(1, 1, 1, 100));
	pModel2->SetToContext(md3dImmediateContext);
	pModel2->Render(md3dImmediateContext);
	///

	//End of Step 3 and step 4
// finishMirror
	pShaderColLight = mirror->finishMirror(md3dImmediateContext, mCam, pShaderColLight);

// STEP 5: Render objects _behind_ the mirror
	// Why must this wall be drawn after the mirror for it to work?
	//*
	pShaderTexLight->SetToContext(md3dImmediateContext);
	pShaderTexLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());

	pShaderTexLight->SendWorldAndMaterial(planeMat2, Colors::DarkMagenta, Colors::DarkMagenta, Vect(1, 1, 1, 100));
	pPlane->Render(md3dImmediateContext);
	//*/

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
	delete pModel1;
	delete pModel2;
	delete pShaderTexLight;
	delete pShaderColLight;
	//delete pShaderTexLightLight;
	delete pPlane;
	delete pMirror;
	delete mirror;
	delete pTex;


	ReleaseAndDeleteCOMobject(FrontFaceAsCCWRS);

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
	assert(SUCCEEDED(hr));

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
	rd.AntialiasedLineEnable = false;

	//ID3D11RasterizerState* rs;
	md3dDevice->CreateRasterizerState(&rd, &FrontFaceAsCCWRS);

	md3dImmediateContext->RSSetState(FrontFaceAsCCWRS);
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