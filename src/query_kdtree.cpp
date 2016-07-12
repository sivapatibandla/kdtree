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
#include "kdtree.h"
#include <fstream>

using namespace std;

extern KDTree<Point<float>, float > *build_main();

KDTree<Point<float>, float > *reconstruct_kdtree (const char* file_name)
{
	KDTree<Point<float>, float > *kdtree = build_main();
	return kdtree;
}

int main() {

	cout << "!!!Application to reconstruct tree and perform nearest neighbor searches on it!!!" << endl; // prints !!!Hello World!!!
	/*
	 * read a query points file and output the indices of nearest points to
	 * each query point
	 *
	 * while file not empty
	 * 	read_point()
	 * 	nearest_point = kdtree->find_nearest_point(query_point)
	 * 	print the index of nearest_point.
	 */
	KDTree<Point<float>, float > *kdtree = reconstruct_kdtree ("saved_kdtree.txt");
	cout << *kdtree << endl;

	const char *file_name = "/Users/Siva/Documents/workspace/kdtree/src/query_data.csv";
	const char *out_file_name = "/Users/Siva/Documents/workspace/kdtree/src/nearest_points_indices_opt.csv";
	ifstream file (file_name, ifstream::in);
	ofstream out_file (out_file_name, ofstream::out);

	string line;
	vector<std::string> words;
	while (file.good())
	{
		getline(file, line);
		words = split (line, ',');
		// ignore lines which are not properly formatted. TODO: wrap this in a try & catch block
		if (words.size() != kdtree->get_num_dimensions()) continue;

		// extract coordinates
		vector<float> coordinates;
		for (vector<string>::iterator it = words.begin(); it!=words.end(); it++)
		{
			coordinates.push_back(stof(*it));
		}

		Point<float> query_point (coordinates);
		float min_distance =  0;
		const Point<float> *min_point = NULL;

		min_point = kdtree->find_nearest_point(&query_point, &min_distance);
		//cout << "kd tree nearest point " << *min_point  << " " << min_point->get_id() << endl;

		//tuple <Point<float> *, float> min_tuple;
		//min_tuple = kdtree->find_nearest_point_brute_force (&query_point);
		//min_point = kdtree->find_nearest_point_brute_force (&query_point, &min_distance);
		/*out_file << "Q:" << query_point << ",P:" << *get<0>(min_tuple) << ",index:" <<
				get<0>(min_tuple)->get_id() << ",distance:" << get<1>(min_tuple) << endl;*/
		/*out_file << "Q:" << query_point << ",P:" << *min_point << ",index:" <<
						min_point->get_id() << ",distance:" << min_distance << endl;*/
		out_file << min_point->get_id() << "," << min_distance << endl;

	}
	out_file.close();
	return 0;
}
