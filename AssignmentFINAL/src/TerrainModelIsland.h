#ifndef _TerrainModelIsland
#define _TerrainModelIsland

#include "Matrix.h"
#include <tuple>
#include <d3d11.h>
#include "DirectXTex.h"

class Model;
struct ID3D11DeviceContext;
struct ID3D11Device;

class TerrainModelIsland : public Align16
{

private:
	Model* pModTerrain;
	size_t pixel_width = 4;			// 4 bytes RGBA per pixel
	float xPixel, yPixel, xValue, zValue, uValue, vValue;
	Matrix pWorldT;
public:
	TerrainModelIsland(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV);
	~TerrainModelIsland();
	int TexelIndex(int side, int i, int j) const;
	void Render(ID3D11DeviceContext* context);
	Matrix getWorldMatrix();
	Vect getFaceNormal(Vect p, Vect up, Vect ul, Vect lf, Vect ld, Vect dn, Vect dr, Vect rt, Vect ru);

};



#endif _TerrainModelIsland