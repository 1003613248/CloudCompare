#ifndef _C2CDIST_H_
#define _C2CDIST_H_


#include "Cloth.h"

//������������֮��ľ���

class c2cdist
{
public:
	c2cdist(double threshold):class_treshold(threshold){}
	~c2cdist(){}
public:
	vector<vector<int>> c2cdist::calCloud2CloudDist(Cloth cloth, PointCloud &pc,unsigned PCSIZE);
private:
	double class_treshold;//
};




#endif