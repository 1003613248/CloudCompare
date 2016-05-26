/*
This source code is about a ground filtering algorithm for airborn LiDAR data
based on physical process simulations, specifically cloth simulation.

this code is based on a Cloth Simulation Tutorial at the cg.alexandra.dk blog.
Thanks to Jesper Mosegaard (clothTutorial@jespermosegaard.dk)



When applying the cloth simulation to LIDAR point filtering. A lot of features
have been added to the original source code, including
* configuration file management
* point cloud data read/write
* point-to-point collsion detection
* nearest point search structure from CGAL
* addding a terrain class


*/
//using discrete steps (drop and pull) to approximate the physical process
//test merge ��ÿ�����ϵ���Χ�����ڽ���N���㣬�Ը߳����ֵ��Ϊ���ܵ������͵㡣

#ifndef _CLOTH_H_
#define _CLOTH_H_

#ifdef _WIN32
#include <windows.h> 
#endif
#include <math.h>
#include <vector>
#include <iostream>
#include <omp.h>
#include <iostream>
#include <sstream>
#include <list>
#include <cmath>
#include <vector>
#include <string>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/Orthogonal_k_neighbor_search.h>
#include <CGAL/Search_traits_2.h>
#include <list>
#include <queue>
#include <cmath>

#include "Vec3.h"
#include "Particle.h"
#include "Constraint.h"
#include <boost/progress.hpp>

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_2 Point_d;
typedef CGAL::Search_traits_2<K> TreeTraits;
typedef CGAL::Orthogonal_k_neighbor_search<TreeTraits> Neighbor_search;
typedef Neighbor_search::Tree Tree;

struct XY{
	XY(int x1, int y1){ x = x1; y = y1; }
	int x;
	int y;
};

class Terrain;

class Cloth
{
private:

	int num_particles_width; // number of particles in "width" direction
	int num_particles_height; // number of particles in "height" direction
	// total number of particles is num_particles_width*num_particles_height
	int constraint_iterations;

	int rigidness;
	double time_step;

	std::vector<Particle> particles; // all particles that are part of this cloth
	std::vector<Constraint> constraints; // alle constraints between particles as part of this cloth


	//��ʼƽ��λ��
	Vec3 origin_pos1; //���ϽǶ���  ƽ���Ϊˮƽ

	//�˲����´������
	double smoothThreshold;
	double heightThreshold;

	//heightvalues
	std::vector<double> heightvals;

	//movable particle index
	std::vector<int> movableIndex;
	std::vector< std::vector<int> > particle_edges;

	Particle* getParticle(int x, int y) { return &particles[y*num_particles_width + x]; }
	const Particle* getParticle(int x, int y) const { return &particles[y*num_particles_width + x]; }
	
	void makeConstraint(Particle *p1, Particle *p2) { constraints.push_back(Constraint(p1, p2)); }

public:
	
	inline int getSize() const { return num_particles_width * num_particles_height; }
	//��ȡ��index��particle
	const Particle* getParticle1d(int index) const { return &particles[index]; }

public:
	
	Cloth() {}
	
	/* This is a important constructor for the entire system of particles and constraints*/
	Cloth(	double width,
			double height,
			int num_particles_width,
			int num_particles_height,
			const Vec3& origin_pos1,
			double smoothThreshold,
			double heightThreshold,
			int rigidness,
			double time_step);

	void setheightvals(const std::vector<double>& heightvals)
	{
		this->heightvals = heightvals;
	}

	/* this is an important methods where the time is progressed one time step for the entire cloth.
	This includes calling satisfyConstraint() for every constraint, and calling timeStep() for all particles
	*/
	double timeStep();

	/* used to add gravity (or any other arbitrary vector) to all particles*/
	void addForce(const Vec3 direction);


	//��Ⲽ���Ƿ��������ײ
	void terrCollision(const std::vector<double>& heightvals, Terrain* terr, bool &flag);

	//�Կ��ƶ��ĵ���б��´���
	void movableFilter();
	//�ҵ�ÿ����ƶ��㣬�����ͨ������Χ�Ĳ����ƶ��㡣���������м�ƽ�
	void findUnmovablePoint(const std::vector<XY>& connected,
							const std::vector<double>& heightvals,
							std::vector<int>& edgePoints);
	
	//ֱ�Ӷ���ͨ�������б��´���
	void handle_slop_connected(	const std::vector<int>& edgePoints,
								const std::vector<XY>& connected,
								const std::vector< std::vector<int> >& neibors,
								const std::vector<double> &heightvals);

	//�����ϵ㱣�浽�ļ�
	void saveToFile(std::string path = "");
	//�����ƶ��㱣�浽�ļ�
	void saveMovableToFile(std::string path = "");

};


#endif