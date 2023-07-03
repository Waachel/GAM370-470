#include "GraphicObject_TextureLight.h"
#include "Model.h"
#include "ShaderTextureLight.h"
#include <assert.h>
#include <d3dUtil.h>

GraphicObject_TextureLight::GraphicObject_TextureLight(ShaderColorMultiLightTexture* shader,  Model* mod, Camera mCam, Texture* pTex[])
{
	SetModel(mod );
	pShader = shader;
	Cam = mCam;
	//pTexture = pTex;
	Color = Vect(1, 1, 1);
	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	for (int i = 0; i < n; i++)
		MeshTextures[i] = pTex[i];
}

GraphicObject_TextureLight::~GraphicObject_TextureLight()
{

}

void GraphicObject_TextureLight::SetColor(const Vect& col) 
{
	Color = col;
}

void GraphicObject_TextureLight::SetWorld(const Matrix& m) 
{ 
	World = m;
}

void GraphicObject_TextureLight::SetPointLight(const Vect& m)
{
	matrixPL = m;
}

void GraphicObject_TextureLight::SetTime(const float& m)
{
	time = m;
}

void GraphicObject_TextureLight::Render()
{
//	pShader->SetDirectionalLightParameters(Vect(-1, -1, 1).getNorm(),
		//.5 * Vect(1, 1, 1), .5 * Vect(1, 1, 1), Vect(1, 1, 1));

	//mWorld7 = Matrix(SCALE, 10, 10, 10) * Matrix(TRANS, 35, 10, -230);
	//mWorld75 = Matrix(SCALE, 10, 10, 10) * Matrix(TRANS, 35, 10, -230) * Matrix(TRANS, 0, 2 * cosf(mTimer.TotalTime()), 0);
	//pos75 = mWorld75.get(MatrixRowType::ROW_3);
	//pShader->SetPointLightParameters(matrixPL, 10, .2 * Vect(0, 1, 0),
		//(2 * cosf(time) + 3) * Vect(1, 1, 1), 2 * Vect(1, 1, 1), 1 * Vect(1, 1, 1));
	//*/
	//*
	//pShader->SetPointLightParameters(Vect(35, 33 * cosf(mTimer.TotalTime()), -230), 100, .2 * Vect(0, 1, 0),
		//.5 * Vect(1, 1, 1), 2 * Vect(1, 1, 1), 1 * Vect(1, 1, 1));

	//pShader->SetSpotLightParameters(Vect(5, -2, 0), 4000, .5 * Vect(1, 0, 0),
		//Vect(5, 3, 8).getNorm(), 100,
		//.2 * Vect(1, 1, 1), 3 * Vect(1, 1, 1), 1 * Vect(1, 1, 1, 150));


	Vect eyepos;
	Cam.getPos(eyepos);
	//pShader->SendLightParameters(eyepos);

	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndMaterial(World, Colors::Gray, 1 * Colors::Gray, Vect(.7, .7, .7, 150));
		pTexture = MeshTextures[i];
		pTexture->SetToContext(pShader->GetContext());
		//pShader->SendWorldAndMaterial(World, MeshColors[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}