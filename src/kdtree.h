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

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

/*
 * Made it a class to abstract out how we store and manage coordinates.
 *  It gives the flexibility to switch from the current implementation of vector
 *  to tuple, map etc. in future.
 */
template <typename TCoordinate>
class Point
{
protected:
	vector<TCoordinate> coordinates; // point's location in space in coordinates
	int id; // unique id

	/* serialization/de-serialization of node */
	friend class boost::serialization::access;
	template <typename Archive>
	void serialize (Archive & ar, const unsigned int version)
	{
		ar & coordinates;
		ar & id;
	}

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
		this->id = -1;
	}
	Point ()
	{
		this->id = -1;
	}
	~Point()
	{

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

	int get_id () const
	{
		return this->id;
	}

};

template <typename TCoordinate>
class NearestPoint : public Point<TCoordinate>
{
private:
	// distance to query point
	TCoordinate distance;

public:
	NearestPoint ()
	{
		this->distance = 0;
	}

	void set_distance (TCoordinate distance)
	{
		this->distance = distance;
	}
	TCoordinate get_distance ()
	{
		return this->distance;
	}
	void set_coordiantes (vector<TCoordinate> coordinates)
	{
		this->coordinates = coordinates;
	}
	void set_id (int id)
	{
		this->id = id;
	}
	void update (const Point<TCoordinate> *point, TCoordinate distance)
	{
		this->coordinates = point->get_coordinates();
		this->id = point->get_id();
		this->distance = distance;
	}
};

template <typename TPoint, typename TCoordinate>
class KDTreeNode
{
private:
	// reference to a Point object.
	const TPoint *point;

	/* serialization/de-serialization of node */
	friend class boost::serialization::access;
	template <typename Archive>
	void serialize (Archive & ar, const unsigned int version)
	{
		ar & point;
		ar & left;
		ar & right;
		ar & splitting_point;
		ar & splitting_axis;
	}
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
	~KDTreeNode ()
	{
		delete left;
		delete right;
		if (point) delete point;
	}

	const TPoint *get_point () const;
	void set_point (const TPoint *point)
	{
		this->point = point;
	}

