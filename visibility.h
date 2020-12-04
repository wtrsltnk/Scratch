#ifndef _VISIBILITY_H
#define	_VISIBILITY_H

#include "hl1types.h"

typedef struct sVISLeaf
{
	int leafCount;
	int* leafs;

} tVISLeaf;


class Visibility
{
public:
	Visibility();
	virtual ~Visibility();

	bool Decompress(int leafCount, HL1::tBSPLeaf* leafs, HL1::tBSPModel* models, unsigned char* visdata);
	
	int m_leafCount;
	tVISLeaf* m_vis;

};

#endif	/* _VISIBILITY_H */

