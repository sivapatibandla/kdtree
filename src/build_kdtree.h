/*
 * build_kdtree.h
 *
 *  Created on: Jul 16, 2016
 *      Author: Siva
 */

#ifndef BUILD_KDTREE_H_
#define BUILD_KDTREE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/lexical_cast.hpp>

#include "kdtree.h"
#include "util.h"

template<typename TCoordinate>
vector <Point<TCoordinate> *> read_points_from_file (const char *file_path, int &num_dimensions)
{

	ifstream file ( file_path, ifstream::in );
	if (!file.good())
	{
		cout << "Unable to read file at " << file_path << "... please give the correct and full path to the points file" << endl;
		file.close();
		assert (0);
	}

	int point_id = 0;
	vector <Point<TCoordinate> *> points;

	// extract no. of dimensions
	string line;
	vector<std::string> words;
	if (file.good())
	{
		getline(file, line);
		words = split (line, ',');
		num_dimensions = words.size();
		file.seekg (0, ios::beg);
	}

	// extract points
	while (file.good())
	{
		getline(file, line);
		words = split (line, ',');

		//assert (words.size() == num_dimensions);
		// ignore lines which are not properly formatted. TODO: wrap this in a try & catch block
		if (words.size() != num_dimensions) {point_id++;continue;}

		// extract coordinates
		vector<TCoordinate> coordinates;
		for (vector<string>::iterator it = words.begin(); it!=words.end(); it++)
		{
			coordinates.push_back(boost::lexical_cast<TCoordinate>(*it));
		}

		Point<TCoordinate> *point = NULL;
		try
		{
		  point = new Point<TCoordinate>(coordinates, point_id);
		  points.push_back(point);
		}
		catch(std::bad_alloc& exc)
		{
		  cout << "Memory allocation failed... try the next point" << endl;
		}

		point_id++;
	}
	file.close();

	return points;
}

template<typename TCoordinate>
KDTree<Point<TCoordinate>, TCoordinate> *build_kdtree(const char* points_file_path) {

	int num_dimensions = 0;
	KDTree<Point<TCoordinate>, TCoordinate> *kdtree = NULL;

	vector <Point<TCoordinate> *> points = read_points_from_file<TCoordinate> (points_file_path, num_dimensions);

	// build kd-tree
	if (points.size() > 0)
	{
		cout << "Creating KD-Tree with num points: " << points.size() << ", num dimensions: " << num_dimensions << endl;
		kdtree = new KDTree<Point<TCoordinate>, TCoordinate>(num_dimensions);
		kdtree->add_points(points);
	}
	else
	{
		cout <<  "Empty points file... exiting" << endl;
	}

	return kdtree;
}

template<typename TCoordinate>
void save_kdtree (KDTree<Point<TCoordinate>, TCoordinate> *kdtree, const char* archive_file_path)
{
	if (!kdtree)
	{
		cout << "Nothing to save... exiting" << endl;
		return;
	}

	ofstream ofs(archive_file_path);
	boost::archive::text_oarchive oa(ofs);
	oa << kdtree;
}

#endif /* BUILD_KDTREE_H_ */
