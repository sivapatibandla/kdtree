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

using namespace std;

/*
 * Made it a class to abstract out how we store and manage coordinates.
 *  It gives the flexibility to switch from the current implementation of vector
 *  to tuple, map etc. in future.
 */
template <typename T>
class Point
{
private:
	vector<T> coordinates; // point's location in space in coordinates
	int id; // unique id
	string str;

public:
	// constructor: takes a list of coordinates and unique id
	Point (vector<T> coordinates, int id);

	// get dimension by zero-based index.
	T get_dimension (int index)
	{
		return this->coordinates[index];
	}

	// get the coordinates of all dimensions of the point
	vector<T> get_coordinates () const
	{
		return this->coordinates;
	}

	// string representation of the object
	string get_str ()
	{
		return this->str;
	}

};

template <typename T>
class KDTreeNode
{
private:
	//Point<T> *point;
public:
	KDTreeNode *left, *right;
	T splitting_point;
	int splitting_axis;
	Point<T> *point;
	T *data;
	KDTreeNode();
};

template<typename T>
class KDTree
{
private:
	KDTreeNode<T> *root;
	int num_dimensions;
	vector<Point<T> > points;

	void build_tree (KDTreeNode<T> *root, vector<Point<T> > points);
	/*
	 * Data partitioning algorithms
	 */
	//T compute_split_point(vector<Point<T> > points);
	int compute_split_axis(vector<Point<T> > points);

public:
	KDTree ();

	/*
	 * Builds a KD-tree given an array of points.
	 * Deprecate this and move to add_points() instead.
	 */
	void build_kdtree(vector<Point<T> > points);

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
	void add_point (Point<T> point);
	/*
	 * Adding multiple points to the tree
	 */
	void add_points (vector<Point<T> > points);
	/*
	 * Deletes a point by ID. How to assign a unique id to a point?
	 * If not by ID, how else can we delete it?
	 */
	void delete_point (int id);
	/*
	 * Given a query point, the function returns the nearest neighbor to it.
	 */
	Point<T>& find_nearest_point (Point<T> query_point);\

};

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
