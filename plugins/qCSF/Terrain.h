#ifndef _TERRIAN_H_
#define _TERRIAN_H_

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "csf_h/point_cloud.h"

class Terrain
{
public:

	Terrain(wl::PointCloud &pc);
	virtual ~Terrain();

	wl::PointCloud& pc; //����
	double cube[8];//  ��Χ���е��Ƶ�����������
	double off_avg_x, off_avg_z;//�Զ�shift�㷨

	//�����Ʊ��浽�ļ�
	void saveToFile(const wl::Point& offset, std::string path = "");

protected:
	
	//������Ƶ����Χ������
	void bound_box();

	//������Ƶ����ֵ����Сֵ
	double getMin(int direction);
	double getMax(int direction);
};


#endif