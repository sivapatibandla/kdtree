//============================================================================
// Name        : kdtree.cpp
// Author      : Siva Teja Patibandla
// Version     :
// Copyright   : Your copyright notice
// Description : KD-Tree library in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>

#include "kdtree.h"

using namespace std;

void test (int num_points, int num_dimensions)
{
	if (!num_points) return;

	// initialize random seed
	srand (time(NULL));

	// generate random points
	KDTree<float> tree;
	vector<Point<float> > points;
	for (int i=0; i<num_points; i++)
	{
		vector<float> point;
		for (int j=0; j<num_dimensions; j++)
		{
			point.push_back(rand()%num_points);

		}
		Point<float> p (point, i);
		points.push_back(p);
	}

	// build kdtree
	tree.build_kdtree (points);

	//cout << tree;
}

template<typename T>
Point<T>::Point (vector<T> coordinates, int id)
{
	cout << "!!!Creating point!!!" << endl;
	this->coordinates = coordinates;
	this->id = id;
	this->str += "(";
	stringstream ss;
	//ostream os;
	ss << "(";
	//s <<
	for (int i = 0; i<coordinates.size(); i++)
	{
		this->str += coordinates[i];//this->get_dimension(i);
		ss << coordinates[i];
		//os << coordinates[i];
		//os << ',';
		//cout << "loop " << coordinates[i] << endl;
	}
	/*for (vector<T>::iterator it = this->p.begin(); it!= this->p.end(); it++)
	{
		this->str += *it;
	}*/
	ss << ")";
	//os << ")"
	cout << "stream " << ss.str() << endl;
	this->str += ")";
}

#if 0
template<typename T>
ostream& operator<< (ostream& os, const Point<T> &dummy)
{
	cout << "called operator" << endl;
	os << "(";
	for (vector<float>::iterator it = dummy.p.begin(); it!= dummy.p.end(); it++)
	{
		os << *it << ",";
	}
	os << ")";

  return os;
}
#endif

template<typename T>
KDTreeNode<T>::KDTreeNode ()
{
	cout << "!!!Creating tree node!!!" << endl;
	this->splitting_axis = 0;
	this->splitting_point = 0;
	this->left = NULL;
	this->right = NULL;
	this->point = NULL;
}

template<typename T>
KDTree<T>::KDTree ()
{
	this->root = NULL;
	this->num_dimensions = 0;
}

template<typename T>
void KDTree<T>::build_tree (KDTreeNode<T> *root, vector<Point<T> > points)
{
	if (points.size() == 0)
	{
		return;
	}
	KDTreeNode<T> *node = new KDTreeNode<T>();
	if (points.size() == 1)
	{
		node->point = &points[0];
	}

	//node->splitting_point = KDTree<T>::compute_split_point(points);
	node->splitting_axis = KDTree<T>::compute_split_axis(points);

	//build_tree (root->left, left_half);
	//build_tree (root->right, right_half);
}

template<typename T>
void KDTree<T>::build_kdtree (vector<Point<T> > points)
{
	this->points = points;
	this->build_tree(this->root, points);
}

/*
template<typename T>
T KDTree<T>::compute_split_point(vector<Point<T> > points)
{
	cout << "TODO" << endl;
	return ;
}*/

template<typename T>
int KDTree<T>::compute_split_axis(vector<Point<T> > points)
{
	cout << "TODO" << endl;
	return 1;
}
