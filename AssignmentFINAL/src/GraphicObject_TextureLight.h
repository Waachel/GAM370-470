// GraphicObject_TextureLight
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_TextureLight
#define _GraphicObject_TextureLight

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "ShaderTextureLight.h"
#include <Camera.h>
#include <Texture.h>
#include <GameTimer.h>
#include <ShaderColorMultiLightTexture.h>

class GraphicObject_TextureLight : public GraphicObject_Base
{
friend class ShaderTextureLight;

public:
	GraphicObject_TextureLight(const GraphicObject_TextureLight&) = delete;				 // Copy constructor
	GraphicObject_TextureLight(GraphicObject_TextureLight&&) = default;                    // Move constructor
	GraphicObject_TextureLight& operator=(const GraphicObject_TextureLight&) & = default;  // Copy assignment operator
	GraphicObject_TextureLight& operator=(GraphicObject_TextureLight&&) & = default;       // Move assignment operator
	~GraphicObject_TextureLight();		  											 // Destructor

	GraphicObject_TextureLight() = delete;

	void SetColor(const Vect& col);
	void SetWorld(const Matrix& m);
	void SetPointLight(const Vect& m);
	void SetTime(const float& m);
	virtual void Render() override;

	GraphicObject_TextureLight(ShaderColorMultiLightTexture* shader, Model* mod, Camera mCam, Texture* pTex[]);

private:
	ShaderColorMultiLightTexture*					pShader;
	Vect							Color;
	Matrix							World;
	Vect							matrixPL;
	Camera							Cam;
	float time;
	Texture* pTexture;
	Texture* MeshTextures[4];
	GameTimer mTimer;
	Matrix mWorld75;
	Vect pos75;

};

#endif _GraphicObject_TextureLight
