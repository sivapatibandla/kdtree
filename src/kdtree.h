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
#include <deque>
#include <cassert>

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
		this->coordinates = coordinates;
		this->id = id;
	}

	// get coordinate on the given axis by zero-based index.
	TCoordinate get_coordinate (int index) const
	{
		return this->coordinates[index];
	}

	// get the coordinates of all dimensions of the point
	const vector<TCoordinate>& get_coordinates () const
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

	/*
	 * Invariants:
	 * 	1. if left and right are NULL then point should be valid
	 * 	2. if left or right is !NULL then point should be NULL
	 */
	KDTreeNode ()
	{
		this->splitting_axis = 0;
		this->splitting_point = 0;
		this->left = NULL;
		this->right = NULL;
		this->point = NULL;
	}

	const TPoint *get_point () const;
	void set_point (const TPoint *point)
	{
		this->point = point;
	}
};

template<typename TPoint, typename TCoordinate>
const TPoint *KDTreeNode<TPoint, TCoordinate>::get_point () const
{
	return this->point;
}

template<typename TPoint, typename TCoordinate>
class KDTree
{
private:
	KDTreeNode<TPoint, TCoordinate> *root;
	int num_dimensions;
	vector<TPoint *> points;

	void build_tree (KDTreeNode<TPoint, TCoordinate> **root,
			const vector<TPoint *> &points,
			typename vector<TPoint *>::iterator start,
			typename vector<TPoint *>::iterator end);
	/*
	 * Data partitioning algorithms
	 */
	TCoordinate compute_split_point(const vector<TPoint *> &points,
			typename vector<TPoint *>::iterator start,
			typename vector<TPoint *>::iterator end,
			int split_axis);
	int compute_split_axis(const vector<TPoint *> &points,
			typename vector<TPoint *>::iterator start,
			typename vector<TPoint *>::iterator end);



public:
	KDTree (int num_dimensions);

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
	void add_point (TPoint *point);
	/*
	 * Adding multiple points to the tree
	 */
	void add_points (vector<TPoint *> points);
	/*
	 * Deletes a point by ID. How to assign a unique id to a point?
	 * If not by ID, how else can we delete it?
	 */
	void delete_point (int id);
	/*
	 * Given a query point, the function returns the nearest neighbor to it.
	 */
	TPoint& find_nearest_point (TPoint query_point);

	KDTreeNode<TPoint, TCoordinate>* get_root () const
	{
		return this->root;
	}

};

template<typename TPoint, typename TCoordinate>
void KDTree<TPoint, TCoordinate>::add_points (vector<TPoint *> points)
{
	cout << "adding no. of points: " << points.size() << endl;
	build_tree (&this->root, points, points.begin(), points.end());
}

template<typename TPoint, typename TCoordinate>
void KDTree<TPoint, TCoordinate>::build_tree (
		KDTreeNode<TPoint, TCoordinate> **root,
		const vector<TPoint *> &points,
		typename vector<TPoint *>::iterator start,
		typename vector<TPoint *>::iterator end)
{
	//cout << "build_tree() called with start " << start - points.begin() << " end " << end - points.begin() << endl;

	// KD-Tree can't be constructed without points so return immediately
	if ((end-start) == 0)
	{
		cout << "size of sub-array 0 -- returning" << endl;
		return;
	}

	KDTreeNode<TPoint, TCoordinate> *node = new KDTreeNode<TPoint, TCoordinate>();
	*root = node;

	/*
	 * base case: size of sub-array is 1.
	 * assign the only point in the sub-array to the leaf node created here,
	 * and return
	 */
	if ((end-start)== 1)
	{
		//cout << "size of sub-array 1 -- assigning the point to the leaf node " << *start << endl;
		const Point<TCoordinate> *p = *start;
		node->set_point(p);
		return;
	}

	node->splitting_axis = KDTree<TPoint, TCoordinate>::compute_split_axis(points, start, end);
	node->splitting_point = KDTree<TPoint, TCoordinate>::compute_split_point(points, start, end, node->splitting_axis);

	//cout << "split point " << node->splitting_point << " split axis " << node->splitting_axis << endl;
	build_tree (&(*root)->left, points, start, end - (end-start)/2);
	build_tree (&(*root)->right, points, end - (end-start)/2, end);
}

