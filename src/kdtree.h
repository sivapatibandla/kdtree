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
#include <cmath>
#include <tuple>

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
	Point (vector<TCoordinate> coordinates)
	{
		this->coordinates = coordinates;
		this->id = 0;
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

	int get_id ()
	{
		return this->id;
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
	TPoint& find_nearest_point (TPoint &query_point);

	/*
	 * Given a query point, the function returns the nearest neighbor to it using a brute force algorithm
	 */
	//tuple<Point<TCoordinate> *, TCoordinate> find_nearest_point_brute_force (Point<TCoordinate> *query_point);
	Point<TCoordinate> *find_nearest_point_brute_force (Point<TCoordinate> *query_point, TCoordinate *min_distance);

	KDTreeNode<TPoint, TCoordinate>* get_root () const
	{
		return this->root;
	}

	int get_num_dimensions ()
	{
		return this->num_dimensions;
	}

};

template<typename TPoint, typename TCoordinate>
void KDTree<TPoint, TCoordinate>::add_points (vector<TPoint *> points)
{
	cout << "adding no. of points: " << points.size() << endl;
	build_tree (&this->root, points, points.begin(), points.end());
	this->points = points;
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

template <typename TCoordinate>
TCoordinate compute_distance (Point<TCoordinate> *point1, Point<TCoordinate> *point2)
{
	assert (point1->get_coordinates().size() == point2->get_coordinates().size());
	int num_dimensions = point1->get_coordinates().size();
	TCoordinate distance = 0;
	for (int i = 0; i<num_dimensions; i++)
	{
		TCoordinate temp = abs(point1->get_coordinate(i) - point2->get_coordinate(i));
		temp *= temp;
		distance += temp;
	}
	distance = sqrt (distance);
	return distance;
}

template<typename TPoint, typename TCoordinate>
KDTree<TPoint, TCoordinate>::KDTree (int num_dimensions)
{
	this->root = NULL;
	this->num_dimensions = num_dimensions;
}

template<typename TPoint, typename TCoordinate>
//tuple<Point<TCoordinate> *, TCoordinate> KDTree<TPoint, TCoordinate>::find_nearest_point_brute_force (Point<TCoordinate> *query_point)
Point<TCoordinate> *KDTree<TPoint, TCoordinate>::find_nearest_point_brute_force (Point<TCoordinate> *query_point, TCoordinate *min_distance)
{
	//cout << "nearest point brute force, for points size " << this->points.size() << endl;
	*min_distance = numeric_limits<TCoordinate>::max();
	TPoint* min_point = NULL;
	for (typename vector<TPoint *>::iterator it = this->points.begin(); it!=this->points.end(); it++)
	{
		TCoordinate distance = compute_distance (*it, query_point);
		if (distance < *min_distance)
		{
			//cout << "min distance: " << min_distance << endl;
			*min_distance = distance;
			min_point = *it;
		}
	}
	cout << "nearest point using brute force: " << *min_point << " min distance " << *min_distance << endl;
	//return make_tuple(min_point, min_distance);
	return min_point;
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

std::vector<std::string> split(const std::string &text, char sep);

#endif /* KDTREE_H_ */
