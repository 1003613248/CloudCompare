//CSF
#include "CSF.h"
#include "XYZReader.h"
#include "Terrian.h"
#include "Vec3.h"
#include "Cloth.h"
#include "KNN.h"
#include "c2cdist.h"
#include <cmath>

//Qt
#include <QProgressDialog>
#include <QApplication>

//CC
#include <DistanceComputationTools.h>

CSF::CSF(wl::PointCloud& cloud)
	: point_cloud(cloud)
{
	params.k_nearest_points = 1;
	params.bSloopSmooth = true;
	params.time_step = 0.65;
	params.class_threshold=0.5;
	params.cloth_resolution = 1.5;
	params.rigidness = 3;
	params.interations = 500;
}

CSF::~CSF()
{
}

//���ļ��ж�ȡ���ƣ��ó�����δ�õ��˺���
void CSF::readPointsFromFile(string filename)
{
	read_xyz(filename, this->point_cloud);
}

//CSF������ dofiltering
bool CSF::do_filtering(unsigned pcsize, vector< vector<int> >& output)
{
	//build the terrain
	Terrian terr(point_cloud);

	//origin cloth height
	double cloth_y_height = 0.05;

	//computing the number of cloth node
	double clothbuffer_d = 4;//set the cloth buffer
	Vec3 origin_pos1(terr.cube[0] - clothbuffer_d, terr.cube[3] + cloth_y_height, terr.cube[4] - clothbuffer_d);
	int width_num = (terr.cube[1] - terr.cube[0] + clothbuffer_d * 2) / params.cloth_resolution;
	int height_num = (terr.cube[5] - terr.cube[4] + clothbuffer_d * 2) /params.cloth_resolution;
	
	//one Cloth object of the Cloth class
	Cloth cloth1(	terr.cube[1] - terr.cube[0] + clothbuffer_d * 2,
					terr.cube[5] - terr.cube[4] + clothbuffer_d * 2,
					width_num,
					height_num,
					origin_pos1,
					0.3,
					9999,
					params.rigidness,
					params.time_step);

	Rasterlization raster(params.k_nearest_points);
	vector<double> heightvals;
	raster.RasterTerrian(cloth1, point_cloud, heightvals);
	cloth1.setheightvals(heightvals);

	double time_step2 = params.time_step * params.time_step;
	double gravity = 0.2;
	bool flag = false;

	//do the filtering
	//��ʾ������
	QProgressDialog pDlg2;
	pDlg2.setWindowTitle("CSF");
	pDlg2.setLabelText("Do filtering....");
	pDlg2.setRange(0, params.interations);
	pDlg2.show();
	QApplication::processEvents();

#ifdef _DEBUG
	//�˲���ѭ��
	ofstream fout("clothDistDiffMax.txt");
	if(!fout)
	{
		cerr<<"File of clothdist.txt not open!"<<endl;
		exit(1);
	}
#endif

	for (int i = 0; i < params.interations; i++)
	{
		//�˲�������
		cloth1.addForce(Vec3(0, -gravity, 0) * time_step2);
		double maxheight = cloth1.timeStep();
		cloth1.terrCollision(heightvals, &terr, flag);

#ifdef _DEBUG
		fout.setf(ios::app);
		fout.precision(3);  
		fout<<i<<" "<<maxheight<<endl;
#endif
		//�ж�ѭ����ֹ����
		if (maxheight != 0 && maxheight < params.class_threshold / 100)
		{
			pDlg2.setValue(params.interations);
			break;
		}
		pDlg2.setValue(pDlg2.value()+1);
		QApplication::processEvents();
	}
	pDlg2.hide();
	QApplication::processEvents();

	//slope processing
	if (params.bSloopSmooth)
	{
		cloth1.movableFilter();
	}
	
	//classification of the points
	return c2cdist(params.class_threshold).calCloud2CloudDist(cloth1, point_cloud, pcsize, output);
}

//��������
void CSF::saveGroundPoints(const vector<int>& grp, string path)
{
	string filepath = "terr_ground.txt";
	if (path != "")
	{
		filepath = path;
	}
	ofstream f1(filepath, ios::out);
	if (!f1)
		return;
	for (size_t i = 0; i < grp.size(); i++)
	{
		f1 << fixed << setprecision(8) << point_cloud[grp[i]].x<< "	" << point_cloud[grp[i]].z << "	" << -point_cloud[grp[i]].y << endl;
	}
	f1.close();
}

void CSF::saveOffGroundPoints(const vector<int>& grp, string path)
{
	string filepath = "off-ground points.txt";
	if (path != "")
	{
		filepath = path;
	}
	ofstream f1(filepath, ios::out);
	if (!f1)
		return;
	for (size_t i = 0; i < grp.size(); i++)
	{
		f1 << fixed << setprecision(8) << point_cloud[grp[i]].x<< "	" << point_cloud[grp[i]].z << "	" << -point_cloud[grp[i]].y << endl;
	}
	f1.close();
}