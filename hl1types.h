#ifndef _BSPTYPES_H
#define	_BSPTYPES_H

#define HL1_BSP_SIGNATURE 30
#define HL1_BSP_LUMPCOUNT 15

#define HL1_BSP_ENTITYLUMP 0
#define HL1_BSP_PLANELUMP 1
#define HL1_BSP_TEXTURELUMP 2
#define HL1_BSP_VERTEXLUMP 3
#define HL1_BSP_VISIBILITYLUMP 4
#define HL1_BSP_NODELUMP 5
#define HL1_BSP_TEXINFOLUMP 6
#define HL1_BSP_FACELUMP 7
#define HL1_BSP_LIGHTINGLUMP 8
#define HL1_BSP_CLIPNODELUMP 9
#define HL1_BSP_LEAFLUMP 10
#define HL1_BSP_MARKSURFACELUMP 11
#define HL1_BSP_EDGELUMP 12
#define HL1_BSP_SURFEDGELUMP 13
#define HL1_BSP_MODELLUMP 14

#define MAX_MAP_HULLS 4
#define MAX_LIGHT_MAPS 4
#define MAX_AMBIENTS 4

// Plane types (0-2 are axial planes, 3-5 are non-axial planes snapped to the nearest)
#define PLANE_X 0
#define PLANE_Y 1
#define PLANE_Z 2
#define PLANE_ANYX 3
#define PLANE_ANYY 4
#define PLANE_ANYZ 5

namespace HL1
{

    #pragma pack(push, 4)

	/* BSP */
	typedef struct sBSPLump
	{
		int offset;
		int size;

	} tBSPLump;

	typedef struct sBSPHeader
	{
		int signature;
		tBSPLump lumps[HL1_BSP_LUMPCOUNT];

	} tBSPHeader;

	typedef struct sBSPMipTexOffsetTable
	{
		int miptexCount;
		int offsets[1];             /* an array with "miptexcount" number of offsets */

	} tBSPMipTexOffsetTable;

	typedef struct sBSPMipTexHeader
	{
		char name[16];
		unsigned int width;
		unsigned int height;
		unsigned int offsets[4];

	} tBSPMipTexHeader;


	typedef struct sBSPModel
	{
		float mins[3], maxs[3];
		float origin[3];
		int headnode[MAX_MAP_HULLS];
		int visLeafs;                       // not including the solid leaf 0
		int firstFace;
		int faceCount;

	} tBSPModel;

	typedef struct sBSPVertex
	{
		float point[3];

	} tBSPVertex;

	typedef struct sBSPEdge
	{
		unsigned short vertex[2];

	} tBSPEdge;

	typedef struct sBSPFace
	{
		short planeIndex;
		short side;
		int firstEdge;
		short edgeCount;
		short texinfo;

		// lighting info
		unsigned char styles[MAX_LIGHT_MAPS];
		int lightOffset;                    // start of [numstyles*surfsize] samples

	} tBSPFace;

	typedef struct sBSPPlane
	{
		float normal[3];
		float distance;
		int type;

	} tBSPPlane;

	typedef struct sBSPNode
	{
		int planeIndex;
		short children[2];                  // negative numbers are -(leafs+1), not nodes
		short mins[3];                      // for sphere culling
		short maxs[3];
		unsigned short firstFace;
		unsigned short faceCount;            // counting both sides

	} tBSPNode;

	typedef struct sBSPClipNode
	{
		int planeIndex;
		short children[2];                  // negative numbers are contents

	} tBSPClipNode;

	typedef struct sBSPTexInfo
	{
		float    vecs[2][4];                // [s/t][xyz offset]
		int      miptexIndex;
		int      flags;
	} tBSPTexInfo;

	typedef struct sBSPLeaf
	{
		int contents;
		int visofs;                         // -1 = no visibility info

		short mins[3];                      // for frustum culling
		short maxs[3];

		unsigned short firstMarkSurface;
		unsigned short markSurfacesCount;

		unsigned char ambientLevel[MAX_AMBIENTS];

	} tBSPLeaf;

#pragma pack(pop)

}

#endif	/* _BSPTYPES_H */

