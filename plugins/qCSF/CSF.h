//cloth simulation filter for airborne lidar filtering
#ifndef _CSF_H_
#define _CSF_H_

#include "point_cloud.h"
#include <iostream>
#include "Cloth.h"

class CSF
{
public:
	CSF(wl::PointCloud& cloud);
	virtual ~CSF();

	//input PC from files
	void readPointsFromFile(std::string filename);

	//save the ground points to file
	void saveGroundPoints(const std::vector<int>& grp, std::string path = "");
	void saveOffGroundPoints(const std::vector<int>& grp, std::string path = "");
	
	//ִ���˲����� �õ���������PointCloud �е����
	bool do_filtering(std::vector< std::vector<int> >& output);

private:
	wl::PointCloud& point_cloud;

public:

	struct
	{
		//parameters
		//���ٽ������ǵĵ�����һ������Ϊ1
		int k_nearest_points;

		//�Ƿ���б��º���
		bool bSloopSmooth;

		//ʱ�䲽��
		double time_step;

		//������ֵ
		double class_threshold;

		//���ϸ�����С
		double cloth_resolution;

		//����Ӳ�Ȳ���
		int rigidness;

		//����������
		int iterations;
	} params;
};

#endif