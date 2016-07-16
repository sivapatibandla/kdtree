/*
 * build_kdtree.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: Siva
 */


#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "kdtree.h"
#include "util.h"

using namespace std;

vector <Point<float> *> read_points_from_file (const char *file_path, int &num_dimensions)
{

	ifstream file ( file_path, ifstream::in );
	if (!file.good())
	{
		cout << "Unable to read file at " << file_path << "... please give the correct and full path to the points file" << endl;
		file.close();
		assert (0);
	}

	int point_id = 0;
	vector <Point<float> *> points;

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
		vector<float> coordinates;
		for (vector<string>::iterator it = words.begin(); it!=words.end(); it++)
		{
			coordinates.push_back(stof(*it));
		}

		Point<float> *point = NULL;
		try
		{
		  point = new Point<float>(coordinates, point_id);
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


KDTree<Point<float>, float> *build_kdtree(const char* points_file_path) {

	int num_dimensions = 0;
	KDTree<Point<float>, float> *kdtree = NULL;

	vector <Point<float> *> points = read_points_from_file (points_file_path, num_dimensions);

	// build kd-tree
	if (points.size() > 0)
	{
		cout << "Creating KD-Tree with num points: " << points.size() << ", num dimensions: " << num_dimensions << endl;
		kdtree = new KDTree<Point<float>, float>(num_dimensions);
		kdtree->add_points(points);
		//cout << *kdtree << endl;
	}
	else
	{
		cout <<  "Empty points file... exiting" << endl;
	}

	return kdtree;
}

void save_kdtree (KDTree<Point<float>, float> *kdtree, const char* archive_file_path)
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
