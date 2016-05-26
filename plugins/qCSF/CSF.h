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
	CSF();
	~CSF();

	//input PC from vectors 
	void setPointCloud(vector< LASPoint > points);
	//input PC from files
	void readPointsFromFile(string filename);
	//save the ground points to file
	void saveGroundPoints(vector<int> grp, string path = "");
	void saveOffGroundPoints(vector<int> grp, string path = "");
	//get size of point cloud
	size_t size(){return point_cloud.size();}

	LASPoint index(int i){return point_cloud[i];}


	//�����е�PointCloud������
	void setPointCloud(PointCloud &pc);

	//ִ���˲����� �õ���������PointCloud �е����
	vector<vector<int>> do_filtering(unsigned pcsize);
private:
	  wl::PointCloud point_cloud;

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
	}params;
};

#endif