#ifndef _C2CDIST_H_
#define _C2CDIST_H_

#include "Cloth.h"

//������������֮��ľ���
class c2cdist
{
public:
	
	c2cdist(double threshold) : class_treshold(threshold) {}
	virtual ~c2cdist() {}

public:
	bool c2cdist::calCloud2CloudDist(Cloth cloth, PointCloud &pc, unsigned PCSIZE, vector< vector<int> >& output);

private:
	
	double class_treshold;
};




#endif