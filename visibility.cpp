#include "visibility.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <vector>

using namespace std;

Visibility::Visibility()
{
	this->m_vis = 0;
}

Visibility::~Visibility()
{
	printf("Cleaning visibility data\n");
	if (this->m_vis != 0) delete []this->m_vis;
	this->m_vis = 0;
}

bool Visibility::Decompress(int leafCount, HL1::tBSPLeaf* leafs, HL1::tBSPModel* models, unsigned char* visdata)
{
	if (leafCount <= 0)
		return true;

	this->m_leafCount = leafCount;
	this->m_vis = new tVISLeaf[this->m_leafCount];
	if (this->m_vis == 0)
		return false;

	for (int i = 0; i < this->m_leafCount; i++)
	{
		this->m_vis[i].leafs = 0;
		this->m_vis[i].leafCount = 0;
		int visOffset = leafs[i].visofs;

		for (int j = 1; j < models[0].visLeafs; visOffset++)
		{
			if (visdata[visOffset] == 0)
			{
				visOffset++;
				j += (visdata[visOffset]<<3);
			}
			else
			{
				for (unsigned char bit = 1; bit; bit<<=1, j++)
				{
					if (visdata[visOffset] & bit)
						this->m_vis[i].leafCount++;
				}
			}
		}

		if (this->m_vis[i].leafCount > 0)
		{
			this->m_vis[i].leafs = new int[this->m_vis[i].leafCount];
			if (this->m_vis[i].leafs == 0)
				return false;
		}
	}

	for (int i = 0; i < this->m_leafCount; i++)
	{
		int visOffset = leafs[i].visofs;
		int index = 0;
		for (int j = 1; j < models[0].visLeafs; visOffset++)
		{
			if (visdata[visOffset] == 0)
			{
				visOffset++;
				j += (visdata[visOffset]<<3);
			}
			else
			{
				for (unsigned char bit = 1; bit; bit<<=1, j++)
				{
					if (visdata[visOffset] & bit)
					{
						this->m_vis[i].leafs[index++] = j;
					}
				}
			}
		}
	}

	return true;
}
