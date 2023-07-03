// MeshSeperator
// Andre Berthiaume, July 2016

#ifndef _MeshSeperator
#define _MeshSeperator

#include <map>
#include <vector>
struct VertexStride_VUN;
struct TriangleIndex;



class MeshSeparator
{
private:
	struct MeshIndexData
	{
		int offset;
		int numTri;
	};

	using KEY = int;
	using MESHINDICES = std::vector<TriangleIndex>;
	
	std::map<KEY, MESHINDICES> meshes;

	MeshIndexData* meshdata;

public:
	MeshSeparator(const MeshSeparator&) = delete;				 // Copy constructor
	MeshSeparator(MeshSeparator&&) = default;                    // Move constructor
	MeshSeparator& operator=(const MeshSeparator&) & = default;  // Copy assignment operator
	MeshSeparator& operator=(MeshSeparator&&) & = default;       // Move assignment operator
	~MeshSeparator();									 // Destructor

	MeshSeparator() = delete;
	MeshSeparator(VertexStride_VUN *pVerts, int nverts, TriangleIndex*& pTriList, int ntri);

	int GetMeshCount();
	void GetMeshTriCountAndOffset(int meshnum, int& count, int& offset);

};

#endif _MeshSeperator

