// GraphicObject_ColorLight
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_ColorLight
#define _GraphicObject_ColorLight

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "ShaderColorLight.h"
#include <Camera.h>
#include <GameTimer.h>

class GraphicObject_ColorLight : public GraphicObject_Base
{
friend class ShaderColorLight;

public:
	GraphicObject_ColorLight(const GraphicObject_ColorLight&) = delete;				 // Copy constructor
	GraphicObject_ColorLight(GraphicObject_ColorLight&&) = default;                    // Move constructor
	GraphicObject_ColorLight& operator=(const GraphicObject_ColorLight&) & = default;  // Copy assignment operator
	GraphicObject_ColorLight& operator=(GraphicObject_ColorLight&&) & = default;       // Move assignment operator
	~GraphicObject_ColorLight();		  											 // Destructor

	GraphicObject_ColorLight() = delete;

	void SetColor(const Vect& col);
	void SetColor(const Vect& col, int meshnum);
	void SetWorld(const Matrix& m);
	virtual void Render() override;

	GraphicObject_ColorLight(ShaderColorLight* shader, Model* mod, Camera mCam);

private:
	ShaderColorLight*					pShader;
	Vect							Color;
	Matrix							World;
	Camera							Cam;
	GameTimer mTimer;
	Vect* MeshColors;

};

#endif _GraphicObject_ColorLight
