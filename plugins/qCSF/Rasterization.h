#ifndef _RASTERIZATION_H_
#define _RASTERIZATION_H_

#include "Cloth.h"
#include "csf_h/point_cloud.h"

class Rasterization
{
public:
	int N;

public:
	Rasterization(int N = 1)
	{
		this->N = N;
	}
	virtual ~Rasterization() {}

	//�Ե��ƽ������ٽ�������Ѱ����Χ�����N����  ����������
	bool RasterTerrian(const Cloth& cloth, const wl::PointCloud& pc, std::vector<double>& heightVal);
};

#endif //_RASTERIZATION_H_
