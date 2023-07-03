#include "GraphicObject_ColorTextureLight.h"
#include "Model.h"
#include "ShaderColorLight.h"
#include <assert.h>
#include "ShaderTexture.h"
#include "Texture.h"
#include <d3dUtil.h>



GraphicObject_ColorTextureLight::GraphicObject_ColorTextureLight(Model* mod)
{
	SetModel(mod );


	//Color = Vect(1, 1, 1);
	World = Matrix(IDENTITY);

}

void GraphicObject_ColorTextureLight::SetShader(ShaderColor* shader, int meshnum, Camera mCam, const Vect& col)
{
	assert(pModel->ValidMeshNum(meshnum));
	pShaderColor = shader;
	shaderNum = meshnum;
	Cam = mCam;
	ColorC = col;
}

void GraphicObject_ColorTextureLight::SetShader(ShaderTexture* shader, int meshnum, Camera mCam, Texture* pTex)
{
	assert(pModel->ValidMeshNum(meshnum));
	pShaderTexture = shader;
	shaderNum = meshnum;
	Cam = mCam;
	pTexture = pTex;
}



void GraphicObject_ColorTextureLight::SetShader(ShaderColorLight* shader, int meshnum, Camera mCam, const Vect& col)
{
	assert(pModel->ValidMeshNum(meshnum));
	pShaderColorLight = shader;
	shaderNum = meshnum;
	Cam = mCam;
	ColorCL = col;
}

void GraphicObject_ColorTextureLight::SetShader(ShaderTextureLight* shader, int meshnum, Camera mCam, Texture* pTex)
{
	assert(pModel->ValidMeshNum(meshnum));
	pShaderTextureLight = shader;
	shaderNum = meshnum;
	pTexture = pTex;
	Cam = mCam;
}

GraphicObject_ColorTextureLight::~GraphicObject_ColorTextureLight()
{
	//delete[] MeshColors;
}

void GraphicObject_ColorTextureLight::SetColor(const Vect& col) 
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
		MeshColors[i] = col;
}

void GraphicObject_ColorTextureLight::SetColor(const Vect& col, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));
	MeshColors[meshnum] = col;
}

void GraphicObject_ColorTextureLight::SetWorld(const Matrix& m) 
{ 
	World = m;
}

void GraphicObject_ColorTextureLight::SetContextAndCamCTL(ID3D11DeviceContext* devcon, const Matrix& view, const Matrix& proj)
{
	context = devcon;
	View = view;
	Project = proj;

}

void GraphicObject_ColorTextureLight::Render()
{

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		//Color
		if (i == 0)
		{
			pShaderColor->SetToContext(context);
			pShaderColor->SendCamMatrices(View, Project);
			pModel->SetToContext(pShaderColor->GetContext());
			pShaderColor->SendWorldColor(World, ColorC);
			pModel->RenderMesh(pShaderColor->GetContext(), i);

		}

		//Texture
		else if (i == 1)
		{
			pShaderTexture->SetToContext(context);
			pShaderTexture->SendCamMatrices(View, Project);
			pModel->SetToContext(pShaderTexture->GetContext());
			pShaderTexture->SendWorld(World);
			pTexture->SetToContext(pShaderTexture->GetContext());
			pModel->RenderMesh(pShaderTexture->GetContext(), i);
		}

		//Color Light
		else if (i == 2)
		{
			pShaderColorLight->SetToContext(context);
			pShaderColorLight->SendCamMatrices(View, Project);

			pModel->SetToContext(pShaderColorLight->GetContext());

			pShaderColorLight->SetDirectionalLightParameters(Vect(-1, -1, 1).getNorm(),
				.1 * Vect(1, 1, 1), .5 * Vect(1, 1, 1), Vect(1, 1, 1));
			//*/
			//*
			pShaderColorLight->SetPointLightParameters(Vect(0, 10, 0), 100, .2 * Vect(0, 1, 0),
				.1 * Vect(1, 1, 1), 2 * Vect(1, 1, 1), 1 * Vect(1, 1, 1));

			pShaderColorLight->SetSpotLightParameters(Vect(0, 10, 0), 4000, .5 * Vect(1, 0, 0),
				Vect(0, 10, 0).getNorm(), 50,
				.2 * Vect(1, 1, 1), 3 * Vect(1, 1, 1), 1 * Vect(1, 1, 1, 150));
			Vect eyepos;
			Cam.getPos(eyepos);
			pShaderColorLight->SendLightParameters(eyepos);
			pShaderColorLight->SendWorldAndMaterial(World, ColorCL);
			pModel->RenderMesh(pShaderColorLight->GetContext(), i);
		}

		//Texture Light
		else if (i == 3)
		{
			pShaderTextureLight->SetToContext(context);
			pShaderTextureLight->SendCamMatrices(View, Project);

			pModel->SetToContext(pShaderTextureLight->GetContext());

			pShaderTextureLight->SetDirectionalLightParameters(Vect(-1, -1, 1).getNorm(),
				.5 * Vect(1, 1, 1), .5 * Vect(1, 1, 1), Vect(1, 1, 1));
			//*/
			//*
			pShaderTextureLight->SetPointLightParameters(Vect(0, 0, 0), 100, .2 * Vect(0, 1, 0),
				.5 * Vect(1, 1, 1), 2 * Vect(1, 1, 1), 1 * Vect(1, 1, 1));

			pShaderTextureLight->SetSpotLightParameters(Vect(5, -2, 0), 4000, .5 * Vect(1, 0, 0),
				Vect(5, 3, 8).getNorm(), 100,
				.2 * Vect(1, 1, 1), 3 * Vect(1, 1, 1), 1 * Vect(1, 1, 1, 150));
			Vect eyepos;
			Cam.getPos(eyepos);
			pShaderTextureLight->SendLightParameters(eyepos);

	
			pShaderTextureLight->SendWorldAndMaterial(World, Colors::Gray, 1 * Colors::Gray, Vect(.7, .7, .7, 150));
			pTexture = pTexture;
			pTexture->SetToContext(pShaderTextureLight->GetContext());
			//pShaderTextureLight->SendWorldAndMaterial(World, MeshColors[i]);
			pModel->RenderMesh(pShaderTextureLight->GetContext(), i);

		}

		else
		{
			//do nothing
		}

	}
}