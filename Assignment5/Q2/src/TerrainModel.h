#ifndef _TerrainModel
#define _TerrainModel

#include "Matrix.h"
#include <tuple>
#include <d3d11.h>
#include "DirectXTex.h"

class Model;
struct ID3D11DeviceContext;
struct ID3D11Device;

class TerrainModel : public Align16
{

private:
	Model* pModTerrain;
	size_t pixel_width = 4;			// 4 bytes RGBA per pixel
	float xPixel, yPixel, xValue, zValue, uValue, vValue;

public:
	TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV);
	~TerrainModel();
	int TexelIndex(int side, int i, int j) const;
	void Render(ID3D11DeviceContext* context);

};



#endif _TerrainModel