/*
 * query_kdtree.h
 *
 *  Created on: Jul 16, 2016
 *      Author: Siva
 */

#ifndef QUERY_KDTREE_H_
#define QUERY_KDTREE_H_

#include <iostream>
#include <fstream>
#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/lexical_cast.hpp>
#include <cmath>

#include "kdtree.h"
#include "util.h"

using namespace std;

template <typename TCoordinate>
void compute_nearest_neighbors (KDTree<Point<TCoordinate>, TCoordinate> *kdtree,
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
		vector<TCoordinate> coordinates;
		for (vector<string>::iterator it = words.begin(); it!=words.end(); it++)
		{
			coordinates.push_back(boost::lexical_cast<TCoordinate>(*it));
		}

		Point<TCoordinate> query_point (coordinates);
		NearestPoint<TCoordinate> *nearest_point = NULL;
		kdtree->find_nearest_point_recursive(&query_point, &nearest_point);
		//kdtree->find_nearest_point_brute_force (&query_point, &nearest_point);
		if (nearest_point)
		{
			out_file << nearest_point->get_id() << "," << sqrt(nearest_point->get_distance()) << endl;
			delete nearest_point;
		}
	}

	file.close();
	out_file.close();
}

template <typename TCoordinate>
KDTree<Point<TCoordinate>, TCoordinate> *load_kdtree (const char* archive_file_path)
{
	KDTree<Point<TCoordinate>, TCoordinate> *kdtree;
	ifstream ifs(archive_file_path);
	boost::archive::text_iarchive ia(ifs);
	ia >> kdtree;

	return kdtree;
}

#endif /* QUERY_KDTREE_H_ */
