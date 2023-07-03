#include "TerrainModel.h"
#include "Model.h"
#include "d3dUtil.h"
#include "DirectXTex.h"
#include <assert.h>


TerrainModel::TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV)
{
	DirectX::ScratchImage scrtTex;
	HRESULT hr = LoadFromTGAFile(heightmapFile, nullptr, scrtTex);
	assert(SUCCEEDED(hr));

	const DirectX::Image* hgtmap = scrtTex.GetImage(0, 0, 0);
	assert(hgtmap->height == hgtmap->width);

	size_t side = hgtmap->height;
	uint8_t h_val = hgtmap->pixels[(int)TexelIndex(side, 3, 5)];

	int nverts = side * side;
	StandardVertex* pVerts = new StandardVertex[nverts];

	int ntri = ((side - 1) * (side - 1)) * 2;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	int vind = 0;
	int tind = 0;

	float repU = RepeatU / (float)(side - 1);
	float repV = RepeatV / (float)(side - 1);

	float origin = (-len + ytrans) / 2.0f;
	float posX = -45;
	float posZ = 0;
	float posY = 0;

	float width = len / (float)(side - 1);

	for (int x = 0; x < (int)side; x++)
	{
		posZ = origin;
		for (int z = 0; z < (int)side; z++)
		{
			h_val = hgtmap->pixels[TexelIndex(side, x, z)];
			posY = h_val / 255.0f * maxheight;

			if (vind == nverts)
			{
				break;
			}
			pVerts[vind].set(posX, posY, posZ, x * repU, z * repV);
			vind++;
			posZ += width;
		}
		posX += width;
	}

	vind = (int)side;
	for (int x = 1; x < (int)side - 1; x++)
	{
		vind++;
		for(int z = 1; z < (int)side -1; z++)
		{
			if (vind < nverts)
			{
				Vect tmpNormals = getFaceNormal(pVerts[vind].Pos, pVerts[vind - 1].Pos, pVerts[vind - side - 1].Pos, pVerts[vind - side].Pos, pVerts[vind - side + 1].Pos, pVerts[vind + 1].Pos, pVerts[vind + side + 1].Pos, pVerts[vind + side].Pos, pVerts[vind + side - 1].Pos);
				pVerts[vind].normal.set(tmpNormals.X(), tmpNormals.Y(), tmpNormals.Z());
				vind++;
			}
		}
		vind++;
	}


	vind = 0;
	tind = 0;

	for (int j = 0; j < (int)side - 1; j++)
	{
		for (int k = 0; k < (int)side - 1; k++)
		{
			if (tind == ntri)
			{
				break;
			}
			pTriList[tind].set(vind, vind + 1, vind + side + 1);
			tind++;
			pTriList[tind].set(vind, vind + side + 1, vind + side);
			tind++;

			vind++;
		}
		vind++;
	}

	pModTerrain = new Model(dev, pVerts, nverts, pTriList, ntri);

	pWorldT = Matrix(SCALE, 1, 1, 1) * Matrix(TRANS, 0.0f, ytrans -7, 0.0f);

	delete[] pVerts;
	delete[] pTriList;
}

int TerrainModel::TexelIndex(int side, int i, int j) const
{
	return pixel_width * (j * side + i);
}


TerrainModel::~TerrainModel()
{
	delete pModTerrain;
}

void TerrainModel::Render(ID3D11DeviceContext* context)
{
	pModTerrain->SetToContext(context);
	pModTerrain->Render(context);
}
														//v2 - v1    v3 - v1
Vect TerrainModel::getFaceNormal(Vect p, Vect up, Vect ul, Vect lf, Vect ld, Vect dn, Vect dr, Vect rt, Vect ru)
{
	return ((up - p).cross(ul - p) + (ul - p).cross(lf - p) + (lf - p).cross(ld - p) + (ld - p).cross(dn - p) + (dn - p).cross(dr - p) + (dr - p).cross(rt - p) + (rt - p).cross(ru - p)) * 0.125f;
}

Matrix TerrainModel::getWorldMatrix()
{
	return pWorldT;
}