	bool is_leaf ()
	{
		if (!this->left && !this->right) return true;
		else return false;
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
	void find_nearest_point_recursive_internal (KDTreeNode<TPoint, TCoordinate> *curr_node,
			Point<TCoordinate> *query_point, NearestPoint<TCoordinate> **nearest_point);
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

	/* serialization/deserialization of tree */
	friend class boost::serialization::access;
	template <typename Archive>
	void serialize (Archive & ar, const unsigned int version)
	{
		ar & root;
		ar & num_dimensions;
		ar & points;
	}


public:
	KDTree (int num_dimensions);
	KDTree ();
	~KDTree ()
	{
		delete root;
		for (typename vector<TPoint *>::iterator it = this->points.begin(); it!=this->points.begin();it++)
		{
			if(*it) delete *it;
		}
	}

	/*
	 * Function saves tree to a file on disk
	 * File format: ?
	 */
	void save_tree (const char* archive_file_path);

	/*
	 * Function builds tree from a file stored in disk.
	 * File format: ?
	 */
	void load_tree (const char* archive_file_path);
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
	const Point<TCoordinate> *find_nearest_point (TPoint *query_point, TCoordinate *min_distance);

	/*
	 * Given a query point, the function returns the nearest neighbor to it using a brute force algorithm
	 */
	//tuple<Point<TCoordinate> *, TCoordinate> find_nearest_point_brute_force (Point<TCoordinate> *query_point);
	void find_nearest_point_brute_force (Point<TCoordinate> *query_point, NearestPoint<TCoordinate> **nearest_point);

	/*
	 * Given a query point, the function returns the nearest neighbor to it using
	 *  recursive kd-tree traversal algorithm
	 */
	void find_nearest_point_recursive (Point<TCoordinate> *query_point, NearestPoint<TCoordinate> **nearest_point);

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

/*
 * Computes distance-squared for efficiency
 */
template <typename TCoordinate>
TCoordinate compute_distance (const Point<TCoordinate> *point1, const Point<TCoordinate> *point2)
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
	//distance = sqrt (distance);
	return distance;
}

template<typename TPoint, typename TCoordinate>
KDTree<TPoint, TCoordinate>::KDTree (int num_dimensions)
{
	this->root = NULL;
	this->num_dimensions = num_dimensions;
}

template<typename TPoint, typename TCoordinate>
KDTree<TPoint, TCoordinate>::KDTree ()
{
	this->root = NULL;
	this->num_dimensions = -1;
}

/*
 * Greedy algorithm
 */
template<typename TPoint, typename TCoordinate>
const Point<TCoordinate> *KDTree<TPoint, TCoordinate>::find_nearest_point (TPoint *query_point, TCoordinate *min_distance)
{
	const Point<TCoordinate> *nearest_point = NULL;
	if (!this->root) return NULL;

	KDTreeNode<TPoint, TCoordinate> *node = this->root;
	while (node)
	{

		int axis = node->splitting_axis;
		TCoordinate point = node->splitting_point;

		if (query_point->get_coordinate (axis) < point)
		{
			node = node->left;
		}
		else
		{
			node = node->right;
		}

		if (node->is_leaf())
		{
			nearest_point = node->get_point();
			*min_distance = compute_distance (nearest_point, query_point);
			break;
		}
	}

	return nearest_point;
}

template<typename TPoint, typename TCoordinate>
void KDTree<TPoint, TCoordinate>::find_nearest_point_recursive_internal (KDTreeNode<TPoint, TCoordinate> *curr_node,
		Point<TCoordinate> *query_point, NearestPoint<TCoordinate> **nearest_point)
{
	if(!curr_node) return;

	const Point<TCoordinate> *curr_point = curr_node->get_point();
	if (curr_node->is_leaf())
	{
		if (!curr_point) return;

		TCoordinate distance = compute_distance (query_point, curr_point); // squared distance for efficiency

		if (!*nearest_point)
		{
			*nearest_point = new NearestPoint<TCoordinate>();
			(*nearest_point)->update (curr_point, distance);
		}
		else if (distance < (*nearest_point)->get_distance())
		{
			(*nearest_point)->update (curr_point, distance);
		}

		return;
	}

	KDTreeNode <TPoint, TCoordinate> *closer_node = NULL;
	KDTreeNode <TPoint, TCoordinate> *farther_node = NULL;

	// based on splitting axis and point determine which node is the closer one among left and right nodes
	if (query_point->get_coordinate(curr_node->splitting_axis) < curr_node->splitting_point)
	{
		closer_node = curr_node->left;
		farther_node = curr_node->right;
	}
	else
	{
		closer_node = curr_node->right;
		farther_node = curr_node->left;
	}

	find_nearest_point_recursive_internal (closer_node, query_point, nearest_point);

	if (*nearest_point)
	{
		TCoordinate distance_to_splitting_axis = abs(curr_node->splitting_point - query_point->get_coordinate(curr_node->splitting_axis));

		// prune the branches which are at least as far as the distance to the splitting axis
		if ((distance_to_splitting_axis*distance_to_splitting_axis /*error*/) <= (*nearest_point)->get_distance())
		{
			find_nearest_point_recursive_internal (farther_node, query_point, nearest_point);
		}
	}
	else
	{
		// until a nearest point is computed, search through both the left and right sub-trees.
		find_nearest_point_recursive_internal (farther_node, query_point, nearest_point);
	}
}


template<typename TPoint, typename TCoordinate>
void KDTree<TPoint, TCoordinate>::find_nearest_point_recursive (Point<TCoordinate> *query_point, NearestPoint<TCoordinate> **nearest_point)
{
	find_nearest_point_recursive_internal (this->root, query_point, nearest_point);

}

template<typename TPoint, typename TCoordinate>
void KDTree<TPoint, TCoordinate>::find_nearest_point_brute_force (Point<TCoordinate> *query_point,
		NearestPoint<TCoordinate> **nearest_point)
{
	*nearest_point = new NearestPoint<TCoordinate>();
	TCoordinate min_distance = numeric_limits<TCoordinate>::max();
	for (typename vector<TPoint *>::iterator it = this->points.begin(); it!=this->points.end(); it++)
	{
		TCoordinate distance = compute_distance (*it, query_point);
		if (distance < min_distance)
		{
			min_distance = distance;
			(*nearest_point)->update (*it, min_distance);
		}
	}
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
	Point<TCoordinate> *split_point = points[(start-points.begin()) + (end-start)/2];
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
