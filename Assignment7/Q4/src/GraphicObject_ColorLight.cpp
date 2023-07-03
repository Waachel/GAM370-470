#include "GraphicObject_ColorLight.h"
#include "Model.h"
#include "ShaderColorLight.h"
#include <assert.h>
#include "d3dUtil.h"



GraphicObject_ColorLight::GraphicObject_ColorLight(ShaderColorLight* shader,  Model* mod, Camera mCam)
{
	SetModel(mod);
	pShader = shader;
	Cam = mCam;
	//Color = Vect(1, 1, 1);
	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	MeshColors = new Vect[n];
	for (int i = 0; i < n; i++)
		MeshColors[i] = Vect(0, 0, 0);
}

GraphicObject_ColorLight::~GraphicObject_ColorLight()
{
	delete[] MeshColors;
}

void GraphicObject_ColorLight::SetColor(const Vect& col)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
		MeshColors[i] = col;
}

void GraphicObject_ColorLight::SetColor(const Vect& col, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));
	MeshColors[meshnum] = col;
}

void GraphicObject_ColorLight::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_ColorLight::Render()
{

	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SetDirectionalLightParameters(Vect(-1, -1, 1).getNorm(),
			.1 * Vect(1, 1, 1), .5 * Vect(1, 1, 1), Vect(1, 1, 1));
		//*/
		//*
		pShader->SetPointLightParameters(Vect(0, 10, 0), 100, .2 * Vect(0, 1, 0),
			.1 * Vect(1, 1, 1), 2 * Vect(1, 1, 1), 1 * Vect(1, 1, 1));

		pShader->SetSpotLightParameters(Vect(0, 10, 0), 4000, .5 * Vect(1, 0, 0),
			Vect(0, 10, 0).getNorm(), 50,
			.2 * Vect(1, 1, 1), 3 * Vect(1, 1, 1), 1 * Vect(1, 1, 1, 150));
		Vect eyepos;
		Cam.getPos(eyepos);
		pShader->SendLightParameters(eyepos);
		pShader->SendWorldAndMaterial(World, MeshColors[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}