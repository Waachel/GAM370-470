#include "ModelTools.h"
//#include "Enum.h"
//#include <math.h>
#include "Matrix.h"
#include <assert.h>
#include "Model.h"
#include "d3dUtil.h"


/// Creates the unit box centered at the origin
void ModelTools::CreateUnitBox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri)
{
	nverts = 8;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];

	pVerts[0].set(-0.5f, -0.5f, -0.5f, Colors::Black);
	pVerts[1].set(-0.5f, +0.5f, -0.5f, Colors::Lime);
	pVerts[2].set(+0.5f, +0.5f, -0.5f, Colors::Yellow);
	pVerts[3].set(+0.5f, -0.5f, -0.5f, Colors::Red );
	pVerts[4].set(-0.5f, -0.5f, +0.5f, Colors::Blue);
	pVerts[5].set(-0.5f, +0.5f, +0.5f, Colors::Cyan);
	pVerts[6].set(+0.5f, +0.5f, +0.5f, Colors::White);
	pVerts[7].set(+0.5f, -0.5f, +0.5f, Colors::Magenta);

	// back face
	pTriList[0].set(0, 1, 2);
	pTriList[1].set(0, 2, 3);

	// front face
	pTriList[2].set(4, 6, 5);
	pTriList[3].set(4, 7, 6);

	// left face
	pTriList[4].set(4, 5, 1);
	pTriList[5].set(4, 1, 0);

	// right face
	pTriList[6].set(3, 2, 6);
	pTriList[7].set(3, 6, 7);

	// top face
	pTriList[8].set(1, 5, 6);
	pTriList[9].set(1, 6, 2);

	// bottom face
	pTriList[10].set(4, 0, 3);
	pTriList[11].set(4, 3, 7);
}

