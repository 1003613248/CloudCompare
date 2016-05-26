#ifndef _KNN_H_
#define _KNN_H_

#include "Cloth.h"
#include "csf_h/point_cloud.h"

class Rasterlization
{
public:
	int N;

public:
	Rasterlization(int N = 1)
	{
		this->N = N;
	}
	virtual ~Rasterlization() {}

	//�Ե��ƽ������ٽ�������Ѱ����Χ�����N����  ����������
	bool RasterTerrian(Cloth cloth, const wl::PointCloud& pc, std::vector<double>& heightVal);
};

#endif