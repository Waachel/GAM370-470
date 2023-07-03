#ifndef Shader_H
#define Shader_H
#include <d3d11.h>

class Shader
{
private:
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pPSBlob = nullptr;

	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpVertexLayout;


public:
	Shader(const Shader&) = delete;				// Copy constructor
	Shader(Shader&&) = delete;                    // Move constructor
	Shader& operator=(const Shader&) &= delete;  // Copy assignment operator
	Shader& operator=(Shader&&) &= delete;       // Move assignment operator


	//method that can take an input to identify which shader we want and 
		//load that file and create vertex shader using pVSBlob 
		//Then, based on the shader define an create input layout
		//lastly, delete pVSBlob

	//method that can take an input to identify which shader we want and 
		//load that file and create pixel shader using pPSBlob 
		//lastly, delete pPSBlob
	
	//method to set VS Shader that can be 
		//called before setting model context and rendering

	//method to set Pixel Shader that can be 
		//called before setting model context and rendering

	virtual ~Shader();
};



#endif