//cloth simulation filter for airborne lidar filtering
#ifndef _CSF_H_
#define _CSF_H_

#include "point_cloud.h"
#include <iostream>
#include "Cloth.h"
using namespace wl;


class CSF
{
public:
	CSF(wl::PointCloud& cloud);
	virtual ~CSF();

	//input PC from files
	void readPointsFromFile(string filename);
	//save the ground points to file
	void saveGroundPoints(const vector<int>& grp, string path = "");
	void saveOffGroundPoints(const vector<int>& grp, string path = "");
	//get size of point cloud
	size_t size() const { return point_cloud.size(); }

	inline const LASPoint& index(int i) { return point_cloud[i]; }

	//ִ���˲����� �õ���������PointCloud �е����
	bool do_filtering(unsigned pcsize, vector< vector<int> >& output);

private:
	wl::PointCloud& point_cloud;

public:

	struct{
		//perameters
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
		int interations;
	} params;
};

#endif