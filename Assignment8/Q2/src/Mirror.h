// Mirror
// AB, June 2016

#ifndef _Mirror
#define _Mirror

#include <d3dcommon.h>
#include "Matrix.h"
#include <string>
#include <FlatPlane.h>
#include <d3d11.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11DeviceContext;
struct D3D11_INPUT_ELEMENT_DESC;

class Mirror : public Align16
{
private:
	// Store these pointers to avoid passing them in every method
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mContext;


	// Mirror
	FlatPlane* pMirror;
	Matrix mirrorMat;
	Matrix ReflectionMat;

	ID3D11DepthStencilState* MarkMirrorDSS;
	ID3D11DepthStencilState* DrawReflectionDSS;

	ID3D11RasterizerState* FrontFaceAsCCWRS;
	ID3D11RasterizerState* MirrorFrontFaceAsClockWiseRS;

	ID3D11BlendState* NoWriteToRenderTargetBS;
	ID3D11BlendState* TransparentBS;

protected:


public:
	Mirror(const Mirror&) = delete;				  // Copy constructor
	Mirror(Mirror&&) = delete;                    // Move constructor
	Mirror& operator=(const Mirror&) & = delete;  // Copy assignment operator
	Mirror& operator=(Mirror&&) & = delete;       // Move assignment operator
	Mirror() = delete;
	virtual ~Mirror();


	Mirror(ID3D11Device* device);

	void MarkingPixels(ID3D11DeviceContext* context);

	void StopStencil(ID3D11DeviceContext* context);

	void WindingsAndStencil(ID3D11DeviceContext* context);

	void StopStencilStep3(ID3D11DeviceContext* context);

	void SetOMSetBlendStateAlpha(ID3D11DeviceContext* context);

	void SetOMSetBlendStateNormal(ID3D11DeviceContext* context);

	void RasterizerStep(ID3D11Device* device, ID3D11DeviceContext* context, D3D11_RASTERIZER_DESC& rd);

	void Render(ID3D11DeviceContext* context);

	//virtual void SetToContext(ID3D11DeviceContext* devcon) = 0;
};


#endif _Mirror