#ifndef _BSPDATA_H
#define	_BSPDATA_H

#include "hl1types.h"
#include <stdlib.h>

class Hl1BspData
{
public:
    Hl1BspData(const char* filename);
    virtual ~Hl1BspData();

	bool isLoaded();
	void printStats();
    static bool testBSP(unsigned char* data, int size);
	
	HL1::tBSPMipTexHeader* getMiptex(int index);

	int dataSize;
	unsigned char* data;

    HL1::tBSPHeader* header;

    int entitySize;
    unsigned char* entityData;

    int planeCount;
    HL1::tBSPPlane* planes;

    int textureSize;
    unsigned char* textureData;

    int vertexCount;
    HL1::tBSPVertex* vertices;

    int visibilitySize;
    unsigned char* visibilityData;

    int nodeCount;
    HL1::tBSPNode* nodes;

    int texinfoCount;
    HL1::tBSPTexInfo* texinfos;

    int faceCount;
    HL1::tBSPFace* faces;

    int lightingSize;
    unsigned char* lightingData;

    int clipnodeCount;
    HL1::tBSPClipNode* clipnodes;

    int leafCount;
    HL1::tBSPLeaf* leafs;

    int marksurfaceCount;
    unsigned short* marksurfaces;

    int edgeCount;
    HL1::tBSPEdge* edges;

    int surfedgeCount;
    int* surfedges;

    int modelCount;
    HL1::tBSPModel* models;

private:
    template <typename T>
    static int loadLump(T** array, const HL1::tBSPLump& lump, unsigned char* data, int size)
    {
        if (size < (lump.offset + lump.size))
            return 0;

        int count = lump.size / sizeof(T);
        if (count > 0)
        {
			*array = (T*)(data + lump.offset);
        }
        return count;
    }
};

#endif	/* _BSPDATA_H */

