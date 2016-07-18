/*
 * query_kdtree_main.cpp
 *
 *  Created on: Jul 16, 2016
 *      Author: Siva
 */


#include <iostream>
#include <cassert>

#include "query_kdtree.h"
#include "kdtree.h"

using namespace std;

/*
 * Inputs:
 * 1. full path for file containing query points
 * 2. full path for the archive file containing kd-tree
 * 3. full path for generating output file (path should include filename)
 */
int main (int argc, char **argv)
{
	cout << "Launching KD-Tree query application..." << endl;

	cout << "Processing input arguments..." << endl;
	if (argc != 4)
	{
		cout << "Program expects 3 arguments but only " << argc-1 << " arguments provided" << endl;
		cout << "Please run the program as follows: ./build full_path_to_query_points_file full_path_to_archive_file full_path_to_output_csv_file " << endl;
		assert (0);
	}

	const char* points_file_path = argv[1];
	const char* archive_file_path = argv[2];
	const char* output_file_path = argv[3];

	cout << "Points file path:" << points_file_path << endl;
	cout << "Archive file path:" << archive_file_path << endl;
	cout << "Output file path:" << output_file_path << endl;

	// load kd-tree
	cout << "Loading KD-Tree..." << endl;
	KDTree<Point<double>, double> *kdtree = load_kdtree<double> (archive_file_path);

	// compute nearest neighbors
	if (kdtree)
	{
		cout << "Computing nearest neighbor for each query point..." << endl;
		compute_nearest_neighbors (kdtree, points_file_path, output_file_path);
	}
	else
	{
		cout << "Tree empty or load failed..." << endl;
	}

	cout << "Exiting query KD-Tree application" << endl;
	delete kdtree;

}
