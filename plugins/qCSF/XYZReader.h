#ifndef XYZ_READER_H_
#define XYZ_READER_H_

#include <string>

#include "csf_h/point_cloud.h"

//��fname�ļ��ж�ȡ���ƣ����������ݴ洢��pointcloud��
bool read_xyz(std::string fname, wl::PointCloud &pointcloud);


#endif