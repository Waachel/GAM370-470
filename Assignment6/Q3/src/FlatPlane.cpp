#include "FlatPlane.h"
#include "Model.h"
#include "d3dUtil.h"

FlatPlane::FlatPlane(ID3D11Device* dev, float size, float hrep, float vrep)
{
	int nverts = 4;
	StandardVertex* pVerts = new StandardVertex[nverts];

	int ntri = 2;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	float c = size / 2;

	Vect tmpVect1;
	Vect tmpVect2;
	Vect normals;

	float subtractX = -c - (-c);
	float subtractY = 0 - 0;
	float subtractZ = c - (-c);

	tmpVect1.set(subtractX, subtractY, subtractZ);

	subtractX = c - (-c);
	subtractY = 0 - 0;
	subtractZ = c - (-c);

	tmpVect2.set(subtractX, subtractY, subtractZ);

	normals.set(tmpVect1.Y() * tmpVect2.Z() - tmpVect1.Z() * tmpVect2.Y(), tmpVect1.Z() * tmpVect2.X() - tmpVect1.X() * tmpVect2.Z(), tmpVect1.X() * tmpVect2.Y() - tmpVect1.Y() * tmpVect2.X());

	pVerts[0].set(-c, 0, -c, hrep, vrep, normals.X(), normals.Y(), normals.Z());
	pVerts[1].set(-c, 0,  c, hrep, 0, normals.X(), normals.Y(), normals.Z());
	pVerts[2].set( c, 0,  c, 0, 0, normals.X(), normals.Y(), normals.Z());
	pVerts[3].set( c, 0, -c, 0, vrep, normals.X(), normals.Y(), normals.Z());
	
	pTriList[0].set(0, 1, 2);
	pTriList[1].set(0, 2, 3);

	pPlane = new Model(dev, pVerts, nverts, pTriList, ntri);

	delete[] pVerts;
	delete[] pTriList;
}

FlatPlane::~FlatPlane()
{
	delete pPlane;
}

void FlatPlane::Render(ID3D11DeviceContext* context)
{
	pPlane->SetToContext(context);
	pPlane->Render(context);
}