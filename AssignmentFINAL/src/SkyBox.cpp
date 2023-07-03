#include "SkyBox.h"
#include "Model.h"
#include "d3dUtil.h"

SkyBox::SkyBox(ID3D11Device* dev, float len)
{
	int nverts = 24;
	StandardVertex* pVerts = new StandardVertex[nverts];

	int ntri = 12;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f * len, 0.5f * len, 0.5f * len, 1, 0.334, Colors::Cyan);			//top right
	pVerts[vind + 1].set(-0.5f * len, 0.5f * len, 0.5f * len, 0.75, 0.334, Colors::Cyan);		//top left
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, 0.5f * len, 0.75, 0.666, Colors::Cyan);			//bottom left
	pVerts[vind + 3].set(0.5f * len, -0.5f * len, 0.5f * len, 1, 0.666, Colors::Cyan);			//bottom right
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * len, 0.5f * len, -0.5f * len, 0.25, 0.334, Colors::Cyan);			//top left
	pVerts[vind + 1].set(-0.5f * len, 0.5f * len, -0.5f * len, 0.5, 0.334, Colors::Cyan);		//top right
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.5, 0.666, Colors::Cyan);		//bottom right
	pVerts[vind + 3].set(0.5f * len, -0.5f * len, -0.5f * len, 0.25, 0.666, Colors::Cyan);		//bottom left
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * len, 0.5f * len, -0.5f * len, 0.25, 0.334, Colors::Cyan);					//top right
	pVerts[vind + 1].set(0.5f * len, 0.5f * len, 0.5f * len, 0, 0.334, Colors::Cyan);				//top left
	pVerts[vind + 2].set(0.5f * len, -0.5f * len, 0.5f * len, 0, 0.666, Colors::Cyan);				//bottom left
	pVerts[vind + 3].set(0.5f * len, -0.5f * len, -0.5f * len, 0.25, 0.666, Colors::Cyan);				//bottom right
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f * len, 0.5f * len, 0.5f * len, 0.75, 0.334, Colors::Cyan);				//top right
	pVerts[vind + 1].set(-0.5f * len, 0.5f * len, -0.5f * len, 0.5, 0.334, Colors::Cyan);			//top left
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.5, 0.666, Colors::Cyan);			//bottom left
	pVerts[vind + 3].set(-0.5f * len, -0.5f * len, 0.5f * len, 0.75, 0.666, Colors::Cyan);			//bottom right
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * len, 0.5f * len, -0.5f * len, 1.0, 0.001, Colors::Cyan);			//bottom left
	pVerts[vind + 1].set(-0.5f * len, 0.5f * len, -0.5f * len, 0.75, 0.001, Colors::Cyan);	//bottom right
	pVerts[vind + 2].set(-0.5f * len, 0.5f * len, 0.5f * len, 0.75, 0.334, Colors::Cyan);			// top right
	pVerts[vind + 3].set(0.5f * len, 0.5f * len, 0.5f * len, 1.0, 0.334, Colors::Cyan);			//top left
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * len, -0.5f * len, 0.5f * len, 1.0, 0.666, Colors::Cyan);				  //top right
	pVerts[vind + 1].set(-0.5f * len, -0.5f * len, 0.5f * len, 0.75, 0.666, Colors::Cyan);			  //top left
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.75, 1, Colors::Cyan);		  //bottom left
	pVerts[vind + 3].set(0.5f * len, -0.5f * len, -0.5f * len, 1, 1, Colors::Cyan);			  //bottom right
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	pSkyBox = new Model(dev, pVerts, nverts, pTriList, ntri);

	delete[] pVerts;
	delete[] pTriList;
}

SkyBox::~SkyBox()
{
	delete pSkyBox;
}

void SkyBox::Render(ID3D11DeviceContext* context)
{
	pSkyBox->SetToContext(context);
	pSkyBox->Render(context);
}