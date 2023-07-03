// GraphicObject_Texture
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_Texture
#define _GraphicObject_Texture

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "ShaderTexture.h"

class GraphicObject_Texture : public GraphicObject_Base
{
	friend class ShaderTexture;

public:
	GraphicObject_Texture(const GraphicObject_Texture&) = delete;				 // Copy constructor
	GraphicObject_Texture(GraphicObject_Texture&&) = default;                    // Move constructor
	//GraphicObject_Texture& operator=(const GraphicObject_Texture&) &= default;  // Copy assignment operator
	//GraphicObject_Texture& operator=(GraphicObject_Texture&&) &= default;       // Move assignment operator
	~GraphicObject_Texture();		  											 // Destructor

	void SetTexture(Texture* tex);

	void SetTexture(int meshnum);

	GraphicObject_Texture() = delete;

	//void SetColor(const Vect& col);
	void SetWorld(const Matrix& m);
	virtual void Render() override;

	GraphicObject_Texture(ShaderTexture* shader, Model* mod, Texture* pTex[]);

private:
	ShaderTexture* pShader;
	//Vect							Color;
	Texture* pTexture;
	Texture* MeshTextures[4];
	Matrix							World;

};

#endif _GraphicObject_Texture
