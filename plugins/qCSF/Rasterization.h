#ifndef _RASTERIZATION_H_
#define _RASTERIZATION_H_

#include "Cloth.h"
#include "PointCloud.h"

#define SQUARE_DIST(x1,y1,x2,y2) (((x1)-(x2))*((x1)-(x2))+((y1)-(y2))*((y1)-(y2)))

class Rasterization
{
public:

	//�Ե��ƽ������ٽ�������Ѱ����Χ�����N����  ����������
	static bool RasterTerrain(Cloth& cloth, const wl::PointCloud& pc, std::vector<double>& heightVal, unsigned KNN = 1);

};

#endif //_RASTERIZATION_H_
