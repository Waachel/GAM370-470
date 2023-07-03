#include "MeshSeparator.h"
#include <assert.h>
#include "Model.h"

// Improtant: the tri index list may be modified (reordered!)
MeshSeparator::MeshSeparator(VertexStride_VUN *pVerts, int nverts, TriangleIndex*& pTriList, int ntri)
{
	// Create the different list of indices for meshes
	nverts;
	for (int i = 0; i < ntri; i++)
	{
		TriangleIndex ind = pTriList[i];

		// triangles in one shouldn't share vertices with another mesh
		assert((pVerts[ind.v0].txt == pVerts[ind.v1].txt) && (pVerts[ind.v0].txt == pVerts[ind.v2].txt));

		KEY key = (KEY) pVerts[ind.v0].txt;
		meshes[key].push_back(ind);
	}

	// Reorder the tri index list as function of mesh number

	TriangleIndex* templist = new TriangleIndex[ntri];
	meshdata = new MeshIndexData[meshes.size()];
	int offsetval = 0;

	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshdata[i].numTri = meshes[i].size();
		meshdata[i].offset = offsetval;

		for (size_t j = 0; j < meshes[i].size(); j++)
		{
			templist[offsetval + j] = meshes[i][j];
		}
		
		offsetval += meshes[i].size();
	}

	delete[] pTriList;		// Delete the original list
	pTriList = templist;	// Replace with new re-ordered one

}

MeshSeparator::~MeshSeparator()
{
	delete[] meshdata;
}

void MeshSeparator::GetMeshTriCountAndOffset(int meshnum, int& count, int& offset)
{
	assert((meshnum >= 0) && (meshnum < GetMeshCount()));

	count = meshdata[meshnum].numTri;
	offset = meshdata[meshnum].offset;
}

int MeshSeparator::GetMeshCount()
{
	return meshes.size();
}

