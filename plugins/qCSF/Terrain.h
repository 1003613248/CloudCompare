#ifndef _TERRIAN_H_
#define _TERRIAN_H_

#include "PointCloud.h"
#include "Vec3.h"

//system
#include <string>

class Terrain
{
public:

	Terrain(wl::PointCloud &pc);
	virtual ~Terrain();

	wl::PointCloud& pc; //associated cloud
	Vec3 bbMin, bbMax;//bounding-box
	double off_avg_x, off_avg_z;//�Զ�shift�㷨

	//�����Ʊ��浽�ļ�
	void saveToFile(const wl::Point& offset, std::string path = "");

protected:
	
	//������Ƶ����Χ������
	void computeBoundingBox();

};


#endif