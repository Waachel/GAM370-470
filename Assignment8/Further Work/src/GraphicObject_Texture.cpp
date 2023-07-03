#include "GraphicObject_Texture.h"
#include "Model.h"
#include "ShaderTexture.h"
#include <assert.h>

GraphicObject_Texture::GraphicObject_Texture(ShaderTexture* shader, Model* mod, Texture* pTex[])
{
	SetModel(mod);
	pShader = shader;
	//pTexture = pTex;

	//Color = Vect(1, 1, 1);
	World = Matrix(IDENTITY);
	int n = mod->GetMeshCount();

	for (int i = 0; i < n; i++)
		MeshTextures[i] = pTex[i];
}

GraphicObject_Texture::~GraphicObject_Texture()
{

}

void GraphicObject_Texture::SetTexture(Texture * tex)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
		MeshTextures[i] = tex;
}

void GraphicObject_Texture::SetTexture(int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));
	pTexture = MeshTextures[meshnum];
	//MeshColors[meshnum] = col;
}

void GraphicObject_Texture::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_Texture::Render()
{

	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorld(World);
		pTexture = MeshTextures[i];
		pTexture->SetToContext(pShader->GetContext());
		//pShader->SendWorldAndMaterial(World, MeshColors[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}