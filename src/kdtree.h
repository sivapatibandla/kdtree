/*
 * kdtree.h
 *
 *  Created on: Jun 30, 2016
 *      Author: Siva
 */

#ifndef KDTREE_H_
#define KDTREE_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <array>

using namespace std;

/*
 * Made it a class to abstract out how we store and manage coordinates.
 *  It gives the flexibility to switch from the current implementation of vector
 *  to tuple, map etc. in future.
 */
template <typename TCoordinate>
class Point
{
private:
	vector<TCoordinate> coordinates; // point's location in space in coordinates
	int id; // unique id

public:
	// constructor: takes a list of coordinates and unique id
	Point (vector<TCoordinate> coordinates, int id)
	{
		cout << "!!!Creating point!!!" << endl;
		this->coordinates = coordinates;
		this->id = id;
	}

	// get coordinate on the given axis by zero-based index.
	TCoordinate get_coordinate (int index) const
	{
		return this->coordinates[index];
	}

	// get the coordinates of all dimensions of the point
	vector<TCoordinate> get_coordinates () const
	{
		return this->coordinates;
	}

};

template <typename TPoint, typename TCoordinate>
class KDTreeNode
{
private:
	// reference to a Point object.
	const TPoint *point;
public:
	KDTreeNode *left, *right;
	TCoordinate splitting_point;
	int splitting_axis;
	KDTreeNode ()
	{
		cout << "!!!Creating tree node!!!" << endl;
		this->splitting_axis = 0;
		this->splitting_point = 0;
		this->left = NULL;
		this->right = NULL;
		this->point = NULL;
	}

	TPoint get_point ();
	void set_point (const TPoint *point)
	{
		this->point = point;
	}
};

template<typename TPoint, typename TCoordinate>
TPoint KDTreeNode<TPoint, TCoordinate>::get_point ()
{
	return this->point;
}

template<typename TPoint, typename TCoordinate>
class KDTree
{
private:
	KDTreeNode<TPoint, TCoordinate> *root;
	int num_dimensions;
	vector<TPoint> points;

	void build_tree (KDTreeNode<TPoint, TCoordinate> *root,
			const vector<TPoint> &points,
			typename vector<TPoint>::const_iterator start,
			typename vector<TPoint>::const_iterator end);
	/*
	 * Data partitioning algorithms
	 */
	TCoordinate compute_split_point(const vector<TPoint> &points,
			typename vector<TPoint>::const_iterator start,
			typename vector<TPoint>::const_iterator end,
			int split_axis);
	int compute_split_axis(const vector<TPoint> &points,
			typename vector<TPoint>::const_iterator start,
			typename vector<TPoint>::const_iterator end);



public:
	KDTree ();

	/*
	 * Function saves tree to a file on disk
	 * File format: ?
	 */
	void save_tree ();

	/*
	 * Function builds tree from a file stored in disk.
	 * File format: ?
	 */
	void read_tree (string filename);
	/*
	 * Add a point to the tree
	 */
	void add_point (TPoint point);
	/*
	 * Adding multiple points to the tree
	 */
	void add_points (vector<TPoint> points);
	/*
	 * Deletes a point by ID. How to assign a unique id to a point?
	 * If not by ID, how else can we delete it?
	 */
	void delete_point (int id);
	/*
	 * Given a query point, the function returns the nearest neighbor to it.
	 */
	TPoint& find_nearest_point (TPoint query_point);

};

template<typename TPoint, typename TCoordinate>
void KDTree<TPoint, TCoordinate>::add_points (vector<TPoint> points)
{
	cout << "adding no. of points: " << points.size() << endl;
	build_tree (this->root, points, points.begin(), points.end());
}

template<typename TPoint, typename TCoordinate>
void KDTree<TPoint, TCoordinate>::build_tree (
		KDTreeNode<TPoint, TCoordinate> *root,
		const vector<TPoint> &points,
		typename vector<TPoint>::const_iterator start,
		typename vector<TPoint>::const_iterator end)
{
	if (points.size() == 0)
	{
		return;
	}
	KDTreeNode<TPoint, TCoordinate> *node = new KDTreeNode<TPoint, TCoordinate>();
	if (points.size() == 1)
	{
		node->set_point(&points[0]);
	}
	node->splitting_axis = KDTree<TPoint, TCoordinate>::compute_split_axis(points, start, end);
	// sort based on split axis
	// sort()
	node->splitting_point = KDTree<TPoint, TCoordinate>::compute_split_point(points, start, end, node->splitting_axis);

	cout << "split point " << node->splitting_point << "split axis " << node->splitting_axis;

	build_tree (root->left, points, start, end - (end-start)/2);
	build_tree (root->right, points, end - (end-start)/2 + 1, end);
}

template<typename TPoint, typename TCoordinate>
KDTree<TPoint, TCoordinate>::KDTree ()
{
	this->root = NULL;
	this->num_dimensions = 0;
}


class Comparator
{
private:
	int dimension;
public:
	Comparator (int dimension)
	{
		this->dimension = dimension;
	}
	template<typename T>
	bool operator()(const Point<T> left, const Point<T> right)
	{
		return left.get_coordinate(dimension) < right.get_coordinate (dimension);
	}
};

template<typename TPoint, typename TCoordinate>
TCoordinate KDTree<TPoint, TCoordinate>::compute_split_point(
			const vector<TPoint> &points,
			typename vector<TPoint>::const_iterator start,
			typename vector<TPoint>::const_iterator end,
			int split_axis)
{
	cout << "Computing split point for axis " << split_axis << endl;

	TCoordinate c = 1.0;
	// compute median of the given axis
	Comparator comp_obj(split_axis);
	sort (start, end, comp_obj);
	Point<TCoordinate> split_point = points[(end-start)/2];
	return split_point.get_coordinate(split_axis);

	return c;
}

template<typename TPoint, typename TCoordinate>
int KDTree<TPoint, TCoordinate>::compute_split_axis(
		const vector<TPoint> &points,
		typename vector<TPoint>::const_iterator start,
		typename vector<TPoint>::const_iterator end)
{
	cout << "Computing split axis" << endl;
	int split_axis = 0;

	// find the range of coordinates each dimension
	TCoordinate max_range = numeric_limits<TCoordinate>::min();
	vector<TCoordinate> min_coordinate (this->num_dimensions, numeric_limits<TCoordinate>::max());
	vector<TCoordinate> max_coordinate (this->num_dimensions, numeric_limits<TCoordinate>::max());
	for (typename vector<TPoint>::const_iterator it = start; it != end; it++)
	{
		vector<TCoordinate> coordinates = it->get_coordinates();
		for (int i = 0; i<this->num_dimensions; i++)
		{
			min_coordinate[i] = min (min_coordinate[i], it->get_coordinate(i));
			max_coordinate[i] = max (max_coordinate[i], it->get_coordinate(i));
		}
	}
	for (int i = 0; i<this->num_dimensions; i++)
	{
		TCoordinate range = max_coordinate[i] - min_coordinate[i];
		max_range = max(max_range, range);
		split_axis = i;
	}

	return split_axis;
}

template<typename T>
ostream& operator<< (ostream &os, const Point<T> &point)
{
	os << "(";
	for (typename vector<T>::iterator it = point.get_coordinates().begin();
			it!=point.get_coordinates().end(); ++it)
	{
		os << *it;
		if (it != point.get_coordinates().end()-1) os << ",";
	}

	os << ")";
	return os;
}

#endif /* KDTREE_H_ */
