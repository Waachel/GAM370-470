// DXApp
// Andre Berthiaume, June 2016
// Note: DX 11 SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812
// Note on weird stuff with swap chain (1s and 2s) https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

#ifndef _DXApp
#define _DXApp

#include <d3d11.h>
#include "d3dUtil.h"
#include "Align16.h"
#include "GameTimer.h"
#include <string>

// New includes for demo
#include "Vect.h"
#include "Matrix.h"
#include "Camera.h"
#include "ShaderColor.h"
#include "ShaderColorLight.h"
#include "ShaderTexture.h"
#include "ShaderTextureLight.h"
#include <GraphicObject_ColorLight.h>
#include <GraphicObject_Texture.h>
#include <GraphicObject_TextureLight.h>
#include <SkyBox.h>
#include <TerrainModelIsland.h>
#include <ShaderColorMultiLightTexture.h>
#include <GraphicObject_TextureMultiLight.h>

class Model;
class Texture;
class FlatPlane;
class TerrainModel;
class GraphicObject_Color;
class ShaderTexture;
class GraphicObject_Texture;
class SkyBox;

class DXApp : public Align16
{
private:	
	// Main window handle
	HWND      mhMainWnd;

	Vect BackgroundColor;
	int SyncInterval = 1;  // 1 for sync to refresh rate, 0 for no sync

	// DX application elements
	ID3D11Device* md3dDevice;					// Connects to the graphics card
	ID3D11DeviceContext* md3dImmediateContext;	// Settings for the GPU to use
	IDXGISwapChain* mSwapChain;					// image buffers used for rendering
	ID3D11RenderTargetView* mRenderTargetView;	// Where to send rendring operations (typically: points to one of the swap buffers)
	ID3D11DepthStencilView* mpDepthStencilView; // Needed to force depth-buffer operations

	GameTimer mTimer;
	std::string mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	void InitDirect3D();
	void InitDemo();
	void UpdateScene();
	void DrawScene();
	void CalculateFrameStats();

	// Shader loading utility. SHould be moved elsewhere later...
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	// Debug utility
	void ReportLiveDXObjects();

	// Demo specific additions
	Camera mCam;

	Model* pModel1;
	Matrix mWorld1;	

	Model* pModel2;
	Matrix mWorld2;

	Model* pModel3;
	Matrix mWorld35;
	Matrix mWorld3;

	Model* pModel4;
	Matrix mWorld4;

	Model* pModel5;
	Matrix mWorld5;

	Model* pModel6;
	Matrix mWorld6;

	Model* pModel7;
	Matrix mWorld7;
	Matrix mWorld75;
	Vect pos75;

	Model* pModel8;
	Matrix mWorld8;
	Matrix mWorld85;
	Vect pos85;

	Model* pModel9;
	Matrix mWorld9;
	Matrix mWorld95;
	Vect pos95;

	Model* pModel10;
	Matrix mWorld10;

	Vect posSL1;
	int count = 0;
	float count1 = 120.0f;

	ShaderColor*			pShaderCol;
	ShaderColorLight* pShaderColorLight;
	ShaderTexture* pShaderTex;
	ShaderTextureLight* pShaderTextureLight;
	ShaderTextureLight* pShaderTextureLight2;
	ShaderColorMultiLightTexture* pShaderTextureMultiLight;
	GraphicObject_Color*	GraphObjC1;
	GraphicObject_Color*	GraphObjC2;
	GraphicObject_ColorLight* GraphObjCL1;
	GraphicObject_ColorLight* GraphObjCL2;
    
	//ShaderTexture*		pShaderTex;
	GraphicObject_Texture*	GraphObjT1;
	GraphicObject_Texture*	GraphObjT2;
	GraphicObject_TextureLight* GraphObjTL1;
	GraphicObject_TextureLight* GraphObjTL2;
	GraphicObject_TextureLight* GraphObjTL3;
	GraphicObject_TextureLight* GraphObjTL4;
	GraphicObject_TextureLight* GraphObjTL5;
	GraphicObject_TextureLight* GraphObjTL6;
	GraphicObject_TextureLight* GraphObjTL7;
	GraphicObject_TextureLight* GraphObjTL8;
	GraphicObject_TextureLight* GraphObjTL9;
	GraphicObject_TextureLight* GraphObjTL10;
	Texture*				pTex1[1];
	Texture*				pTex2;
	Texture* allTextures[4];
	Texture* mTex1;
	Texture* mTex2;

	SkyBox* pSkyBox;

	TerrainModel* pTerrainModel;
	Texture* mTexPlane;
	Matrix mWorldT;
	Matrix mWorldT2;
	Texture* mTexBlock[1];
	Texture* mTexBlock2[2];
	Texture* mTexBlock3[1];
	Texture* mTexBlock4[1];
	Texture* mTexBlock5[1];
	Texture* mTexBlock6[1];
	Texture* mTexBlock7[1];

	FlatPlane* pPlane;
	Matrix planeMat;
	Vect pos2;

	ID3D11RasterizerState* FrontFaceAsCCWRS;
	ID3D11RasterizerState* FrontFaceAsClockWiseRS;

public:
	DXApp(HWND hwnd);
	virtual ~DXApp();
	
	void FrameTick();

	// overrides for handling mouse input.
	void OnMouseDown(WPARAM btnState, int xval, int yval);
	void OnMouseUp(WPARAM btnState, int xval, int yval);
	void OnMouseMove(WPARAM btnState, int xval, int yval);
	void OnMouseWheel(short delta);


};

#endif _DXApp