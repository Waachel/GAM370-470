// GraphicObject_TextureMultiLight
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_TextureMultiLight
#define _GraphicObject_TextureMultiLight

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "ShaderColorMultiLightTexture.h"
#include <Camera.h>
#include <Texture.h>
#include <GameTimer.h>

class GraphicObject_TextureMultiLight : public GraphicObject_Base
{
friend class ShaderTextureLight;

public:
	GraphicObject_TextureMultiLight(const GraphicObject_TextureMultiLight&) = delete;				 // Copy constructor
	GraphicObject_TextureMultiLight(GraphicObject_TextureMultiLight&&) = default;                    // Move constructor
	GraphicObject_TextureMultiLight& operator=(const GraphicObject_TextureMultiLight&) & = default;  // Copy assignment operator
	GraphicObject_TextureMultiLight& operator=(GraphicObject_TextureMultiLight&&) & = default;       // Move assignment operator
	~GraphicObject_TextureMultiLight();		  											 // Destructor

	GraphicObject_TextureMultiLight() = delete;

	void SetColor(const Vect& col);
	void SetWorld(const Matrix& m);
	virtual void Render() override;

	GraphicObject_TextureMultiLight(ShaderColorMultiLightTexture* shader, Model* mod, Camera mCam, Texture* pTex[]);

private:
	ShaderColorMultiLightTexture*					pShader;
	Vect							Color;
	Matrix							World;
	Camera							Cam;
	Texture* pTexture;
	Texture* MeshTextures[4];
	GameTimer mTimer;

};

#endif _GraphicObject_TextureMultiLight
