#include "GraphicObject_Texture.h"
#include "Model.h"
#include "ShaderTexture.h"
#include <assert.h>

GraphicObject_Texture::GraphicObject_Texture(ShaderTexture* shader, Model* mod, Texture* pTex)
{
	SetModel(mod);
	pShader = shader;
	pTexture = pTex;

	Color = Vect(1, 1, 1);
	World = Matrix(IDENTITY);
}

GraphicObject_Texture::~GraphicObject_Texture()
{

}

void GraphicObject_Texture::SetColor(const Vect& col)
{
	Color = col;
}

void GraphicObject_Texture::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_Texture::Render()
{
	pShader->SendWorld(World);
	pModel->SetToContext(pShader->GetContext());
	pTexture->SetToContext(pShader->GetContext());
	pModel->Render(pShader->GetContext());
}