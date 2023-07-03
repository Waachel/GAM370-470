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

/// Creates the unit box centered at the origin
void ModelTools::CreateUnitPyramid(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 5;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	pVerts[0].set(0.0f, 1.0f, 0.0f, Colors::Black);
	pVerts[1].set(-1.0f, -1.0f, -1.0f, Colors::Lime);
	pVerts[2].set(+1.0f, -1.0f, -1.0f, Colors::Yellow);
	pVerts[3].set(+1.0f, -1.0f, +1.0f, Colors::Red);
	pVerts[4].set(-1.0f, -1.0f, +1.0f, Colors::Blue);

	// back face
	pTriList[0].set(0, 2, 1);

	// front face
	pTriList[1].set(0, 4, 3);

	// left face
	pTriList[2].set(0, 3, 2);

	// right face
	pTriList[3].set(0, 1, 4);

	// bottom face
	pTriList[4].set(1, 2, 3);
	pTriList[5].set(1, 3, 4);

}

void ModelTools::CreateUnitSphere(int vslice, int hslice, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 2 + (hslice - 1) * vslice;
	pVerts = new StandardVertex[nverts];
	ntri = vslice * (2 * hslice - 2);
	pTriList = new TriangleByIndex[ntri];

	float pi = acosf(-1);
	float vAngle = pi / vslice;
	float hAngle = 2 * pi / hslice;

	pVerts[0].set(0.0f, 1.0f, 0.0f, Colors::Magenta);

	for (int i = 0; i < hslice - 1; i++)
	{
		for (int j = 0; j < vslice; j++)
		{
			pVerts[j + i * vslice + 1].set(sinf((i + 1) * vAngle) * cosf(hAngle * j), cosf((i + 1) * vAngle), sinf((i + 1) * vAngle) * sinf(hAngle * j), Colors::Magenta);
		}
	}

	pVerts[nverts - 1].set(0.0f, -1.0f, 0.0f, Colors::Magenta);

	for (int i = 0; i < hslice - 1; i++)
	{
		for (int j = 0; j < vslice; j++)
		{
			int vert_number = i * vslice + j;

			if (i == 0)
			{
				if (j == vslice - 1)
				{
					pTriList[vert_number].set(vert_number + 1, i * vslice + 1, 0);
				}

				else
				{
					pTriList[vert_number].set(vert_number + 1, vert_number + 2, 0);
				}
			}
			else
			{
				if (j == vslice - 1)
				{
					pTriList[vert_number].set(vert_number + 1, i * vslice + 1, vert_number + 1 - vslice);
				}

				else
				{
					pTriList[vert_number].set(vert_number + 1, vert_number + 2, vert_number + 1 - vslice);
				}
			}

			if (i == hslice - 2)
			{
				if (j == vslice - 1)
				{
					pTriList[vert_number].set(vert_number + 1, nverts - 1, i * vslice + 1);
				}

				else
				{
					pTriList[vert_number + ntri / 2].set(vert_number + 1, nverts - 1, vert_number + 2);
				}
			}
			else
			{
				if (j == vslice - 1)
				{
					pTriList[vert_number].set(vert_number + 1, i * vslice + 1 + vslice, i * vslice + 1);
				}

				else
				{
					pTriList[vert_number + ntri / 2].set(vert_number + 1, vert_number + 2 + vslice, vert_number + 2);
				}
			}
		}
	}


}


