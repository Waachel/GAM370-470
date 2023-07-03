// GraphicObject_ColorTextureLight
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_ColorTextureLight
#define _GraphicObject_ColorTextureLight

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "ShaderColorLight.h"
#include "ShaderTexture.h"
#include <ShaderColor.h>
#include <Camera.h>
#include <ShaderTextureLight.h>

class GraphicObject_ColorTextureLight : public GraphicObject_Base
{
friend class ShaderColorTextureLight;

public:
	GraphicObject_ColorTextureLight(const GraphicObject_ColorTextureLight&) = delete;				 // Copy constructor
	GraphicObject_ColorTextureLight(GraphicObject_ColorTextureLight&&) = default;                    // Move constructor
	GraphicObject_ColorTextureLight& operator=(const GraphicObject_ColorTextureLight&) & = default;  // Copy assignment operator
	GraphicObject_ColorTextureLight& operator=(GraphicObject_ColorTextureLight&&) & = default;       // Move assignment operator
	~GraphicObject_ColorTextureLight();		  											 // Destructor

	GraphicObject_ColorTextureLight() = delete;

	void SetColor(const Vect& col);
	void SetColor(const Vect& col, int meshnum);
	void SetWorld(const Matrix& m);
	virtual void Render() override;

	GraphicObject_ColorTextureLight(Model* mod);

	void SetShader(ShaderColor* shader, int meshnum, Camera mCam, const Vect& col);

	void SetShader(ShaderTexture* shader, int meshnum, Camera mCam, Texture* pTex);

	void SetShader(ShaderColorLight* shader, int meshnum, Camera mCam, const Vect& col);

	void SetShader(ShaderTextureLight* shader, int meshnum, Camera mCam, Texture* pTex);
	
	void SetContextAndCamCTL(ID3D11DeviceContext* devcon,const Matrix& view, const Matrix& proj);


private:
	ShaderColor*					pShaderColor;
	ShaderTexture* pShaderTexture;
	ShaderColorLight* pShaderColorLight;
	ShaderTextureLight* pShaderTextureLight;
	Vect							ColorC;
	Vect							ColorCL;
	Vect*							MeshColors;
	Camera							Cam;
	Matrix							World, View, Project;
	Texture* pTexture;
	ID3D11DeviceContext* context;

	int shaderNum; //0 = Color, 1 = Texture, 2 = Color and Light

};

#endif _GraphicObject_ColorTextureLight