void ModelTools::CreateUnitBoxRepTexture(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 1 );
	pVerts[vind + 1].set(-0.5f, 0.5f, 0.5f, 0, 0, 0, 0, 1);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 0, 1, 0, 0, 1);
	pVerts[vind + 3].set(0.5f, -0.5f, 0.5f, 1, 1, 0, 0, 1);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0, 0, 0, 0, -1);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 1, 0, 0, 0, -1);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 1, 1, 0, 0, -1);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0, 1, 0, 0, -1);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 1, 0, 0);
	pVerts[vind + 1].set(0.5f, 0.5f, 0.5f, 0, 0, 1, 0, 0);
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, 0, 1, 1, 0, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f, 0.5f, 0.5f, 1, 0, -1, 0, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, -1, 0, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, -1, 0, 0);
	pVerts[vind + 3].set(-0.5f, -0.5f, 0.5f, 1, 1, -1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 0, 1, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, 0, 1, 0);
	pVerts[vind + 2].set(-0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 0);
	pVerts[vind + 3].set(0.5f, 0.5f, 0.5f, 1, 1, 0, 1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 1, 0, 0, -1, 0);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 0, 0, 0, -1, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, 0, -1, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 0, -1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitBoxSixFacesTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f, 0.5f, 0.5f, 0.25, 0.625);			//top right
	pVerts[vind + 1].set(-0.5f, 0.5f, 0.5f, 0.25, 0.385);		//top left
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 0, 0.385);			//bottom left
	pVerts[vind + 3].set(0.5f, -0.5f, 0.5f, 0, 0.625);			//bottom right
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0.51, 0.625);			//top left
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0.51, 0.385);		//top right
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0.75, 0.385);		//bottom right
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0.75, 0.625);		//bottom left
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0.51, 0.625);					//top right
	pVerts[vind + 1].set(0.5f, 0.5f, 0.5f, 0.26, 0.625);				//top left
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, 0.26, 0.875);				//bottom left
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0.51, 0.875);				//bottom right
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f, 0.5f, 0.5f, 0.26, 0.375);				//top right
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0.51, 0.375);			//top left
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0.51, 0.125);			//bottom left
	pVerts[vind + 3].set(-0.5f, -0.5f, 0.5f, 0.26, 0.125);			//bottom right
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0.5, 0.625);				//top right
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0.5, 0.375);			//top left
	pVerts[vind + 2].set(-0.5f, 0.5f, 0.5f, 0.25, 0.375);			//bottom left
	pVerts[vind + 3].set(0.5f, 0.5f, 0.5f, 0.25, 0.625);			//bottom right
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 1, 0.625);				  //top right
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 1, 0.375);			  //top left
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0.75, 0.375);		  //bottom left
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0.75, 0.625);			  //bottom right
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitPyramid(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 16;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	//back face
	pVerts[0].set(-1.0f, -1.0f, -1.0f, 0, 0, 0.0f, -2.0f, -4.0f); //1
	pVerts[1].set(0.0f, 1.0f, 0.0f, 0, 0, 0.0f, -2.0f, -4.0f); //0
	pVerts[2].set(+1.0f, -1.0f, -1.0f, 0, 0, 0.0f, -2.0f, -4.0f); //2

	//left face
	pVerts[3].set(-1.0f, -1.0f, +1.0f, 0, 0, -4.0f, -2.0f, 0.0f); //4
	pVerts[4].set(0.0f, 1.0f, 0.0f, 0, 0, -4.0f, -2.0f, 0.0f); //0
	pVerts[5].set(-1.0f, -1.0f, -1.0f, 0, 0, -4.0f, -2.0f, 0.0f); //1

	//right face
	pVerts[6].set(+1.0f, -1.0f, -1.0f, 0, 0, 4.0f, -2.0f, 0.0f); //2
	pVerts[7].set(0.0f, 1.0f, 0.0f, 0, 0, 4.0f, -2.0f, 0.0f); //0
	pVerts[8].set(+1.0f, -1.0f, +1.0f, 0, 0, 4.0f, -2.0f, 0.0f); //3

	//front face
	pVerts[9].set(+1.0f, -1.0f, +1.0f, 0, 0, 0.0f, -2.0f, 4.0f); //3
	pVerts[10].set(0.0f, 1.0f, 0.0f, 0, 0, 0.0f, -2.0f, 4.0f); //0
	pVerts[11].set(-1.0f, -1.0f, +1.0f, 0, 0, 0.0f, -2.0f, 4.0f); //4

	//bottom face
	pVerts[12].set(-1.0f, -1.0f, -1.0f, 0, 0, 0.0f, -4.0f, 0.0f); //0
	pVerts[13].set(+1.0f, -1.0f, -1.0f, 0, 0, 0.0f, -4.0f, 0.0f); //4
	pVerts[14].set(+1.0f, -1.0f, +1.0f, 0, 0, 0.0f, -4.0f, 0.0f); //1
	pVerts[15].set(-1.0f, -1.0f, +1.0f, 0, 0, 0.0f, -4.0f, 0.0f); //2

	//12, 13, 14

	pTriList[0].set(1, 2, 0);

	//front
	pTriList[1].set(10, 11, 9);

	// left face
	pTriList[2].set(7, 8, 6);

	// right face
	pTriList[3].set(4, 5, 3);

	// bottom face
	pTriList[4].set(12, 13, 14); //14, 15, 13
	pTriList[5].set(12, 14, 15); //14, 13, 12

}

void ModelTools::CreateUnitPyramidRepTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 18;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	// Setting up faces
	// Front
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(-0.5f, -0.5f, 0.5f, 0, 1);
	pVerts[vind + 1].set(0.5f, -0.5f, 0.5f, 1, 1);
	pVerts[vind + 2].set(0.0f, 0.5f, 0.0f, 0.5, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);

	// Back
	vind += 3;
	tind += 1;
	pVerts[vind].set(0.5f, -0.5f, -0.5f, 0, 1); //bottom left
	pVerts[vind + 1].set(-0.5f, -0.5f, -0.5f, 1, 1); //bottom right
	pVerts[vind + 2].set(0.0f, 0.5f, 0.0f, 0.5, 0); //top
	pTriList[tind].set(vind, vind + 1, vind + 2);

	// Left
	vind += 3;
	tind += 1;
	pVerts[vind].set(-0.5f, -0.5f, -0.5f, 0, 1);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 1, 1);
	pVerts[vind + 2].set(0.0f, 0.5f, 0.0f, 0.5, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);

	// Right
	vind += 3;
	tind += 1;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 0, 1);
	pVerts[vind + 1].set(0.5f, -0.5f, -0.5f, 1, 1);
	pVerts[vind + 2].set(0.0f, 0.5f, 0.0f, 0.5, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);

	//Bottom
	vind += 3;
	tind += 1;
	pVerts[vind].set(-0.5f, -0.5f, -0.5f, 1, 0);
	pVerts[vind + 1].set(0.5f, -0.5f, 0.5f, 0, 1);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 1, 1);
	pTriList[tind].set(vind, vind + 1, vind + 2);

	vind += 3;
	tind += 1;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 0, 1);
	pVerts[vind + 1].set(-0.5f, -0.5f, -0.5f, 1, 0);
	pVerts[vind + 2].set(0.5f, -0.5f, -0.5f, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);

}


void ModelTools::CreateUnitSphere(int vslice, int hslice, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{

	nverts = vslice * hslice + 2;
	pVerts = new StandardVertex[nverts];
	ntri = vslice * hslice * 8;
	pTriList = new TriangleByIndex[ntri];

	int vind = 0;
	int tind = 0;

	Vect tmpVect;
	Matrix rot(ROT_Y, MATH_PI * 2 / (float)vslice);

	pVerts[0].set(0.0f, 1.0f, 0.0f, 0, 0, 0.0f, 1.0f, 0.0f);
	vind++;

	for (int i = 1; i < hslice + 1; i++)
	{
		rot = Matrix(ROT_Z, MATH_PI * i / (float)(hslice + 1));
		tmpVect.set(0.0f, 1.0f, 0.0f);
		tmpVect = tmpVect * rot;

		if (vind != nverts)
		{
			pVerts[vind].set(tmpVect.X(), tmpVect.Y(), tmpVect.Z(), 0, 0, tmpVect.X(), tmpVect.Y(), tmpVect.Z());
			vind++;
		}
	}

	for (int i = 1; i < vslice; i++)
	{
		rot = Matrix(ROT_Y, MATH_PI * 2 * i / (float)vslice);
		for (int j = 1; j < hslice + 1; j++)
		{
			if (j == nverts)
			{
				break;
			}

			tmpVect.set(pVerts[j].Pos);
			tmpVect = tmpVect * rot;
			pVerts[vind].set(tmpVect.X(), tmpVect.Y(), tmpVect.Z(), 0, 0, tmpVect.X(), tmpVect.Y(), tmpVect.Z());
			vind++;
		}
	}

	if (vind != nverts)
	{
		pVerts[vind].set(0.0f, -1.0f, 0.0f, 0, 0, 0.0f, -1.0f, 0.0f);
	}



	int i, j, k;

	for (i = 0; i < vslice - 1; i++)
	{
		j = 1 + i * hslice;
		k = j + hslice - 1;

		pTriList[tind].set(0, j, j + hslice);
		tind++;
		while (j < k)
		{
			pTriList[tind].set(j, j + 1, j + 1 + hslice);
			tind++;
			pTriList[tind].set(j, j + 1 + hslice, j + hslice);
			tind++;

			j++;
		}
		pTriList[tind].set(j, nverts - 1, j + hslice);
		tind++;
	}
	if (tind != ntri)
	{
		pTriList[tind].set(0, (vslice - 1) * hslice + 1, 1);
		tind++;
	}

	k = (1 + i * hslice) + hslice - 1;
	int t = 1;

	for (j = 1 + i * hslice; j < k; j++)
	{
		if (tind != ntri && tind + 1 != ntri)
		{
			pTriList[tind].set(j, j + 1, t);
			tind++;
			pTriList[tind].set(j + 1, t + 1, t);
			tind++;
			t++;
		}
	}

	if (tind != ntri)
	{
		pTriList[tind].set(j, nverts - 1, t);
	}
}
