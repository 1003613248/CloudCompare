#ifndef XYZ_READER_H_
#define XYZ_READER_H_

#include "PointCloud.h"

//system
#include <string>

//��fname�ļ��ж�ȡ���ƣ����������ݴ洢��pointcloud��
bool read_xyz(std::string fname, wl::PointCloud &pointcloud);


#endif