template<typename TPoint, typename TCoordinate>
KDTree<TPoint, TCoordinate>::KDTree (int num_dimensions)
{
	this->root = NULL;
	this->num_dimensions = num_dimensions;
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
	bool operator()(const Point<T> *left, const Point<T> *right)
	{
		return left->get_coordinate(dimension) < right->get_coordinate (dimension);
	}
};

template<typename TPoint, typename TCoordinate>
TCoordinate KDTree<TPoint, TCoordinate>::compute_split_point(
			const vector<TPoint *> &points,
			typename vector<TPoint *>::iterator start,
			typename vector<TPoint *>::iterator end,
			int split_axis)
{

	// compute median of the given axis
	Comparator comp_obj(split_axis);
	sort (start, end, comp_obj);
	Point<TCoordinate> *split_point = points[(end-start)/2];
	//cout << "Computed split point for axis " << split_axis << " point " << split_point->get_coordinate(split_axis) <<  endl;
	return split_point->get_coordinate(split_axis);
}

template<typename TPoint, typename TCoordinate>
int KDTree<TPoint, TCoordinate>::compute_split_axis(
		const vector<TPoint *> &points,
		typename vector<TPoint *>::iterator start,
		typename vector<TPoint *>::iterator end)
{
	int split_axis = 0;

	// find the range of coordinates each dimension
	TCoordinate max_range = numeric_limits<TCoordinate>::min();
	vector<TCoordinate> min_coordinate (this->num_dimensions, numeric_limits<TCoordinate>::max());
	vector<TCoordinate> max_coordinate (this->num_dimensions, numeric_limits<TCoordinate>::min());
	for (typename vector<TPoint *>::const_iterator it = start; it != end; it++)
	{
		vector<TCoordinate> coordinates = (*it)->get_coordinates();
		for (int i = 0; i<this->num_dimensions; i++)
		{
			min_coordinate[i] = min (min_coordinate[i], (*it)->get_coordinate(i));
			max_coordinate[i] = max (max_coordinate[i], (*it)->get_coordinate(i));
		}
	}
	for (int i = 0; i<this->num_dimensions; i++)
	{
		TCoordinate range = max_coordinate[i] - min_coordinate[i];
		if (range > max_range)
		{
			max_range = range;
			split_axis = i;
		}

	}
	//cout << "Computed max_range " << max_range << " for split axis " << split_axis << endl;
	return split_axis;
}

template<typename T>
ostream& operator<< (ostream &os, const Point<T> &point)
{
	os << "(";
	const vector<T> &coordinates = point.get_coordinates();
	for (typename vector<T>::const_iterator it = coordinates.cbegin();
			it!=coordinates.cend(); it++)
	{
		os << *it;
		if (it != coordinates.end()-1) os << ",";
	}

	os << ")";
	return os;
}

template<typename TPoint, typename TCoordinate>
ostream& operator<< (ostream &os, const KDTreeNode<TPoint, TCoordinate> &node)
{
	os << "split axis: " << node.splitting_axis << " split point " << node.splitting_point;
	const Point<TCoordinate> *p = node.get_point();
	if (p) os << " point " << *p;

	return os;
}

template<typename TPoint, typename TCoordinate>
ostream& operator<< (ostream &os, const KDTree<TPoint, TCoordinate> &tree)
{
	cout << "Printing tree:" << endl;
	if (!tree.get_root()) return os;

	deque<KDTreeNode<TPoint, TCoordinate> *> q;
	q.push_front(tree.get_root());
	KDTreeNode<TPoint, TCoordinate> *curr = q.back();
	while (!q.empty())
	{
		curr = q.back();
		os << *curr << " ";
		q.pop_back();
		if (curr->left) q.push_front (curr->left);
		if (curr->right) q.push_front (curr->right);
		os << endl;
	}

	os << endl;

	return os;
}



#endif /* KDTREE_H_ */
