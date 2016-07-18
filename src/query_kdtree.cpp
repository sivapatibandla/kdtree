/*
 * query_kdtree.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: Siva
 *    	Description: the application reads a list of data points from .csv file
 *    	and stores the points in a kd-tree.
 */


#include <iostream>
#include <fstream>
#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <cmath>

#include "kdtree.h"
#include "util.h"

using namespace std;

void compute_nearest_neighbors (KDTree<Point<float>, float> *kdtree,
		const char* query_points_file_path,
		const char* output_file_path)
{

	ifstream file (query_points_file_path, ifstream::in);
	ofstream out_file (output_file_path, ofstream::out);

	string line;
	vector<std::string> words;
	while (file.good())
	{
		getline(file, line);
		words = split (line, ',');
		// ignore lines which are not properly formatted. TODO: wrap this in a try & catch block
		if (words.size() != kdtree->get_num_dimensions()) continue;

		// extract coordinates of query point
		vector<float> coordinates;
		for (vector<string>::iterator it = words.begin(); it!=words.end(); it++)
		{
			coordinates.push_back(stof(*it));
		}

		Point<float> query_point (coordinates);
		NearestPoint<float> *nearest_point = NULL;
		kdtree->find_nearest_point_recursive(&query_point, &nearest_point);

		if (nearest_point)
		{
			out_file << nearest_point->get_id() << "," << sqrt(nearest_point->get_distance()) << endl;
		}
	}

	file.close();
	out_file.close();
}

KDTree<Point<float>, float> *load_kdtree (const char* archive_file_path)
{
	KDTree<Point<float>, float> *kdtree;
	ifstream ifs(archive_file_path);
	boost::archive::text_iarchive ia(ifs);
	ia >> kdtree;

	return kdtree;
}
