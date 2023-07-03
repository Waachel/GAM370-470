// Mirror
// AB, June 2016

#include <d3d11.h>
#include <FlatPlane.h>
#include <Matrix.h>
#include <Camera.h>
#include <ShaderColor.h>
#include <ShaderColorLight.h>
#ifndef _Mirror
#define _Mirror

class Mirror
{
public:
	Mirror(const Mirror&) = delete;				  // Copy constructor
	Mirror(Mirror&&) = delete;                    // Move constructor
	//Mirror& operator=(const Mirror&) &= delete;  // Copy assignment operator
	//Mirror& operator=(Mirror&&) &= delete;       // Move assignment operator
	Mirror() = delete;
	virtual ~Mirror();

	Mirror(ID3D11Device* device, ID3D11RasterizerState* rState, FlatPlane* pMirror, Matrix mMatrix, Vect mColor);
	ShaderColorLight* beginMirror(ID3D11DeviceContext* context, Camera cam, ShaderColorLight* pShader);
	ShaderColorLight* finishMirror(ID3D11DeviceContext* context, Camera cam, ShaderColorLight* pShader);
	//void beginMirror(ID3D11DeviceContext* context, Camera cam);
	//void finishMirror(ID3D11DeviceContext* context, Camera cam);
	Matrix getReflectionMatrix();

	ID3D11RasterizerState* FrontFaceAsCCWRS;

private:

	FlatPlane* pMir;
	Matrix mirMatrix;
	Matrix reflectMatrix;
	Vect mirCol;

	ShaderColorLight* pShaderColLight;

	ID3D11Device* mDevice;
	ID3D11DepthStencilState* MarkMirrorDSS;
	ID3D11DepthStencilState* DrawReflectionDSS;
	ID3D11RasterizerState* MirrorFrontFaceAsClockWiseRS;

	ID3D11BlendState* NoWriteToRenderTargetBS;
	ID3D11BlendState* TransparentBS;
};


#endif _Mirror